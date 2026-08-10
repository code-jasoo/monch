#include "ThemeParser.h"
#include <QDebug>
#include <QFileSystemWatcher>
#include <cstdlib>
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>
using json = nlohmann::json;

ThemeParser::ThemeParser() {
    const std::string HOME_DIR = std::getenv("HOME");
    themePath = HOME_DIR + "/.config/matugen/colors.json";

    fileWatcher.addPath(QString::fromStdString(themePath));
    connect(&fileWatcher, &QFileSystemWatcher::fileChanged, this, &ThemeParser::reloadTheme);
    reloadTheme();
}

void ThemeParser::reloadTheme() {
    std::ifstream f(themePath);

    json themeData = json::parse(f);

    m_surface = QString::fromStdString(themeData["surface"]);
    m_surface_container = QString::fromStdString(themeData["surface_container"]);
    m_surface_container_lowest = QString::fromStdString(themeData["surface_container_lowest"]);
    m_surface_container_low = QString::fromStdString(themeData["surface_container_low"]);
    m_surface_container_high = QString::fromStdString(themeData["surface_container_high"]);
    m_surface_dim = QString::fromStdString(themeData["surface_dim"]);
    m_on_surface = QString::fromStdString(themeData["on_surface"]);
    m_on_surface_variant = QString::fromStdString(themeData["on_surface_variant"]);
    m_outline = QString::fromStdString(themeData["outline"]);
    m_outline_variant = QString::fromStdString(themeData["outline_variant"]);
    m_primary = QString::fromStdString(themeData["primary"]);
    m_secondary = QString::fromStdString(themeData["secondary"]);
    m_tertiary = QString::fromStdString(themeData["tertiary"]);
    m_error = QString::fromStdString(themeData["error"]);
    m_surface_container_highest = QString::fromStdString(themeData["surface_container_highest"]);

    emit themeChanged();
}
