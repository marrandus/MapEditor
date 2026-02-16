cmake_minimum_required(VERSION 3.16)
project(MapEditor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(MapEditor
    src/main.cpp
    src/CMainWindow.cpp
    src/CMainView.cpp
    src/CMap.cpp
    src/CMapPreferencesDialog.cpp
    src/CTilesetSettingsDialog.cpp
    resources/resources.qrc
)

target_include_directories(MapEditor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
set_target_properties(MapEditor PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)

target_link_libraries(MapEditor PRIVATE Qt6::Widgets)
