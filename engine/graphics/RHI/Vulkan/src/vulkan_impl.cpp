#include <EGP/GraphicsApi/vulkan_impl.hpp>
#include <limits>
#include <stdexcept>

#include "vulkan_internal.hpp"

namespace EGP {

VulkanAPI::VulkanAPI(void* nativeWindowHandle)
    : nativeWindowHandle_(nativeWindowHandle) {}

VulkanAPI::~VulkanAPI() { destroy(); }

auto VulkanAPI::destroy() -> void {
  if (device_) {
    device_.waitIdle();
    destroySwapchainResources();

    if (commandPool_) {
      device_.destroyCommandPool(commandPool_);
      commandPool_ = nullptr;
    }

    device_.destroy();
    device_ = nullptr;
  }

  if (debugMessenger_) {
    const vk::detail::DispatchLoaderDynamic dispatch(instance_,
                                                     vkGetInstanceProcAddr);
    instance_.destroyDebugUtilsMessengerEXT(debugMessenger_, nullptr, dispatch);
    debugMessenger_ = nullptr;
  }

  if (surface_) {
    instance_.destroySurfaceKHR(surface_);
    surface_ = nullptr;
  }

  if (instance_) {
    instance_.destroy();
    instance_ = nullptr;
  }
}

auto VulkanAPI::init(void* nativeWindowHandle) -> void {
  nativeWindowHandle_ = nativeWindowHandle;
  createInstance();
  createDebugMessenger();
  createSurface();
  pickPhysicalDevice();
  createDevice();
  createSwapchain();
  createImageViews();
  createCommandPool();
  createCommandBuffers();
  createSyncObjects();
}

auto VulkanAPI::clear(const glm::vec4& color, float) -> void {
  clearColor_ = color;
}

auto VulkanAPI::drawFrame() -> void {
  if (framebufferResized_) {
    framebufferResized_ = false;
    resize();
  }

  const auto& fence = inFlightFences_[currentFrame_];
  if (device_.waitForFences(fence, vk::True,
                            std::numeric_limits<std::uint64_t>::max()) !=
      vk::Result::eSuccess) {
    throw std::runtime_error("Failed to wait for in-flight fence");
  }

  std::uint32_t imageIndex = 0;
  const auto acquireResult = device_.acquireNextImageKHR(
      swapchain_, std::numeric_limits<std::uint64_t>::max(),
      imageAvailableSemaphores_[currentFrame_], nullptr, &imageIndex);
  if (acquireResult == vk::Result::eErrorOutOfDateKHR) {
    framebufferResized_ = true;
    return;
  }
  if (acquireResult != vk::Result::eSuccess &&
      acquireResult != vk::Result::eSuboptimalKHR) {
    throw std::runtime_error("Failed to acquire swapchain image");
  }

  device_.resetFences(fence);
  recordCommandBuffer(commandBuffers_[imageIndex], imageIndex);

  const auto waitSemaphore = imageAvailableSemaphores_[currentFrame_];
  const auto waitStage =
      vk::PipelineStageFlags{vk::PipelineStageFlagBits::eColorAttachmentOutput};
  const auto commandBuffer = commandBuffers_[imageIndex];
  const auto signalSemaphore = renderFinishedSemaphores_[imageIndex];

  vk::SubmitInfo submitInfo;
  submitInfo.setWaitSemaphores(waitSemaphore);
  submitInfo.setWaitDstStageMask(waitStage);
  submitInfo.setCommandBuffers(commandBuffer);
  submitInfo.setSignalSemaphores(signalSemaphore);
  graphicsQueue_.submit(submitInfo, fence);

  vk::PresentInfoKHR presentInfo;
  presentInfo.setWaitSemaphores(signalSemaphore);
  presentInfo.setSwapchains(swapchain_);
  presentInfo.setImageIndices(imageIndex);
  const auto presentResult = presentQueue_.presentKHR(presentInfo);
  if (presentResult == vk::Result::eErrorOutOfDateKHR ||
      presentResult == vk::Result::eSuboptimalKHR) {
    framebufferResized_ = true;
  } else if (presentResult != vk::Result::eSuccess) {
    throw std::runtime_error("Failed to present swapchain image");
  }

  currentFrame_ = (currentFrame_ + 1) % detail::kMaxFramesInFlight;
}

auto VulkanAPI::onFramebufferResized() -> void { framebufferResized_ = true; }

auto VulkanAPI::resize() -> void {
  const auto capabilities = physicalDevice_.getSurfaceCapabilitiesKHR(surface_);
  if (capabilities.currentExtent.width == 0 ||
      capabilities.currentExtent.height == 0) {
    return;
  }

  device_.waitIdle();
  destroySwapchainResources();

  createSwapchain();
  createImageViews();
  createCommandBuffers();
  createSyncObjects();
  currentFrame_ = 0;
}

auto VulkanAPI::destroySwapchainResources() -> void {
  for (const auto semaphore : renderFinishedSemaphores_) {
    device_.destroySemaphore(semaphore);
  }
  renderFinishedSemaphores_.clear();

  for (const auto semaphore : imageAvailableSemaphores_) {
    device_.destroySemaphore(semaphore);
  }
  imageAvailableSemaphores_.clear();

  for (const auto fence : inFlightFences_) {
    device_.destroyFence(fence);
  }
  inFlightFences_.clear();

  if (!commandBuffers_.empty()) {
    device_.freeCommandBuffers(commandPool_, commandBuffers_);
  }
  commandBuffers_.clear();

  for (const auto view : swapchainImageViews_) {
    device_.destroyImageView(view);
  }
  swapchainImageViews_.clear();
  swapchainImages_.clear();

  device_.destroySwapchainKHR(swapchain_);
  swapchain_ = nullptr;
}

auto VulkanAPI::recordCommandBuffer(vk::CommandBuffer commandBuffer,
                                    std::uint32_t imageIndex) -> void {
  const vk::CommandBufferBeginInfo beginInfo{};
  commandBuffer.begin(beginInfo);

  vk::ClearColorValue color;
  color.setFloat32(
      {clearColor_.r, clearColor_.g, clearColor_.b, clearColor_.a});
  vk::ClearValue clearValue;
  clearValue.setColor(color);

  vk::RenderingAttachmentInfo attachmentInfo;
  attachmentInfo.setImageView(swapchainImageViews_[imageIndex]);
  attachmentInfo.setImageLayout(vk::ImageLayout::eColorAttachmentOptimal);
  attachmentInfo.setLoadOp(vk::AttachmentLoadOp::eClear);
  attachmentInfo.setStoreOp(vk::AttachmentStoreOp::eStore);
  attachmentInfo.setClearValue(clearValue);

  vk::RenderingInfo renderingInfo;
  renderingInfo.setRenderArea(vk::Rect2D{{0, 0}, swapchainExtent_});
  renderingInfo.setLayerCount(1);
  renderingInfo.setColorAttachments(attachmentInfo);

  const vk::ImageSubresourceRange range{vk::ImageAspectFlagBits::eColor, 0, 1,
                                        0, 1};

  vk::ImageMemoryBarrier toColorAttachment;
  toColorAttachment.setOldLayout(vk::ImageLayout::eUndefined);
  toColorAttachment.setNewLayout(vk::ImageLayout::eColorAttachmentOptimal);
  toColorAttachment.setSrcQueueFamilyIndex(vk::QueueFamilyIgnored);
  toColorAttachment.setDstQueueFamilyIndex(vk::QueueFamilyIgnored);
  toColorAttachment.setImage(swapchainImages_[imageIndex]);
  toColorAttachment.setSubresourceRange(range);
  toColorAttachment.setSrcAccessMask({});
  toColorAttachment.setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite);

  commandBuffer.pipelineBarrier(
      vk::PipelineStageFlagBits::eTopOfPipe,
      vk::PipelineStageFlagBits::eColorAttachmentOutput, vk::DependencyFlags{},
      {}, {}, toColorAttachment);

  commandBuffer.beginRendering(renderingInfo);
  commandBuffer.endRendering();

  vk::ImageMemoryBarrier toPresent;
  toPresent.setOldLayout(vk::ImageLayout::eColorAttachmentOptimal);
  toPresent.setNewLayout(vk::ImageLayout::ePresentSrcKHR);
  toPresent.setSrcQueueFamilyIndex(vk::QueueFamilyIgnored);
  toPresent.setDstQueueFamilyIndex(vk::QueueFamilyIgnored);
  toPresent.setImage(swapchainImages_[imageIndex]);
  toPresent.setSubresourceRange(range);
  toPresent.setSrcAccessMask(vk::AccessFlagBits::eColorAttachmentWrite);
  toPresent.setDstAccessMask({});

  commandBuffer.pipelineBarrier(
      vk::PipelineStageFlagBits::eColorAttachmentOutput,
      vk::PipelineStageFlagBits::eBottomOfPipe, vk::DependencyFlags{}, {}, {},
      toPresent);

  commandBuffer.end();
}

}  // namespace EGP
