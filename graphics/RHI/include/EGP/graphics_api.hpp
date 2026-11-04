#pragma once

#include <string_view>

namespace EGP {

enum class ApiKind {
  Vulkan,
};

consteval auto apiKindFromString(std::string_view name) -> ApiKind {
  if (name == "Vulkan") {
    return ApiKind::Vulkan;
  }
  throw "Unsupported graphics API";
}

}  // namespace EGP
