#include <EGP/GraphicsApi/vulkan_impl.hpp>

#include <cstddef>

#include "vulkan_internal.hpp"

namespace EGP {

auto VulkanAPI::createCommandPool() -> void {
  vk::CommandPoolCreateInfo createInfo;
  createInfo.setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer);
  createInfo.setQueueFamilyIndex(graphicsQueueFamily_);
  commandPool_ = device_.createCommandPool(createInfo);
}

auto VulkanAPI::createCommandBuffers() -> void {
  commandBuffers_.resize(framebuffers_.size());
  vk::CommandBufferAllocateInfo allocateInfo;
  allocateInfo.setCommandPool(commandPool_);
  allocateInfo.setLevel(vk::CommandBufferLevel::ePrimary);
  allocateInfo.setCommandBufferCount(
      static_cast<std::uint32_t>(commandBuffers_.size()));
  commandBuffers_ = device_.allocateCommandBuffers(allocateInfo);
}

auto VulkanAPI::createSyncObjects() -> void {
  const auto frameCount = static_cast<std::size_t>(detail::kMaxFramesInFlight);
  vk::SemaphoreCreateInfo semaphoreInfo;
  vk::FenceCreateInfo fenceInfo;
  fenceInfo.setFlags(vk::FenceCreateFlagBits::eSignaled);

  imageAvailableSemaphores_.resize(frameCount);
  inFlightFences_.resize(frameCount);
  for (std::size_t i = 0; i < frameCount; ++i) {
    imageAvailableSemaphores_[i] = device_.createSemaphore(semaphoreInfo);
    inFlightFences_[i] = device_.createFence(fenceInfo);
  }

  renderFinishedSemaphores_.resize(swapchainImages_.size());
  for (auto& semaphore : renderFinishedSemaphores_) {
    semaphore = device_.createSemaphore(semaphoreInfo);
  }
}

}  // namespace EGP
