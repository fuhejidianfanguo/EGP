#include <EGP/GraphicsApi/vulkan_impl.hpp>
#include <limits>
#include <set>
#include <stdexcept>
#include <vector>

#include "vulkan_internal.hpp"

namespace EGP {

auto VulkanAPI::pickPhysicalDevice() -> void {
  constexpr auto kInvalidFamily = std::numeric_limits<std::uint32_t>::max();

  for (const auto candidate : instance_.enumeratePhysicalDevices()) {
    const auto families = candidate.getQueueFamilyProperties();
    auto graphicsFamily = kInvalidFamily;
    auto presentFamily = kInvalidFamily;

    for (std::uint32_t i = 0; i < families.size(); ++i) {
      if (families[i].queueFlags & vk::QueueFlagBits::eGraphics) {
        graphicsFamily = i;
      }
      if (candidate.getSurfaceSupportKHR(i, surface_) == vk::True) {
        presentFamily = i;
      }
    }

    if (graphicsFamily == kInvalidFamily || presentFamily == kInvalidFamily) {
      continue;
    }
    if (candidate.getSurfaceFormatsKHR(surface_).empty() ||
        candidate.getSurfacePresentModesKHR(surface_).empty()) {
      continue;
    }

    physicalDevice_ = candidate;
    graphicsQueueFamily_ = graphicsFamily;
    presentQueueFamily_ = presentFamily;
    return;
  }

  throw std::runtime_error("No suitable Vulkan physical device found");
}

auto VulkanAPI::createDevice() -> void {
  const std::set<std::uint32_t> families{graphicsQueueFamily_,
                                         presentQueueFamily_};
  constexpr float priority = 1.0F;
  std::vector<vk::DeviceQueueCreateInfo> queueInfos;
  for (const auto family : families) {
    vk::DeviceQueueCreateInfo info;
    info.setQueueFamilyIndex(family);
    info.setQueueCount(1);
    info.setPQueuePriorities(&priority);
    queueInfos.push_back(info);
  }

  const auto dynamicRenderingSupported =
      physicalDevice_
          .getFeatures2<vk::PhysicalDeviceFeatures2,
                        vk::PhysicalDeviceVulkan13Features>()
          .get<vk::PhysicalDeviceVulkan13Features>()
          .dynamicRendering == vk::True;
  if (!dynamicRenderingSupported) {
    throw std::runtime_error("Dynamic rendering is not supported");
  }

  vk::PhysicalDeviceVulkan13Features vulkan13Features;
  vulkan13Features.setDynamicRendering(vk::True);

  const auto features = physicalDevice_.getFeatures();
  vk::PhysicalDeviceFeatures enabled;
  enabled.setSamplerAnisotropy(features.samplerAnisotropy);

  vk::DeviceCreateInfo createInfo;
  createInfo.setQueueCreateInfos(queueInfos);
  createInfo.setPEnabledFeatures(&enabled);
  createInfo.setPEnabledExtensionNames(detail::kDeviceExtensions);
  createInfo.setPNext(&vulkan13Features);

  device_ = physicalDevice_.createDevice(createInfo);
  graphicsQueue_ = device_.getQueue(graphicsQueueFamily_, 0);
  presentQueue_ = device_.getQueue(presentQueueFamily_, 0);
}

}  // namespace EGP
