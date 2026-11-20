#pragma once
#include <vector>
namespace EGP {
class Object {
  std::vector<Object> children;

 public:
  virtual ~Object() = default;
};
}  // namespace EGP
