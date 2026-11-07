#include <EGP/GraphicsApi/vulkan_impl.hpp>

#include <set>
#include <stdexcept>
#include <vector>

#include "vulkan_internal.hpp"

namespace EGP {

auto VulkanAPI::pickPhysicalDevice() -> void {
  for (const auto& candidate : instance_.enumeratePhysicalDevices()) {
    std::uint32_t graphicsFamily = UINT32_MAX;
    std::uint32_t presentFamily = UINT32_MAX;
    const auto families = candidate.getQueueFamilyProperties();
    for (std::uint32_t i = 0; i < families.size(); ++i) {
      if ((families[i].queueFlags & vk::QueueFlagBits::eGraphics) !=
          vk::QueueFlags{}) {
        graphicsFamily = i;
      }
      if (candidate.getSurfaceSupportKHR(i, surface_) == VK_TRUE) {
        presentFamily = i;
      }
    }
    if (graphicsFamily == UINT32_MAX || presentFamily == UINT32_MAX) {
      continue;
    }
    std::uint32_t formatCount = 0;
    std::uint32_t presentModeCount = 0;
    candidate.getSurfaceFormatsKHR(surface_, &formatCount, nullptr);
    candidate.getSurfacePresentModesKHR(surface_, &presentModeCount, nullptr);
    if (formatCount == 0 || presentModeCount == 0) {
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
  const float priority = 1.0F;
  std::vector<vk::DeviceQueueCreateInfo> queueInfos;
  for (const auto family : families) {
    vk::DeviceQueueCreateInfo info;
    info.setQueueFamilyIndex(family);
    info.setQueueCount(1);
    info.setPQueuePriorities(&priority);
    queueInfos.push_back(info);
  }

  const auto features = physicalDevice_.getFeatures();
  vk::PhysicalDeviceFeatures enabled;
  enabled.setSamplerAnisotropy(features.samplerAnisotropy);

  vk::DeviceCreateInfo createInfo;
  createInfo.setQueueCreateInfos(queueInfos);
  createInfo.setPEnabledFeatures(&enabled);
  createInfo.setPEnabledExtensionNames(detail::kDeviceExtensions);

  device_ = physicalDevice_.createDevice(createInfo);
  graphicsQueue_ = device_.getQueue(graphicsQueueFamily_, 0);
  presentQueue_ = device_.getQueue(presentQueueFamily_, 0);
}

}  // namespace EGP
