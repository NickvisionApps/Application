if(LINUX)
    add_executable(${PLATFORM_PROJECT_NAME}
        "include/controls/shortcuts_dialog.h"
        "include/helpers/gtk.h"
        "include/helpers/ui_builder.h"
        "include/views/main_window.h"
        "include/views/preferences_dialog.h"
        "include/gtk_lifetime_service.h"
        "src/controls/shortcuts_dialog.cpp"
        "src/helpers/gtk.cpp"
        "src/helpers/ui_builder.cpp"
        "src/views/main_window.cpp"
        "src/views/preferences_dialog.cpp"
        "src/gtk_lifetime_service.cpp"
        "src/main.cpp"
    )

    if(ENABLE_CLANG_TIDY AND CLANG_TIDY)
        set_target_properties(${PLATFORM_PROJECT_NAME} PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY};--extra-arg=-fexceptions;--extra-arg=-std=c++20")
    endif()

    if(ENABLE_CLANG_FORMAT AND CLANG_FORMAT)
        file(GLOB_RECURSE FORMAT_SOURCE_FILES CONFIGURE_DEPENDS
            "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/**/*.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.cpp")
        add_custom_target(platform_format
            COMMAND ${CLANG_FORMAT} -i ${FORMAT_SOURCE_FILES}
            COMMENT "Running clang-format")
        add_dependencies(${PLATFORM_PROJECT_NAME} platform_format)
    endif()

    target_include_directories(${PLATFORM_PROJECT_NAME} PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>")

    set_target_properties(${PLATFORM_PROJECT_NAME} PROPERTIES
        VERSION "${PROJECT_VERSION}")
    set_property(TARGET ${PLATFORM_PROJECT_NAME} APPEND PROPERTY COMPATIBLE_INTERFACE_STRING "${PROJECT_VERSION}")

    if(MSVC)
        target_compile_options(${PLATFORM_PROJECT_NAME} PRIVATE /W4)
        target_compile_definitions(${PLATFORM_PROJECT_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)
    else()
        target_compile_options(${PLATFORM_PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
    endif()

    find_package(libdesktop CONFIG REQUIRED)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(gtk REQUIRED IMPORTED_TARGET gtk4)
    pkg_check_modules(adwaita REQUIRED IMPORTED_TARGET libadwaita-1)
    pkg_check_modules(libxmlpp REQUIRED IMPORTED_TARGET libxml++-5.0)
    target_link_libraries(${PLATFORM_PROJECT_NAME} PRIVATE ${LIBRARY_PROJECT_NAME} libdesktop::libdesktop PkgConfig::gtk PkgConfig::adwaita PkgConfig::libxmlpp)

    include(DesktopHelpers)
    desktop_generate_translations(TARGET ${PLATFORM_PROJECT_NAME}
        SHORT_NAME ${SHORT_NAME}
        OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
    desktop_compile_glib_resources(TARGET ${PLATFORM_PROJECT_NAME}
        RESOURCE_XML "${CMAKE_CURRENT_SOURCE_DIR}/resources/${PROJECT_NAME}.gresource.xml"
        OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.gresource")
    desktop_compile_blueprints(TARGET ${PLATFORM_PROJECT_NAME}
        OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/ui")
    desktop_linux_install(APP_ID ${PROJECT_NAME}
        ICON_DIR "${CMAKE_SOURCE_DIR}/resources")
endif()
