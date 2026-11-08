#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <EGP/GraphicsApi/vulkan_impl.hpp>
#include <algorithm>
#include <stdexcept>
#include <string_view>
#include <vector>

#include "vulkan_internal.hpp"

namespace EGP {

namespace {

auto validationLayersSupported() -> bool {
  return std::ranges::any_of(
      vk::enumerateInstanceLayerProperties(), [](const auto& layer) {
        return std::string_view{layer.layerName.data()} ==
               detail::kValidationLayer;
      });
}

auto requiredInstanceExtensions() -> std::vector<const char*> {
  std::uint32_t count = 0;
  const char* const* names = glfwGetRequiredInstanceExtensions(&count);
  if (names == nullptr) {
    throw std::runtime_error("GLFW does not support Vulkan");
  }
  return {names, names + count};
}

}  // namespace

auto VulkanAPI::createInstance() -> void {
  vk::ApplicationInfo appInfo;
  appInfo.setPApplicationName("EGP");
  appInfo.setApplicationVersion(vk::makeApiVersion(0, 0, 0, 1));
  appInfo.setPEngineName("EGP");
  appInfo.setEngineVersion(vk::makeApiVersion(0, 0, 0, 1));
  appInfo.setApiVersion(vk::ApiVersion13);

  std::vector<const char*> layers;
  if constexpr (detail::kEnableValidation) {
    if (!validationLayersSupported()) {
      throw std::runtime_error("Validation layer not available");
    }
    layers.push_back(detail::kValidationLayer);
  }

  auto extensions = requiredInstanceExtensions();
  extensions.push_back(vk::EXTDebugUtilsExtensionName);

  vk::InstanceCreateInfo createInfo;
  createInfo.setPApplicationInfo(&appInfo);
  createInfo.setPEnabledLayerNames(layers);
  createInfo.setPEnabledExtensionNames(extensions);

  instance_ = vk::createInstance(createInfo);
}

auto VulkanAPI::createDebugMessenger() -> void {
  if constexpr (!detail::kEnableValidation) {
    return;
  }

  vk::DebugUtilsMessengerCreateInfoEXT createInfo;
  createInfo.setMessageSeverity(
      vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
      vk::DebugUtilsMessageSeverityFlagBitsEXT::eError);
  createInfo.setMessageType(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
                            vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
                            vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance);
  createInfo.setPfnUserCallback(&detail::debugCallback);

  const vk::detail::DispatchLoaderDynamic dispatch(instance_,
                                                   vkGetInstanceProcAddr);
  debugMessenger_ =
      instance_.createDebugUtilsMessengerEXT(createInfo, nullptr, dispatch);
}

auto VulkanAPI::createSurface() -> void {
  if (nativeWindowHandle_ == nullptr) {
    throw std::runtime_error("Native window handle is null");
  }

  VkSurfaceKHR surface = nullptr;
  const auto result = static_cast<vk::Result>(glfwCreateWindowSurface(
      instance_, static_cast<GLFWwindow*>(nativeWindowHandle_), nullptr,
      &surface));
  if (result != vk::Result::eSuccess) {
    throw std::runtime_error("Failed to create window surface");
  }
  surface_ = vk::SurfaceKHR{surface};
}

}  // namespace EGP
