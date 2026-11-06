#include <EGP/GraphicsApi/vulkan_impl.hpp>

#include <GLFW/glfw3.h>

#include <algorithm>
#include <array>

#include "vulkan_internal.hpp"

namespace EGP {

auto VulkanAPI::createSwapchain() -> void {
  const auto capabilities = physicalDevice_.getSurfaceCapabilitiesKHR(surface_);

  auto format = vk::Format::eB8G8R8A8Srgb;
  auto colorSpace = vk::ColorSpaceKHR::eSrgbNonlinear;
  const auto availableFormats = physicalDevice_.getSurfaceFormatsKHR(surface_);
  bool formatFound = false;
  for (const auto& candidate : availableFormats) {
    if (candidate.format == format && candidate.colorSpace == colorSpace) {
      formatFound = true;
      break;
    }
  }
  if (!formatFound) {
    format = availableFormats.front().format;
    colorSpace = availableFormats.front().colorSpace;
  }

  auto presentMode = vk::PresentModeKHR::eFifo;
  for (const auto& candidate :
       physicalDevice_.getSurfacePresentModesKHR(surface_)) {
    if (candidate == vk::PresentModeKHR::eMailbox) {
      presentMode = candidate;
      break;
    }
  }

  vk::Extent2D extent = capabilities.currentExtent;
  if (extent.width == UINT32_MAX) {
    int width = 0;
    int height = 0;
    glfwGetFramebufferSize(static_cast<GLFWwindow*>(nativeWindowHandle_), &width,
                           &height);
    extent.width = std::clamp(static_cast<std::uint32_t>(width),
                              capabilities.minImageExtent.width,
                              capabilities.maxImageExtent.width);
    extent.height = std::clamp(static_cast<std::uint32_t>(height),
                               capabilities.minImageExtent.height,
                               capabilities.maxImageExtent.height);
  }

  std::uint32_t imageCount = capabilities.minImageCount + 1;
  if (capabilities.maxImageCount > 0 &&
      imageCount > capabilities.maxImageCount) {
    imageCount = capabilities.maxImageCount;
  }

  vk::SwapchainCreateInfoKHR createInfo;
  createInfo.setSurface(surface_);
  createInfo.setMinImageCount(imageCount);
  createInfo.setImageFormat(format);
  createInfo.setImageColorSpace(colorSpace);
  createInfo.setImageExtent(extent);
  createInfo.setImageArrayLayers(1);
  createInfo.setImageUsage(vk::ImageUsageFlagBits::eColorAttachment);
  createInfo.setPreTransform(capabilities.currentTransform);
  createInfo.setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque);
  createInfo.setPresentMode(presentMode);
  createInfo.setClipped(VK_TRUE);
  createInfo.setImageSharingMode(vk::SharingMode::eExclusive);

  const std::array<std::uint32_t, 2> familyIndices{graphicsQueueFamily_,
                                                   presentQueueFamily_};
  if (graphicsQueueFamily_ != presentQueueFamily_) {
    createInfo.setImageSharingMode(vk::SharingMode::eConcurrent);
    createInfo.setQueueFamilyIndices(familyIndices);
  }

  swapchain_ = device_.createSwapchainKHR(createInfo);
  swapchainFormat_ = format;
  swapchainExtent_ = extent;
  swapchainImages_ = device_.getSwapchainImagesKHR(swapchain_);
}

auto VulkanAPI::createImageViews() -> void {
  swapchainImageViews_.clear();
  swapchainImageViews_.reserve(swapchainImages_.size());
  for (const auto& image : swapchainImages_) {
    vk::ImageViewCreateInfo createInfo;
    createInfo.setImage(image);
    createInfo.setViewType(vk::ImageViewType::e2D);
    createInfo.setFormat(swapchainFormat_);
    vk::ComponentMapping components;
    components.setR(vk::ComponentSwizzle::eIdentity);
    components.setG(vk::ComponentSwizzle::eIdentity);
    components.setB(vk::ComponentSwizzle::eIdentity);
    components.setA(vk::ComponentSwizzle::eIdentity);
    createInfo.setComponents(components);
    vk::ImageSubresourceRange range;
    range.setAspectMask(vk::ImageAspectFlagBits::eColor);
    range.setBaseMipLevel(0);
    range.setLevelCount(1);
    range.setBaseArrayLayer(0);
    range.setLayerCount(1);
    createInfo.setSubresourceRange(range);
    swapchainImageViews_.push_back(device_.createImageView(createInfo));
  }
}

auto VulkanAPI::createRenderPass() -> void {
  vk::AttachmentDescription colorAttachment;
  colorAttachment.setFormat(swapchainFormat_);
  colorAttachment.setSamples(vk::SampleCountFlagBits::e1);
  colorAttachment.setLoadOp(vk::AttachmentLoadOp::eClear);
  colorAttachment.setStoreOp(vk::AttachmentStoreOp::eStore);
  colorAttachment.setStencilLoadOp(vk::AttachmentLoadOp::eDontCare);
  colorAttachment.setStencilStoreOp(vk::AttachmentStoreOp::eDontCare);
  colorAttachment.setInitialLayout(vk::ImageLayout::eUndefined);
  colorAttachment.setFinalLayout(vk::ImageLayout::ePresentSrcKHR);

  vk::AttachmentReference colorRef;
  colorRef.setAttachment(0);
  colorRef.setLayout(vk::ImageLayout::eColorAttachmentOptimal);

  vk::SubpassDescription subpass;
  subpass.setPipelineBindPoint(vk::PipelineBindPoint::eGraphics);
  subpass.setColorAttachments(colorRef);

  vk::SubpassDependency dependency;
  dependency.setSrcSubpass(VK_SUBPASS_EXTERNAL);
  dependency.setDstSubpass(0);
  dependency.setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput);
  dependency.setSrcAccessMask(vk::AccessFlags{});
  dependency.setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput);
  dependency.setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite);

  vk::RenderPassCreateInfo createInfo;
  createInfo.setAttachments(colorAttachment);
  createInfo.setSubpasses(subpass);
  createInfo.setDependencies(dependency);

  renderPass_ = device_.createRenderPass(createInfo);
}

auto VulkanAPI::createFramebuffers() -> void {
  framebuffers_.clear();
  framebuffers_.reserve(swapchainImageViews_.size());
  for (const auto& view : swapchainImageViews_) {
    vk::FramebufferCreateInfo createInfo;
    createInfo.setRenderPass(renderPass_);
    createInfo.setAttachments(view);
    createInfo.setWidth(swapchainExtent_.width);
    createInfo.setHeight(swapchainExtent_.height);
    createInfo.setLayers(1);
    framebuffers_.push_back(device_.createFramebuffer(createInfo));
  }
}

}  // namespace EGP
