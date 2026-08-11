include(FetchContent) # Grab external libraries

cmake_minimum_required(VERSION 3.16)

project (monch VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 REQUIRED COMPONENTS Qml Quick Gui)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)

qt_standard_project_setup()

qt_add_executable(monch
  src/main.cpp
  src/ThemeParser.cpp
  src/ThemeParser.h
  src/Monitor.cpp
  src/Monitor.h
  src/MonitorModel.cpp
  src/MonitorModel.h
  src/IPC.cpp
  src/IPC.h
)

qt_add_qml_module(monch
  URI monched
  QML_FILES
  ui/Main.qml
)

target_link_libraries(monch PRIVATE
  Qt6::Qml
  Qt6::Gui
  Qt6::Quick
  nlohmann_json::nlohmann_json
)
