#pragma once

#include <array>
#include <cstdint>
#include <iostream>
#include <vulkan/vulkan.hpp>

namespace EGP::detail {

inline constexpr const char* kValidationLayer = "VK_LAYER_KHRONOS_validation";

inline const std::array<const char*, 1> kDeviceExtensions{
    vk::KHRSwapchainExtensionName};

inline constexpr std::uint32_t kMaxFramesInFlight = 2;

#ifdef NDEBUG
inline constexpr bool kEnableValidation = false;
#else
inline constexpr bool kEnableValidation = true;
#endif

inline auto debugCallback(vk::DebugUtilsMessageSeverityFlagBitsEXT severity,
                          vk::DebugUtilsMessageTypeFlagsEXT,
                          const vk::DebugUtilsMessengerCallbackDataEXT* data,
                          void*) -> vk::Bool32 {
  if (severity >= vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning) {
    if (data != nullptr && data->pMessage != nullptr) {
      std::cerr << "[vulkan] " << data->pMessage << '\n';
    }
  }
  return vk::False;
}

}  // namespace EGP::detail
