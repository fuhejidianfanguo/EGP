#pragma once

#include <cstdint>
#include <iostream>

#include <vulkan/vulkan.hpp>

namespace EGP::detail {

inline constexpr const char* kValidationLayer = "VK_LAYER_KHRONOS_validation";

inline constexpr const char* kDeviceExtensions[] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

inline constexpr std::uint32_t kMaxFramesInFlight = 2;

#ifdef NDEBUG
inline constexpr bool kEnableValidation = false;
#else
inline constexpr bool kEnableValidation = true;
#endif

inline auto debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severity,
                          VkDebugUtilsMessageTypeFlagsEXT,
                          const VkDebugUtilsMessengerCallbackDataEXT* data,
                          void*) -> VKAPI_ATTR VkBool32 {
  if (severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
    if (data != nullptr && data->pMessage != nullptr) {
      std::cerr << "[vulkan] " << data->pMessage << '\n';
    }
  }
  return VK_FALSE;
}

}  // namespace EGP::detail
