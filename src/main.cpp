#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTimer>

#include "MonitorModel.h"
#include "ThemeParser.h"

int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    ThemeParser themeParser;
    MonitorModel monitorModel;
    QQmlContext* rootContext = engine.rootContext();
    rootContext->setContextProperty("Theme", &themeParser);
    rootContext->setContextProperty("Monitors", &monitorModel);
    engine.loadFromModule("monched", "Main");

    // Load monitors beforehand
    QTimer::singleShot(0, &monitorModel, &MonitorModel::reloadMonitors);

    return app.exec();
}
