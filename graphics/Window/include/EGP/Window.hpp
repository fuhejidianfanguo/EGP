#pragma once

#include <cstdint>
#include <functional>
#include <string>

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
  virtual auto setResizeCallback(ResizeCallback callback) -> void = 0;
};
}  // namespace EGP
