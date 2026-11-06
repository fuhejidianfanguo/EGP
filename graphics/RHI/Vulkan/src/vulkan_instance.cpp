#include <EGP/GraphicsApi/vulkan_impl.hpp>

#include <GLFW/glfw3.h>

#include <stdexcept>
#include <string>
#include <vector>

#include "vulkan_internal.hpp"

namespace EGP {

namespace {

auto validationLayersSupported() -> bool {
  for (const auto& layer : vk::enumerateInstanceLayerProperties()) {
    if (std::string(layer.layerName.data()) == detail::kValidationLayer) {
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

}  // namespace

auto VulkanAPI::createInstance() -> void {
  vk::ApplicationInfo appInfo;
  appInfo.setPApplicationName("EGP");
  appInfo.setApplicationVersion(VK_MAKE_VERSION(0, 0, 1));
  appInfo.setPEngineName("EGP");
  appInfo.setEngineVersion(VK_MAKE_VERSION(0, 0, 1));
  appInfo.setApiVersion(VK_API_VERSION_1_3);

  auto layers = std::vector<const char*>{};
  if (detail::kEnableValidation) {
    if (!validationLayersSupported()) {
      throw std::runtime_error("Validation layer not available");
    }
    layers.push_back(detail::kValidationLayer);
  }

  auto extensions = requiredInstanceExtensions();
  extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

  vk::InstanceCreateInfo createInfo;
  createInfo.setPApplicationInfo(&appInfo);
  createInfo.setPEnabledLayerNames(layers);
  createInfo.setPEnabledExtensionNames(extensions);

  instance_ = vk::createInstance(createInfo);
}

auto VulkanAPI::createDebugMessenger() -> void {
  if (!detail::kEnableValidation) {
    return;
  }
  vk::DebugUtilsMessengerCreateInfoEXT createInfo;
  createInfo.setMessageSeverity(
      vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
      vk::DebugUtilsMessageSeverityFlagBitsEXT::eError);
  createInfo.setMessageType(
      vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
      vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
      vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance);
  createInfo.setPfnUserCallback(&detail::debugCallback);
  vk::detail::DispatchLoaderDynamic dispatch(instance_, vkGetInstanceProcAddr);
  debugMessenger_ =
      instance_.createDebugUtilsMessengerEXT(createInfo, nullptr, dispatch);
}

auto VulkanAPI::createSurface() -> void {
  if (nativeWindowHandle_ == nullptr) {
    throw std::runtime_error("Native window handle is null");
  }
  VkSurfaceKHR surface = VK_NULL_HANDLE;
  const auto result = glfwCreateWindowSurface(
      instance_, static_cast<GLFWwindow*>(nativeWindowHandle_), nullptr,
      &surface);
  if (result != VK_SUCCESS) {
    throw std::runtime_error("Failed to create window surface");
  }
  surface_ = vk::SurfaceKHR(surface);
}

}  // namespace EGP
