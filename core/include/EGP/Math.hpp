#pragma once
#include <cmath>
#include <cstdint>

namespace EGP::math {
  template <typename T>
  struct Vector2 {
    T x;
    T y;

    Vector2 operator+(const Vector2& other) const {
      return {x + other.x, y + other.y};
    }

    Vector2 operator-(const Vector2& other) const {
      return {x - other.x, y - other.y};
    }

    Vector2 operator*(T scalar) const {
      return {x * scalar, y * scalar};
    }

    Vector2 operator/(T scalar) const {
      return {x / scalar, y / scalar};
    }

    float length() const {
      return std::sqrt(static_cast<float>(x * x + y * y));
    }

    template <typename U = T>
    std::enable_if_t<std::is_floating_point_v<U>, Vector2> normalized() const {
      float len = length();
      if (len == 0) return {0, 0};
      return {x / len, y / len};
    }
  };

  using WindowSize = Vector2<uint32_t>;
}
