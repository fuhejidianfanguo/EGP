#include <GLFW/glfw3.h>

#include <EGP/GraphicsApi/vulkan_impl.hpp>
#include <algorithm>
#include <array>
#include <iterator>
#include <limits>

namespace EGP {

auto VulkanAPI::createSwapchain() -> void {
  const auto capabilities = physicalDevice_.getSurfaceCapabilitiesKHR(surface_);

  const auto availableFormats = physicalDevice_.getSurfaceFormatsKHR(surface_);
  const vk::SurfaceFormatKHR preferredFormat{vk::Format::eB8G8R8A8Srgb,
                                             vk::ColorSpaceKHR::eSrgbNonlinear};
  const auto formatIt = std::ranges::find(availableFormats, preferredFormat);
  const auto format =
      formatIt != availableFormats.end() ? *formatIt : availableFormats.front();

  const auto presentModes = physicalDevice_.getSurfacePresentModesKHR(surface_);
  const auto presentMode =
      std::ranges::find(presentModes, vk::PresentModeKHR::eMailbox) !=
              presentModes.end()
          ? vk::PresentModeKHR::eMailbox
          : vk::PresentModeKHR::eFifo;

  const auto extent = [&] {
    if (capabilities.currentExtent.width !=
        std::numeric_limits<std::uint32_t>::max()) {
      return capabilities.currentExtent;
    }
    int width = 0;
    int height = 0;
    glfwGetFramebufferSize(static_cast<GLFWwindow*>(nativeWindowHandle_),
                           &width, &height);
    return vk::Extent2D{std::clamp(static_cast<std::uint32_t>(width),
                                   capabilities.minImageExtent.width,
                                   capabilities.maxImageExtent.width),
                        std::clamp(static_cast<std::uint32_t>(height),
                                   capabilities.minImageExtent.height,
                                   capabilities.maxImageExtent.height)};
  }();

  const auto imageCount =
      capabilities.maxImageCount > 0
          ? std::min(capabilities.minImageCount + 1, capabilities.maxImageCount)
          : capabilities.minImageCount + 1;

  vk::SwapchainCreateInfoKHR createInfo;
  createInfo.setSurface(surface_);
  createInfo.setMinImageCount(imageCount);
  createInfo.setImageFormat(format.format);
  createInfo.setImageColorSpace(format.colorSpace);
  createInfo.setImageExtent(extent);
  createInfo.setImageArrayLayers(1);
  createInfo.setImageUsage(vk::ImageUsageFlagBits::eColorAttachment);
  createInfo.setPreTransform(capabilities.currentTransform);
  createInfo.setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque);
  createInfo.setPresentMode(presentMode);
  createInfo.setClipped(vk::True);
  createInfo.setImageSharingMode(vk::SharingMode::eExclusive);

  const std::array familyIndices{graphicsQueueFamily_, presentQueueFamily_};
  if (graphicsQueueFamily_ != presentQueueFamily_) {
    createInfo.setImageSharingMode(vk::SharingMode::eConcurrent);
    createInfo.setQueueFamilyIndices(familyIndices);
  }

  swapchain_ = device_.createSwapchainKHR(createInfo);
  swapchainFormat_ = format.format;
  swapchainExtent_ = extent;
  swapchainImages_ = device_.getSwapchainImagesKHR(swapchain_);
}

auto VulkanAPI::createImageViews() -> void {
  const auto components = vk::ComponentMapping{
      vk::ComponentSwizzle::eIdentity, vk::ComponentSwizzle::eIdentity,
      vk::ComponentSwizzle::eIdentity, vk::ComponentSwizzle::eIdentity};
  const auto range =
      vk::ImageSubresourceRange{vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1};

  swapchainImageViews_.clear();
  swapchainImageViews_.reserve(swapchainImages_.size());
  std::ranges::transform(swapchainImages_,
                         std::back_inserter(swapchainImageViews_),
                         [&](const vk::Image image) {
                           vk::ImageViewCreateInfo createInfo;
                           createInfo.setImage(image);
                           createInfo.setViewType(vk::ImageViewType::e2D);
                           createInfo.setFormat(swapchainFormat_);
                           createInfo.setComponents(components);
                           createInfo.setSubresourceRange(range);
                           return device_.createImageView(createInfo);
                         });
}

}  // namespace EGP
