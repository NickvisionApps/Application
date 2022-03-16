cmake_minimum_required(VERSION 3.5)

project(NickvisionApplication LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(UPDATE update/version.h update/version.cpp update/updateconfig.h update/updateconfig.cpp update/updater.h update/updater.cpp)
set(MODELS models/theme.h models/configuration.h models/configuration.cpp)
set(UI ui/application.h ui/application.cpp ui/progressdialog.xml ui/mainwindow.xml ui/menuHelp.xml ui/preferencesdialog.xml ui/xmlstrings.h ui/xmlstrings.cpp)
set(CONTROLS ui/controls/progressdialog.h ui/controls/progressdialog.cpp)
set(VIEWS ui/views/mainwindow.h ui/views/mainwindow.cpp ui/views/preferencesdialog.h ui/views/preferencesdialog.cpp)

add_executable(NickvisionApplication main.cpp ${UPDATE} ${MODELS} ${UI} ${CONTROLS} ${VIEWS})

target_compile_options(NickvisionApplication PRIVATE -fpie)

find_package(PkgConfig)
pkg_check_modules(GTK REQUIRED IMPORTED_TARGET gtk4)
pkg_check_modules(ADWAITA REQUIRED IMPORTED_TARGET libadwaita-1)
pkg_check_modules(JSONCPP REQUIRED IMPORTED_TARGET jsoncpp)
pkg_check_modules(CURLPP REQUIRED curlpp)

target_link_libraries(NickvisionApplication PRIVATE PkgConfig::GTK)
target_link_libraries(NickvisionApplication PRIVATE PkgConfig::ADWAITA)
target_link_libraries(NickvisionApplication PRIVATE PkgConfig::JSONCPP)
target_link_libraries(NickvisionApplication PRIVATE ${CURLPP_LDFLAGS})
