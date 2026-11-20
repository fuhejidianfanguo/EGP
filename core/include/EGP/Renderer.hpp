#pragma once

namespace EGP::Renderer {
  enum class Object {

  };

  /// 使用 CRTP 模式实现的渲染器基类。
  /// 实现方最好使用单例模式。
  class Renderer {
    void setup(this auto& self) {
      self.setupImpl();
    }
  };
}
