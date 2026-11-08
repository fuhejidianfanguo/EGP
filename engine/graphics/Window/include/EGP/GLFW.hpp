#pragma once
#include <exception>
#include <stdexcept>
#include <string>

#include "EGP/Window.hpp"
#include "GLFW/glfw3.h"

namespace EGP {
class GLFWWindow : public Window {
 public:
  static auto getInstance(int width = 800, int height = 600,
                          std::string title = "EGP") -> GLFWWindow&;
  ~GLFWWindow() override;
  GLFWWindow(const GLFWWindow&) = delete;
  GLFWWindow& operator=(const GLFWWindow&) = delete;

  auto pollEvents() -> void override;
  auto shouldClose() const -> bool override;
  auto getProperties() const -> WindowProperties override;
  auto getNativeWindowHandle() const -> void* override;
  auto setResizeCallback(ResizeCallback callback) -> void override;

 private:
  GLFWWindow(int width, int height, std::string title);
  std::string title_;
  GLFWwindow* window_;
  ResizeCallback resizeCallback_;
};
}  // namespace EGP
