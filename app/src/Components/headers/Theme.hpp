#pragma once

#include <EGP/settings_structures.hpp>
#include <QtQmlIntegration/qqmlintegration.h>

#include <QObject>
#include <QString>

namespace EGP::Components {
class Theme final : public QObject {
  Q_OBJECT
  QML_ELEMENT
  QML_SINGLETON
  Q_PROPERTY(QString primary READ primary WRITE setPrimary NOTIFY primaryChanged)
  Q_PROPERTY(QString secondary READ secondary WRITE setSecondary NOTIFY secondaryChanged)
  Q_PROPERTY(QString tertiary READ tertiary WRITE setTertiary NOTIFY tertiaryChanged)
  Q_PROPERTY(QString background READ background WRITE setBackground NOTIFY backgroundChanged)
  Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
  Q_PROPERTY(QString surface READ surface WRITE setSurface NOTIFY surfaceChanged)
  Q_PROPERTY(QString surfacePressed READ surfacePressed WRITE setSurfacePressed NOTIFY surfacePressedChanged)
  Q_PROPERTY(QString border READ border WRITE setBorder NOTIFY borderChanged)
  Q_PROPERTY(QString sliderBorder READ sliderBorder WRITE setSliderBorder NOTIFY sliderBorderChanged)
  Q_PROPERTY(QString track READ track WRITE setTrack NOTIFY trackChanged)
  Q_PROPERTY(QString dark READ dark WRITE setDark NOTIFY darkChanged)
  Q_PROPERTY(QString danger READ danger WRITE setDanger NOTIFY dangerChanged)
  Q_PROPERTY(QString sidebar READ sidebar WRITE setSidebar NOTIFY sidebarChanged)
  Q_PROPERTY(QString statusBar READ statusBar WRITE setStatusBar NOTIFY statusBarChanged)
  Q_PROPERTY(QString controlPressed READ controlPressed WRITE setControlPressed NOTIFY controlPressedChanged)

 public:
  explicit Theme(QObject* parent = nullptr);

  [[nodiscard]] auto primary() const -> QString;
  [[nodiscard]] auto secondary() const -> QString;
  [[nodiscard]] auto tertiary() const -> QString;
  [[nodiscard]] auto background() const -> QString;
  [[nodiscard]] auto text() const -> QString;
  [[nodiscard]] auto surface() const -> QString;
  [[nodiscard]] auto surfacePressed() const -> QString;
  [[nodiscard]] auto border() const -> QString;
  [[nodiscard]] auto sliderBorder() const -> QString;
  [[nodiscard]] auto track() const -> QString;
  [[nodiscard]] auto dark() const -> QString;
  [[nodiscard]] auto danger() const -> QString;
  [[nodiscard]] auto sidebar() const -> QString;
  [[nodiscard]] auto statusBar() const -> QString;
  [[nodiscard]] auto controlPressed() const -> QString;
  void setPrimary(const QString& value);
  void setSecondary(const QString& value);
  void setTertiary(const QString& value);
  void setBackground(const QString& value);
  void setText(const QString& value);
  void setSurface(const QString& value);
  void setSurfacePressed(const QString& value);
  void setBorder(const QString& value);
  void setSliderBorder(const QString& value);
  void setTrack(const QString& value);
  void setDark(const QString& value);
  void setDanger(const QString& value);
  void setSidebar(const QString& value);
  void setStatusBar(const QString& value);
  void setControlPressed(const QString& value);

 signals:
  void primaryChanged();
  void secondaryChanged();
  void tertiaryChanged();
  void backgroundChanged();
  void textChanged();
  void surfaceChanged();
  void surfacePressedChanged();
  void borderChanged();
  void sliderBorderChanged();
  void trackChanged();
  void darkChanged();
  void dangerChanged();
  void sidebarChanged();
  void statusBarChanged();
  void controlPressedChanged();

 private:
  Settings::Theme theme_;
};
}  // namespace EGP::Components