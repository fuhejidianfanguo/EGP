#include "headers/IconFonts.hpp"

#include <QDebug>
#include <QFontDatabase>

namespace {

auto loadFont(const QString& resourcePath) -> QString {
  const auto fontId = QFontDatabase::addApplicationFont(resourcePath);
  if (fontId < 0) {
    qWarning() << "Failed to load application font:" << resourcePath;
    return {};
  }

  const auto families = QFontDatabase::applicationFontFamilies(fontId);
  if (families.isEmpty()) {
    qWarning() << "Application font has no family:" << resourcePath;
    return {};
  }

  return families.constFirst();
}

}  // namespace

EGP::Components::IconFonts::IconFonts(QObject* parent)
    : QObject(parent),
      solid_(loadFont(":/resources/fonts/Font-Awesome-7-Free-Solid-900.otf")),
      regular_(loadFont(":/resources/fonts/Font-Awesome-7-Free-Regular-400.otf")),
      brands_(loadFont(":/resources/fonts/Font-Awesome-7-Brands-Regular-400.otf")) {}

auto EGP::Components::IconFonts::solid() const -> QString {
  return solid_;
}

auto EGP::Components::IconFonts::regular() const -> QString {
  return regular_;
}

auto EGP::Components::IconFonts::brands() const -> QString {
  return brands_;
}

auto EGP::Components::IconFonts::solidStyle() const -> QString {
  return QStringLiteral("Solid");
}

auto EGP::Components::IconFonts::regularStyle() const -> QString {
  return QStringLiteral("Regular");
}

auto EGP::Components::IconFonts::brandsStyle() const -> QString {
  return QStringLiteral("Brands");
}