#include "EGP/GLFW.hpp"

#include <cstdint>
#include <utility>
#include <vector>

namespace EGP {

auto GLFWWindow::getInstance(int width, int height, std::string title)
    -> GLFWWindow& {
  if (width <= 0 || height <= 0) {
    throw std::invalid_argument("Width and height must be positive integers");
  }
  static GLFWWindow instance(width, height, title);
  return instance;
}

GLFWWindow::GLFWWindow(int width, int height, std::string title) {
  if (!glfwInit()) {
    throw std::runtime_error("Failed to initialize GLFW");
  };
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  window_ = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
}

GLFWWindow::~GLFWWindow() {
  glfwDestroyWindow(window_);
  glfwTerminate();
}

auto GLFWWindow::pollEvents() -> void { glfwPollEvents(); }

auto GLFWWindow::shouldClose() const -> bool {
  return glfwWindowShouldClose(window_);
}

auto GLFWWindow::getProperties() const -> Window::WindowProperties {
  int width, height;
  glfwGetWindowSize(window_, &width, &height);
  auto title = glfwGetWindowTitle(window_);
  return {width, height, title};
}

auto GLFWWindow::getNativeWindowHandle() const -> void* {
  return static_cast<void*>(window_);
}

auto GLFWWindow::getFramebufferSize() const -> vk::Extent2D {
  int width = 0;
  int height = 0;
  glfwGetFramebufferSize(window_, &width, &height);
  return {static_cast<std::uint32_t>(width),
          static_cast<std::uint32_t>(height)};
}

auto GLFWWindow::getRequiredVulkanInstanceExtensions() const
    -> std::vector<const char*> {
  std::uint32_t count = 0;
  const char* const* names = glfwGetRequiredInstanceExtensions(&count);
  if (names == nullptr) {
    throw std::runtime_error("GLFW does not support Vulkan");
  }
  return {names, names + count};
}

auto GLFWWindow::createVulkanSurface(vk::Instance instance) const
    -> vk::SurfaceKHR {
  VkSurfaceKHR surface = nullptr;
  const auto result = static_cast<vk::Result>(glfwCreateWindowSurface(
      instance, window_, nullptr, &surface));
  if (result != vk::Result::eSuccess) {
    throw std::runtime_error("Failed to create window surface");
  }
  return vk::SurfaceKHR{surface};
}

auto GLFWWindow::setResizeCallback(ResizeCallback callback) -> void {
  resizeCallback_ = std::move(callback);
  glfwSetWindowUserPointer(window_, this);
  glfwSetFramebufferSizeCallback(
      window_, [](GLFWwindow* window, int width, int height) {
        auto* self = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));
        if (self != nullptr && self->resizeCallback_) {
          self->resizeCallback_(width, height);
        }
      });
}

}  // namespace EGP
