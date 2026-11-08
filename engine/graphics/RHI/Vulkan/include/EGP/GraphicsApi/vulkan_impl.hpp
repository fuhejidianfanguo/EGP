#pragma once

#include <EGP/rhi.hpp>
#include <cstdint>
#include <vector>
#include <vulkan/vulkan.hpp>

namespace EGP {

class Window;

class VulkanAPI : public RHI::GraphicsAPI {
 public:
  VulkanAPI() = default;
  ~VulkanAPI() override;

  VulkanAPI(const VulkanAPI&) = delete;
  VulkanAPI& operator=(const VulkanAPI&) = delete;

  auto init(Window& window) -> void override;
  auto clear(const glm::vec4& color, float depth) -> void override;
  auto drawFrame() -> void override;
  auto resize() -> void override;
  auto onFramebufferResized() -> void override;

 private:
  void destroy();
  void createInstance();
  void createDebugMessenger();
  void createSurface();
  void pickPhysicalDevice();
  void createDevice();
  void createSwapchain();
  void createImageViews();
  void destroySwapchainResources();

  void createCommandPool();
  void createCommandBuffers();
  void createSyncObjects();
  void recordCommandBuffer(vk::CommandBuffer commandBuffer,
                           std::uint32_t imageIndex);

  Window* window_ = nullptr;
  vk::Instance instance_;
  vk::DebugUtilsMessengerEXT debugMessenger_;
  vk::SurfaceKHR surface_;
  vk::PhysicalDevice physicalDevice_;
  vk::Device device_;
  vk::Queue graphicsQueue_;
  vk::Queue presentQueue_;
  std::uint32_t graphicsQueueFamily_ = 0;
  std::uint32_t presentQueueFamily_ = 0;
  vk::SwapchainKHR swapchain_;
  vk::Format swapchainFormat_ = vk::Format::eUndefined;
  vk::Extent2D swapchainExtent_{};
  std::vector<vk::Image> swapchainImages_;
  std::vector<vk::ImageView> swapchainImageViews_;
  bool framebufferResized_ = false;

  vk::CommandPool commandPool_;
  std::vector<vk::CommandBuffer> commandBuffers_;
  std::vector<vk::Semaphore> imageAvailableSemaphores_;
  std::vector<vk::Semaphore> renderFinishedSemaphores_;
  std::vector<vk::Fence> inFlightFences_;
  glm::vec4 clearColor_{0.0F, 0.0F, 0.0F, 1.0F};
  std::uint32_t currentFrame_ = 0;
};

}  // namespace EGP
