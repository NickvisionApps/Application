add_library(${LIBRARY_PROJECT_NAME} STATIC
    "include/controllers/main_window_controller.h"
    "include/controllers/preferences_view_controller.h"
    "include/events/folder_changed_event_args.h"
    "include/helpers/host.h"
    "include/models/theme.h"
    "include/services/app_config_service.h"
    "include/services/events_service.h"
    "include/services/folder_service.h"
    "src/controllers/main_window_controller.cpp"
    "src/controllers/preferences_view_controller.cpp"
    "src/events/folder_changed_event_args.cpp"
    "src/helpers/host.cpp"
    "src/services/app_config_service.cpp"
    "src/services/events_service.cpp"
    "src/services/folder_service.cpp"
)

if(ENABLE_CLANG_TIDY AND CLANG_TIDY)
    if(MSVC)
        set_target_properties(${LIBRARY_PROJECT_NAME} PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY};--extra-arg=/EHsc")
    elseif(LINUX)
        set_target_properties(${LIBRARY_PROJECT_NAME} PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY};--extra-arg=-fexceptions;--extra-arg=-std=c++20")
    else()
        set_target_properties(${LIBRARY_PROJECT_NAME} PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY};--extra-arg=-fexceptions")
    endif()
endif()

if(ENABLE_CLANG_FORMAT AND CLANG_FORMAT)
    file(GLOB_RECURSE FORMAT_SOURCE_FILES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/**/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.cpp")
    add_custom_target(library_format
        COMMAND ${CLANG_FORMAT} -i ${FORMAT_SOURCE_FILES}
        COMMENT "Running clang-format")
    add_dependencies(${LIBRARY_PROJECT_NAME} library_format)
endif()

target_include_directories(${LIBRARY_PROJECT_NAME} PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>")

set_target_properties(${LIBRARY_PROJECT_NAME} PROPERTIES
    VERSION "${PROJECT_VERSION}")
set_property(TARGET ${LIBRARY_PROJECT_NAME} APPEND PROPERTY COMPATIBLE_INTERFACE_STRING "${PROJECT_VERSION}")

if(MSVC)
    target_compile_options(${LIBRARY_PROJECT_NAME} PRIVATE /W4)
    target_compile_definitions(${LIBRARY_PROJECT_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)
else()
    target_compile_options(${LIBRARY_PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

find_package(libdesktop CONFIG REQUIRED)
target_link_libraries(${LIBRARY_PROJECT_NAME} PRIVATE libdesktop::libdesktop)