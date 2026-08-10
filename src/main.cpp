#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "ThemeParser.h"

int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    ThemeParser themeParser;
    QQmlContext* rootContext = engine.rootContext();
    rootContext->setContextProperty("Theme", &themeParser);

    engine.loadFromModule("monched", "Main");

    return app.exec();
}
