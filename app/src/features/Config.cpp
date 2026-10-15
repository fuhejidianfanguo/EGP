#include "Config.hpp"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStandardPaths>
#include <QString>
#include <glaze/glaze.hpp>

namespace EGP {

auto Config::instance() -> Config& {
  static Config config;
  return config;
}

Config::Config() : path_(configPath()) {
  load();
}

auto Config::settings() const -> const Settings& {
  return settings_;
}

auto Config::path() const -> const QString& {
  return path_;
}

auto Config::configPath() -> QString {
  const auto directory =
      QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
  return QDir(directory).filePath(QStringLiteral("settings.json"));
}

void Config::load() {
  const QFileInfo file(path_);
  if (!file.exists()) {
    const auto directory = file.absoluteDir();
    if (!directory.exists() && !QDir().mkpath(directory.absolutePath())) {
      qWarning().noquote()
          << "Unable to create configuration directory"
          << directory.absolutePath();
      return;
    }

    std::string buffer;
    const auto error = glz::write_json(settings_, buffer);
    if (error) {
      qWarning().noquote()
          << "Unable to serialize default configuration" << path_ << ":"
          << QString::fromStdString(
                 glz::format_error(error, path_.toStdString()));
      return;
    }

    QFile file(path_);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate) ||
        file.write(QByteArray::fromStdString(buffer)) !=
            static_cast<qint64>(buffer.size())) {
      qWarning().noquote() << "Unable to write default configuration"
                           << path_ << ":" << file.errorString();
    }
    return;
  }
  std::string buffer;
  const auto error = glz::read_file_json(settings_, path_.toStdString(), buffer);
  if (error) {
    qWarning().noquote() << "Unable to read configuration file" << path_ << ":"
                         << QString::fromStdString(
                                glz::format_error(error, path_.toStdString()));
  }
}

}  // namespace EGP
