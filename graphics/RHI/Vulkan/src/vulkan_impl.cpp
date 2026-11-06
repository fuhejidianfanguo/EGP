#include <EGP/GraphicsApi/vulkan_impl.hpp>

#include <stdexcept>

#include "vulkan_internal.hpp"

namespace EGP {

VulkanAPI::VulkanAPI(void* nativeWindowHandle)
    : nativeWindowHandle_(nativeWindowHandle) {}

VulkanAPI::~VulkanAPI() = default;

auto VulkanAPI::init(void* nativeWindowHandle) -> void {
  nativeWindowHandle_ = nativeWindowHandle;
  createInstance();
  createDebugMessenger();
  createSurface();
  pickPhysicalDevice();
  createDevice();
  createSwapchain();
  createImageViews();
  createRenderPass();
  createFramebuffers();
  createCommandPool();
  createCommandBuffers();
  createSyncObjects();
}

auto VulkanAPI::clear(const glm::vec4& color, float depth) -> void {
  clearColor_ = color;
  clearDepth_ = depth;
}

auto VulkanAPI::drawFrame() -> void {
  if (framebufferResized_) {
    framebufferResized_ = false;
    resize();
  }

  auto& fence = inFlightFences_[currentFrame_];
  const auto waitResult = device_.waitForFences(fence, VK_TRUE, UINT64_MAX);
  if (waitResult != vk::Result::eSuccess) {
    throw std::runtime_error("Failed to wait for in-flight fence");
  }

  std::uint32_t imageIndex = 0;
  const auto acquireResult = device_.acquireNextImageKHR(
      swapchain_, UINT64_MAX, imageAvailableSemaphores_[currentFrame_], nullptr,
      &imageIndex);
  if (acquireResult == vk::Result::eErrorOutOfDateKHR) {
    framebufferResized_ = true;
    return;
  }
  if (acquireResult != vk::Result::eSuccess &&
      acquireResult != vk::Result::eSuboptimalKHR) {
    throw std::runtime_error("Failed to acquire swapchain image");
  }

  device_.resetFences(fence);
  recordCommandBuffer(commandBuffers_[currentFrame_], imageIndex);

  const vk::PipelineStageFlags waitStage =
      vk::PipelineStageFlagBits::eColorAttachmentOutput;
  vk::SubmitInfo submitInfo;
  submitInfo.setWaitSemaphores(imageAvailableSemaphores_[currentFrame_]);
  submitInfo.setWaitDstStageMask(waitStage);
  submitInfo.setCommandBuffers(commandBuffers_[currentFrame_]);
  submitInfo.setSignalSemaphores(renderFinishedSemaphores_[imageIndex]);
  graphicsQueue_.submit(submitInfo, fence);

  vk::PresentInfoKHR presentInfo;
  presentInfo.setWaitSemaphores(renderFinishedSemaphores_[imageIndex]);
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
  createFramebuffers();
  createCommandBuffers();
  createSyncObjects();
  currentFrame_ = 0;
}

auto VulkanAPI::destroySwapchainResources() -> void {
  for (const auto& semaphore : renderFinishedSemaphores_) {
    device_.destroySemaphore(semaphore);
  }
  renderFinishedSemaphores_.clear();

  for (const auto& semaphore : imageAvailableSemaphores_) {
    device_.destroySemaphore(semaphore);
  }
  imageAvailableSemaphores_.clear();

  for (const auto& fence : inFlightFences_) {
    device_.destroyFence(fence);
  }
  inFlightFences_.clear();

  commandBuffers_.clear();

  for (const auto& framebuffer : framebuffers_) {
    device_.destroyFramebuffer(framebuffer);
  }
  framebuffers_.clear();

  for (const auto& view : swapchainImageViews_) {
    device_.destroyImageView(view);
  }
  swapchainImageViews_.clear();
  swapchainImages_.clear();

  device_.destroySwapchainKHR(swapchain_);
  swapchain_ = nullptr;
}

auto VulkanAPI::recordCommandBuffer(vk::CommandBuffer commandBuffer,
                                    std::uint32_t imageIndex) -> void {
  vk::CommandBufferBeginInfo beginInfo;
  commandBuffer.begin(beginInfo);

  vk::ClearValue clearValue;
  vk::ClearColorValue color;
  color.setFloat32({clearColor_.r, clearColor_.g, clearColor_.b, clearColor_.a});
  clearValue.setColor(color);

  vk::RenderPassBeginInfo renderPassInfo;
  renderPassInfo.setRenderPass(renderPass_);
  renderPassInfo.setFramebuffer(framebuffers_[imageIndex]);
  renderPassInfo.setRenderArea(vk::Rect2D{{0, 0}, swapchainExtent_});
  renderPassInfo.setClearValues(clearValue);

  commandBuffer.beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);
  commandBuffer.endRenderPass();
  commandBuffer.end();
}

}  // namespace EGP
