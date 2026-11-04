#pragma once

#include <memory>

#include <glm/vec4.hpp>

namespace EGP {

class RHI {
 public:
  class GraphicsAPI {
   public:
    virtual ~GraphicsAPI() = default;

    virtual auto init(void* nativeWindowHandle) -> void = 0;
    virtual auto clear(const glm::vec4& color, float depth) -> void = 0;
  };

  static auto create() -> std::unique_ptr<RHI>;

  ~RHI();
  RHI(const RHI&) = delete;
  RHI& operator=(const RHI&) = delete;

  auto init(void* nativeWindowHandle) -> void;
  auto clear(const glm::vec4& color, float depth) -> void;

 private:
  RHI();
  std::unique_ptr<GraphicsAPI> api_ = nullptr;
};

}  // namespace EGP
