#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

#include <vulkan/vulkan.hpp>

namespace EGP {
class Window {
 public:
  struct WindowProperties {
    int width;
    int height;
    std::string title;
  };

  using ResizeCallback = std::function<void(int width, int height)>;

  virtual ~Window() = default;

  virtual auto pollEvents() -> void = 0;
  virtual auto shouldClose() const -> bool = 0;
  virtual auto getProperties() const -> WindowProperties = 0;
  virtual auto getNativeWindowHandle() const -> void* = 0;
  virtual auto getFramebufferSize() const -> vk::Extent2D = 0;
  virtual auto getRequiredVulkanInstanceExtensions() const
      -> std::vector<const char*> = 0;
  virtual auto createVulkanSurface(vk::Instance instance) const
      -> vk::SurfaceKHR = 0;
  virtual auto setResizeCallback(ResizeCallback callback) -> void = 0;
};
}  // namespace EGP
