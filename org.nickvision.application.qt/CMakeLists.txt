set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
qt_standard_project_setup()

qt_add_executable(org.nickvision.application.qt
    "include/controls/aboutdialog.h"
    "include/views/mainwindow.h"
    "include/views/settingsdialog.h"
    "include/application.h"
    "src/controls/aboutdialog.ui"
    "src/controls/aboutdialog.cpp"
    "src/views/mainwindow.ui"
    "src/views/mainwindow.cpp"
    "src/views/settingsdialog.ui"
    "src/views/settingsdialog.cpp"
    "src/application.cpp"
    "src/main.cpp"
    "org.nickvision.application.qt.qrc"
    "org.nickvision.application.qt.rc")
target_include_directories(org.nickvision.application.qt PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(org.nickvision.application.qt PRIVATE libapplication Qt6::Core Qt6::Gui Qt6::Widgets)
set_target_properties(org.nickvision.application.qt PROPERTIES
    WIN32_EXECUTABLE ON)

add_custom_target(qt_commands ALL DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/__qt.h")
add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/__qt.h" "${CMAKE_CURRENT_BINARY_DIR}/qt.h" COMMAND "${CMAKE_COMMAND}" -E touch "${CMAKE_CURRENT_BINARY_DIR}/qt.h")
add_custom_command(TARGET qt_commands POST_BUILD COMMAND Qt6::windeployqt ARGS $<TARGET_FILE:org.nickvision.application.qt>)
if(HAS_GETTEXT)
    foreach(LINGUA IN ITEMS ${LINGUAS})
        add_custom_command(TARGET qt_commands PRE_BUILD COMMAND "${CMAKE_COMMAND}" -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/${LINGUA}")
        add_custom_command(TARGET qt_commands PRE_BUILD COMMAND "${GETTEXT_MSGFMT_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/resources/po/${LINGUA}.po" --output-file="${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/${LINGUA}/${SHORT_NAME}.mo")
    endforeach()
endif()