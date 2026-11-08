#pragma once

#include <glm/vec4.hpp>
#include <memory>

namespace EGP {

class Window;

class RHI {
 public:
  class GraphicsAPI {
   public:
    virtual ~GraphicsAPI() = default;

    virtual auto init(Window& window) -> void = 0;
    virtual auto clear(const glm::vec4& color, float depth) -> void = 0;
    virtual auto drawFrame() -> void = 0;
    virtual auto resize() -> void = 0;
    virtual auto onFramebufferResized() -> void = 0;
  };

  static auto create() -> std::unique_ptr<RHI>;

  ~RHI();
  RHI(const RHI&) = delete;
  RHI& operator=(const RHI&) = delete;

  auto init(Window& window) -> void;
  auto clear(const glm::vec4& color, float depth) -> void;
  auto drawFrame() -> void;
  auto resize() -> void;
  auto onFramebufferResized() -> void;

 private:
  RHI();
  std::unique_ptr<GraphicsAPI> api_ = nullptr;
};

}  // namespace EGP
