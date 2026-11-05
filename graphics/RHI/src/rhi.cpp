#include <EGP/rhi.hpp>

#include <EGP/GraphicsApi/vulkan_impl.hpp>
#include <EGP/graphics_api.hpp>

namespace EGP {

auto RHI::create() -> std::unique_ptr<RHI> { return std::unique_ptr<RHI>(new RHI()); }

RHI::RHI() {
  if constexpr (apiKindFromString(EGP_GRAPHICS_API) == ApiKind::Vulkan) {
    api_ = std::make_unique<VulkanAPI>(nullptr);
  }
}

RHI::~RHI() = default;

auto RHI::init(void* nativeWindowHandle) -> void { api_->init(nativeWindowHandle); }

auto RHI::clear(const glm::vec4& color, float depth) -> void { api_->clear(color, depth); }

auto RHI::drawFrame() -> void { api_->drawFrame(); }

}  // namespace EGP
