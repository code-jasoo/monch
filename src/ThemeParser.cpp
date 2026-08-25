#include "ThemeParser.h"
#include <QDebug>
#include <QFile>
#include <QFileSystemWatcher>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <cstdlib>

ThemeParser::ThemeParser() {
    const QString HOME_DIR = std::getenv("HOME");
    themePath = HOME_DIR + "/.config/matugen/colors.json";

    fileWatcher.addPath(themePath);
    connect(&fileWatcher, &QFileSystemWatcher::fileChanged, this, &ThemeParser::reloadTheme);
    reloadTheme();
}

void ThemeParser::reloadTheme() {
    QFile file(themePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Unable to read file.";
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (!doc.isObject()) {
        return;
    }

    QJsonObject themeData = doc.object();

    m_surface = themeData["surface"].toString();
    m_surface_container = themeData["surface_container"].toString();
    m_surface_container_lowest = themeData["surface_container_lowest"].toString();
    m_surface_container_low = themeData["surface_container_low"].toString();
    m_surface_container_high = themeData["surface_container_high"].toString();
    m_surface_dim = themeData["surface_dim"].toString();
    m_on_surface = themeData["on_surface"].toString();
    m_on_surface_variant = themeData["on_surface_variant"].toString();
    m_outline = themeData["outline"].toString();
    m_outline_variant = themeData["outline_variant"].toString();
    m_primary = themeData["primary"].toString();
    m_secondary = themeData["secondary"].toString();
    m_tertiary = themeData["tertiary"].toString();
    m_error = themeData["error"].toString();
    m_surface_container_highest = themeData["surface_container_highest"].toString();

    emit themeChanged();
}
