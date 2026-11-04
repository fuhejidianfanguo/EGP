#include <EGP/GraphicsApi/vulkan_impl.hpp>

#include <GLFW/glfw3.h>

#include <stdexcept>
#include <string>
#include <vector>

namespace EGP {

namespace {

constexpr const char* kValidationLayer = "VK_LAYER_KHRONOS_validation";

#ifdef NDEBUG
constexpr bool kEnableValidation = false;
#else
constexpr bool kEnableValidation = true;
#endif

auto validationLayersSupported() -> bool {
  for (const auto& layer : vk::enumerateInstanceLayerProperties()) {
    if (std::string(layer.layerName.data()) == kValidationLayer) {
      return true;
    }
  }
  return false;
}

auto requiredInstanceExtensions() -> std::vector<const char*> {
  std::uint32_t count = 0;
  const char** names = glfwGetRequiredInstanceExtensions(&count);
  if (names == nullptr) {
    throw std::runtime_error("GLFW does not support Vulkan");
  }
  return std::vector<const char*>(names, names + count);
}

auto debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severity,
                   VkDebugUtilsMessageTypeFlagsEXT,
                   const VkDebugUtilsMessengerCallbackDataEXT*,
                   void*) -> VKAPI_ATTR VkBool32 {
  if (severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
  }
  return VK_FALSE;
}

}  // namespace

VulkanAPI::VulkanAPI(void* nativeWindowHandle)
    : nativeWindowHandle_(nativeWindowHandle) {
  createInstance();
}

VulkanAPI::~VulkanAPI() = default;

auto VulkanAPI::init(void* nativeWindowHandle) -> void {
  nativeWindowHandle_ = nativeWindowHandle;
}

auto VulkanAPI::clear(const glm::vec4& color, float depth) -> void {
  (void)color;
  (void)depth;
}

auto VulkanAPI::createInstance() -> void {
  vk::ApplicationInfo appInfo;
  appInfo.setPApplicationName("EGP");
  appInfo.setApplicationVersion(VK_MAKE_VERSION(0, 0, 1));
  appInfo.setPEngineName("EGP");
  appInfo.setEngineVersion(VK_MAKE_VERSION(0, 0, 1));
  appInfo.setApiVersion(VK_API_VERSION_1_3);

  auto layers = std::vector<const char*>{};
  if (kEnableValidation) {
    if (!validationLayersSupported()) {
      throw std::runtime_error("Validation layer not available");
    }
    layers.push_back(kValidationLayer);
  }

  auto extensions = requiredInstanceExtensions();
  extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

  vk::InstanceCreateInfo createInfo;
  createInfo.setPApplicationInfo(&appInfo);
  createInfo.setPEnabledLayerNames(layers);
  createInfo.setPEnabledExtensionNames(extensions);

  instance_ = vk::createInstance(createInfo);
}

}  // namespace EGP
