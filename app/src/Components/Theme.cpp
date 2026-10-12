#include "Theme.hpp"

namespace EGP::Components {

Theme::Theme(QObject* parent) : QObject(parent) {}

auto Theme::primary() const -> QString { return primary_; }
auto Theme::secondary() const -> QString { return secondary_; }
auto Theme::tertiary() const -> QString { return tertiary_; }
auto Theme::background() const -> QString { return background_; }
auto Theme::text() const -> QString { return text_; }
auto Theme::surface() const -> QString { return surface_; }
auto Theme::surfacePressed() const -> QString { return surfacePressed_; }
auto Theme::border() const -> QString { return border_; }
auto Theme::sliderBorder() const -> QString { return sliderBorder_; }
auto Theme::track() const -> QString { return track_; }
auto Theme::dark() const -> QString { return dark_; }
auto Theme::danger() const -> QString { return danger_; }
auto Theme::sidebar() const -> QString { return sidebar_; }
auto Theme::statusBar() const -> QString { return statusBar_; }
auto Theme::controlPressed() const -> QString { return controlPressed_; }

void Theme::setPrimary(const QString& value) {
  if (primary_ == value) return;
  primary_ = value;
  primaryChanged();
}

void Theme::setSecondary(const QString& value) {
  if (secondary_ == value) return;
  secondary_ = value;
  secondaryChanged();
}

void Theme::setTertiary(const QString& value) {
  if (tertiary_ == value) return;
  tertiary_ = value;
  tertiaryChanged();
}

void Theme::setBackground(const QString& value) {
  if (background_ == value) return;
  background_ = value;
  backgroundChanged();
}

void Theme::setText(const QString& value) {
  if (text_ == value) return;
  text_ = value;
  textChanged();
}

void Theme::setSurface(const QString& value) {
  if (surface_ == value) return;
  surface_ = value;
  surfaceChanged();
}

void Theme::setSurfacePressed(const QString& value) {
  if (surfacePressed_ == value) return;
  surfacePressed_ = value;
  surfacePressedChanged();
}

void Theme::setBorder(const QString& value) {
  if (border_ == value) return;
  border_ = value;
  borderChanged();
}

void Theme::setSliderBorder(const QString& value) {
  if (sliderBorder_ == value) return;
  sliderBorder_ = value;
  sliderBorderChanged();
}

void Theme::setTrack(const QString& value) {
  if (track_ == value) return;
  track_ = value;
  trackChanged();
}

void Theme::setDark(const QString& value) {
  if (dark_ == value) return;
  dark_ = value;
  darkChanged();
}

void Theme::setDanger(const QString& value) {
  if (danger_ == value) return;
  danger_ = value;
  dangerChanged();
}

void Theme::setSidebar(const QString& value) {
  if (sidebar_ == value) return;
  sidebar_ = value;
  sidebarChanged();
}

void Theme::setStatusBar(const QString& value) {
  if (statusBar_ == value) return;
  statusBar_ = value;
  statusBarChanged();
}

void Theme::setControlPressed(const QString& value) {
  if (controlPressed_ == value) return;
  controlPressed_ = value;
  controlPressedChanged();
}

}  // namespace EGP::Components