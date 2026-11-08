#include <EGP/GraphicsApi/vulkan_impl.hpp>
#include <algorithm>
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
  vk::CommandBufferAllocateInfo allocateInfo;
  allocateInfo.setCommandPool(commandPool_);
  allocateInfo.setLevel(vk::CommandBufferLevel::ePrimary);
  allocateInfo.setCommandBufferCount(
      static_cast<std::uint32_t>(swapchainImages_.size()));
  commandBuffers_ = device_.allocateCommandBuffers(allocateInfo);
}

auto VulkanAPI::createSyncObjects() -> void {
  constexpr auto frameCount = detail::kMaxFramesInFlight;
  const vk::SemaphoreCreateInfo semaphoreInfo{};
  vk::FenceCreateInfo fenceInfo;
  fenceInfo.setFlags(vk::FenceCreateFlagBits::eSignaled);

  imageAvailableSemaphores_.resize(frameCount);
  inFlightFences_.resize(frameCount);
  std::ranges::generate(imageAvailableSemaphores_,
                        [&] { return device_.createSemaphore(semaphoreInfo); });
  std::ranges::generate(inFlightFences_,
                        [&] { return device_.createFence(fenceInfo); });

  renderFinishedSemaphores_.resize(swapchainImages_.size());
  std::ranges::generate(renderFinishedSemaphores_,
                        [&] { return device_.createSemaphore(semaphoreInfo); });
}

}  // namespace EGP
