#include "Theme.hpp"

#include "Config.hpp"

namespace EGP::Components {

Theme::Theme(QObject* parent)
    : QObject(parent), theme_(EGP::Config::instance().settings().theme) {}

auto Theme::primary() const -> QString {
  return QString::fromStdString(theme_.primary);
}
auto Theme::secondary() const -> QString {
  return QString::fromStdString(theme_.secondary);
}
auto Theme::tertiary() const -> QString {
  return QString::fromStdString(theme_.tertiary);
}
auto Theme::background() const -> QString {
  return QString::fromStdString(theme_.background);
}
auto Theme::text() const -> QString {
  return QString::fromStdString(theme_.text);
}
auto Theme::surface() const -> QString {
  return QString::fromStdString(theme_.surface);
}
auto Theme::surfacePressed() const -> QString {
  return QString::fromStdString(theme_.surfacePressed);
}
auto Theme::border() const -> QString {
  return QString::fromStdString(theme_.border);
}
auto Theme::sliderBorder() const -> QString {
  return QString::fromStdString(theme_.sliderBorder);
}
auto Theme::track() const -> QString {
  return QString::fromStdString(theme_.track);
}
auto Theme::dark() const -> QString {
  return QString::fromStdString(theme_.dark);
}
auto Theme::danger() const -> QString {
  return QString::fromStdString(theme_.danger);
}
auto Theme::sidebar() const -> QString {
  return QString::fromStdString(theme_.sidebar);
}
auto Theme::statusBar() const -> QString {
  return QString::fromStdString(theme_.statusBar);
}
auto Theme::controlPressed() const -> QString {
  return QString::fromStdString(theme_.controlPressed);
}

void Theme::setPrimary(const QString& value) {
  if (primary() == value) return;
  theme_.primary = value.toStdString();
  primaryChanged();
}

void Theme::setSecondary(const QString& value) {
  if (secondary() == value) return;
  theme_.secondary = value.toStdString();
  secondaryChanged();
}

void Theme::setTertiary(const QString& value) {
  if (tertiary() == value) return;
  theme_.tertiary = value.toStdString();
  tertiaryChanged();
}

void Theme::setBackground(const QString& value) {
  if (background() == value) return;
  theme_.background = value.toStdString();
  backgroundChanged();
}

void Theme::setText(const QString& value) {
  if (text() == value) return;
  theme_.text = value.toStdString();
  textChanged();
}

void Theme::setSurface(const QString& value) {
  if (surface() == value) return;
  theme_.surface = value.toStdString();
  surfaceChanged();
}

void Theme::setSurfacePressed(const QString& value) {
  if (surfacePressed() == value) return;
  theme_.surfacePressed = value.toStdString();
  surfacePressedChanged();
}

void Theme::setBorder(const QString& value) {
  if (border() == value) return;
  theme_.border = value.toStdString();
  borderChanged();
}

void Theme::setSliderBorder(const QString& value) {
  if (sliderBorder() == value) return;
  theme_.sliderBorder = value.toStdString();
  sliderBorderChanged();
}

void Theme::setTrack(const QString& value) {
  if (track() == value) return;
  theme_.track = value.toStdString();
  trackChanged();
}

void Theme::setDark(const QString& value) {
  if (dark() == value) return;
  theme_.dark = value.toStdString();
  darkChanged();
}

void Theme::setDanger(const QString& value) {
  if (danger() == value) return;
  theme_.danger = value.toStdString();
  dangerChanged();
}

void Theme::setSidebar(const QString& value) {
  if (sidebar() == value) return;
  theme_.sidebar = value.toStdString();
  sidebarChanged();
}

void Theme::setStatusBar(const QString& value) {
  if (statusBar() == value) return;
  theme_.statusBar = value.toStdString();
  statusBarChanged();
}

void Theme::setControlPressed(const QString& value) {
  if (controlPressed() == value) return;
  theme_.controlPressed = value.toStdString();
  controlPressedChanged();
}

}  // namespace EGP::Components