#pragma once
#include "Window.hpp"

namespace EGP::window {
  class SFMLWindow : public impl::window::WindowBase<SFMLWindow> {
  public:
    void createImpl();
    void destroyImpl();
    void pollEventsImpl();
    bool shouldCloseImpl() const;
    auto getNativeWindowHandleImpl() const;
  };

}
