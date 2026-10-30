#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlDebuggingEnabler>
#include <QQuickStyle>

auto main(int argc, char* argv[]) -> int {
  QGuiApplication app(argc, argv);
  QQuickStyle::setStyle("Basic");
  QQmlApplicationEngine engine;
  engine.loadFromModule("EGP.ui", "Main");

  return app.exec();
}