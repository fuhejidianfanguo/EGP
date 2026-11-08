#include <EGP/GLFW.hpp>
#include <EGP/rhi.hpp>
#include <iostream>

int main() {
  try {
    EGP::GLFWWindow& window = EGP::GLFWWindow::getInstance(800, 600, "EGP");
    auto rhi = EGP::RHI::create();
    rhi->init(window);
    rhi->clear({0.1F, 0.2F, 0.3F, 1.0F}, 1.0F);
    window.setResizeCallback([&rhi](int, int) { rhi->onFramebufferResized(); });
    while (!window.shouldClose()) {
      window.pollEvents();
      rhi->drawFrame();
    }
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
