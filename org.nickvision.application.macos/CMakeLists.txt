if(APPLE)
    enable_language(OBJCXX)

    set(ICON_PNG "${CMAKE_SOURCE_DIR}/resources/${PROJECT_NAME}.macos.png")
    set(ICONSET_DIR "${CMAKE_CURRENT_BINARY_DIR}/AppIcon.iconset")
    set(ICNS_FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.icns")
    set(CMAKE_XCODE_ATTRIBUTE_ENABLE_HARDENED_RUNTIME YES)
    file(GLOB XIB_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/resources/*.xib")
    find_program(IBTOOL ibtool REQUIRED)

    add_custom_command(
        OUTPUT "${ICNS_FILE}"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${ICONSET_DIR}"
        COMMAND sips -z 16   16   "${ICON_PNG}" --out "${ICONSET_DIR}/icon_16x16.png"
        COMMAND sips -z 32   32   "${ICON_PNG}" --out "${ICONSET_DIR}/icon_16x16@2x.png"
        COMMAND sips -z 32   32   "${ICON_PNG}" --out "${ICONSET_DIR}/icon_32x32.png"
        COMMAND sips -z 64   64   "${ICON_PNG}" --out "${ICONSET_DIR}/icon_32x32@2x.png"
        COMMAND sips -z 128  128  "${ICON_PNG}" --out "${ICONSET_DIR}/icon_128x128.png"
        COMMAND sips -z 256  256  "${ICON_PNG}" --out "${ICONSET_DIR}/icon_128x128@2x.png"
        COMMAND sips -z 256  256  "${ICON_PNG}" --out "${ICONSET_DIR}/icon_256x256.png"
        COMMAND sips -z 512  512  "${ICON_PNG}" --out "${ICONSET_DIR}/icon_256x256@2x.png"
        COMMAND sips -z 512  512  "${ICON_PNG}" --out "${ICONSET_DIR}/icon_512x512.png"
        COMMAND sips -z 1024 1024 "${ICON_PNG}" --out "${ICONSET_DIR}/icon_512x512@2x.png"
        COMMAND iconutil -c icns "${ICONSET_DIR}" -o "${ICNS_FILE}"
        DEPENDS "${ICON_PNG}"
        COMMENT "Generating ${PROJECT_NAME}.icns"
        VERBATIM)

    set(NIB_FILES)
    foreach(XIB ${XIB_FILES})
        get_filename_component(XIB_NAME "${XIB}" NAME_WE)
        set(NIB_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${XIB_NAME}.nib")
        add_custom_command(
            OUTPUT "${NIB_OUTPUT}"
            COMMAND ${IBTOOL} --compile "${NIB_OUTPUT}" "${XIB}"
            DEPENDS "${XIB}"
            COMMENT "Compiling ${XIB_NAME}.xib"
            VERBATIM)
        list(APPEND NIB_FILES "${NIB_OUTPUT}")
    endforeach()

    set_source_files_properties(${ICNS_FILE} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    set_source_files_properties(${NIB_FILES} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

    add_executable(${PLATFORM_PROJECT_NAME} MACOSX_BUNDLE
        "${ICNS_FILE}"
        "${NIB_FILES}"
        "include/views/main_window.h"
        "include/views/settings_dialog.h"
        "include/app_delegate.h"
        "include/cocoa_lifetime_service.h"
        "src/views/main_window.mm"
        "src/views/settings_dialog.mm"
        "src/app_delegate.mm"
        "src/cocoa_lifetime_service.mm"
        "src/main.mm")

    if(ENABLE_CLANG_TIDY AND CLANG_TIDY)
        set_target_properties(${PLATFORM_PROJECT_NAME} PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY};--extra-arg=-fexceptions;--extra-arg=-Wno-elaborated-enum-base")
    endif()

    if(ENABLE_CLANG_FORMAT AND CLANG_FORMAT)
        file(GLOB_RECURSE FORMAT_SOURCE_FILES CONFIGURE_DEPENDS
            "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/**/*.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/*.mm"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.mm")
        add_custom_target(platform_format
            COMMAND ${CLANG_FORMAT} -i ${FORMAT_SOURCE_FILES}
            COMMENT "Running clang-format")
        add_dependencies(${PLATFORM_PROJECT_NAME} platform_format)
    endif()

    target_include_directories(${PLATFORM_PROJECT_NAME} PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>")

    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/resources/Info.plist"
        "${CMAKE_CURRENT_BINARY_DIR}/Info.plist"
        @ONLY)
    set_target_properties(${PLATFORM_PROJECT_NAME} PROPERTIES
        OUTPUT_NAME ${SHORT_NAME_WORD}
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_BINARY_DIR}/Info.plist")
    set_property(TARGET ${PLATFORM_PROJECT_NAME} APPEND PROPERTY COMPATIBLE_INTERFACE_STRING "${PROJECT_VERSION}")

    target_compile_options(${PLATFORM_PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Wno-deprecated-declarations)

    find_package(libdesktop CONFIG REQUIRED)
    target_link_libraries(${PLATFORM_PROJECT_NAME} PRIVATE ${LIBRARY_PROJECT_NAME} libdesktop::libdesktop "-framework AppKit" "-framework Foundation")

    include(DesktopHelpers)
    desktop_generate_translations(TARGET ${PLATFORM_PROJECT_NAME}
        SHORT_NAME ${SHORT_NAME}
        OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

    add_custom_command(TARGET ${PLATFORM_PROJECT_NAME} POST_BUILD
        COMMENT "Signing app bundle..."
        COMMAND codesign --force --deep --sign - "$<TARGET_BUNDLE_DIR:${PLATFORM_PROJECT_NAME}>"
        VERBATIM)
endif()
