if(WIN32)
    add_executable(${PLATFORM_PROJECT_NAME}
        "src/main.cpp"
    )

    if(ENABLE_CLANG_TIDY AND CLANG_TIDY)
        set_target_properties(${PLATFORM_PROJECT_NAME} PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY};--extra-arg=/EHsc")
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
    target_link_libraries(${PLATFORM_PROJECT_NAME} PRIVATE ${LIBRARY_PROJECT_NAME} libdesktop::libdesktop)

    include(DesktopHelpers)
    desktop_generate_translations(TARGET ${PLATFORM_PROJECT_NAME}
        SHORT_NAME ${SHORT_NAME}
        OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
endif()
