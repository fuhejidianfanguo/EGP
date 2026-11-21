#pragma once
#include <concepts>

namespace EGP::impl::window {
template <typename T>
concept WindowImpl = requires(T t) {
  { t.createImpl() } -> std::same_as<void>;
  { t.destroyImpl() } -> std::same_as<void>;
  { t.pollEventsImpl() } -> std::same_as<void>;
  { t.shouldCloseImpl() } -> std::same_as<bool>;
  t.getNativeWindowHandleImpl();
};

class IWindow {
 public:
  virtual ~IWindow() = default;

  virtual void create() = 0;
  virtual void destroy() = 0;
  virtual void pollEvents() = 0;
  virtual bool shouldClose() const = 0;
};

template <typename T>
class WindowBase : public IWindow {
 protected:
  void create() override { static_cast<T*>(this)->createImpl(); }
  void destroy() override { static_cast<T*>(this)->destroyImpl(); }
  void pollEvents() override { static_cast<T*>(this)->pollEventsImpl(); }
  bool shouldClose() const override {
    return static_cast<const T*>(this)->shouldCloseImpl();
  }
  auto getNativeWindowHandle() const {
    return static_cast<const T*>(this)->getNativeWindowHandleImpl();
  }

 private:
  static void checkImpl() {
    static_assert(WindowImpl<T>, "the type T must implement WindowImpl");
  }
};

}  // namespace EGP::impl::window
