#pragma once

#include <cstdint>

#include <EGP/rhi.hpp>
#include <vulkan/vulkan.hpp>

namespace EGP {

class VulkanAPI : public RHI::GraphicsAPI {
 public:
  explicit VulkanAPI(void* nativeWindowHandle);
  ~VulkanAPI() override;

  VulkanAPI(const VulkanAPI&) = delete;
  VulkanAPI& operator=(const VulkanAPI&) = delete;

  auto init(void* nativeWindowHandle) -> void override;
  auto clear(const glm::vec4& color, float depth) -> void override;

 private:
  void createInstance();

  void* nativeWindowHandle_ = nullptr;
  vk::Instance instance_;
  vk::DebugUtilsMessengerEXT debugMessenger_;
};

}  // namespace EGP
