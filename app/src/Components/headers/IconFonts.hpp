#pragma once

#include <QtQmlIntegration/qqmlintegration.h>

#include <QObject>
#include <QString>


namespace EGP::Components {

class IconFonts final : public QObject {
  Q_OBJECT
  QML_ELEMENT
  QML_SINGLETON
  Q_PROPERTY(QString solid READ solid CONSTANT)
  Q_PROPERTY(QString regular READ regular CONSTANT)
  Q_PROPERTY(QString brands READ brands CONSTANT)
  Q_PROPERTY(QString solidStyle READ solidStyle CONSTANT)
  Q_PROPERTY(QString regularStyle READ regularStyle CONSTANT)
  Q_PROPERTY(QString brandsStyle READ brandsStyle CONSTANT)

 public:
  explicit IconFonts(QObject* parent = nullptr);

  [[nodiscard]] auto solid() const -> QString;
  [[nodiscard]] auto regular() const -> QString;
  [[nodiscard]] auto brands() const -> QString;
  [[nodiscard]] auto solidStyle() const -> QString;
  [[nodiscard]] auto regularStyle() const -> QString;
  [[nodiscard]] auto brandsStyle() const -> QString;

 private:
  QString solid_;
  QString regular_;
  QString brands_;
};
}  // namespace EGP::Components