#pragma once

#include <EGP/settings_structures.hpp>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStandardPaths>
#include <QString>
#include <glaze/glaze.hpp>

namespace EGP {
class Config final {
 public:
  static auto instance() -> Config&;

  Config(const Config&) = delete;
  Config(Config&&) = delete;
  auto operator=(const Config&) -> Config& = delete;
  auto operator=(Config&&) -> Config& = delete;

  [[nodiscard]] auto settings() const -> const Settings&;
  [[nodiscard]] auto path() const -> const QString&;

 private:
  Config();

  static auto configPath() -> QString;

  void load();

  Settings settings_;
  QString path_;
};
}  // namespace EGP