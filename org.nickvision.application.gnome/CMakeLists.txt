if(LINUX)
    add_executable(org.nickvision.application.gnome
        "src/helpers/builder.cpp"
        "src/views/mainwindow.cpp"
        "src/views/preferencesdialog.cpp"
        "src/application.cpp"
        "src/main.cpp")
    target_include_directories(org.nickvision.application.gnome PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(gtk REQUIRED IMPORTED_TARGET gtk4)
    pkg_check_modules(adwaita REQUIRED IMPORTED_TARGET libadwaita-1)
    pkg_check_modules(libxmlpp REQUIRED IMPORTED_TARGET libxml++-5.0)
    target_link_libraries(org.nickvision.application.gnome PRIVATE libapplication PkgConfig::gtk PkgConfig::adwaita PkgConfig::libxmlpp)

    find_program(GLIB_COMPULE_RESOURCES_EXECUTABLE glib-compile-resources)
    if(GLIB_COMPULE_RESOURCES_EXECUTABLE)
        add_custom_command(TARGET org.nickvision.application.gnome PRE_BUILD COMMAND "${GLIB_COMPULE_RESOURCES_EXECUTABLE}" --sourcedir "${CMAKE_CURRENT_SOURCE_DIR}/resources" "${CMAKE_CURRENT_SOURCE_DIR}/resources/org.nickvision.application.gresource.xml" --target="${CMAKE_CURRENT_BINARY_DIR}/org.nickvision.application.gresource")
    endif()

    find_program(BLUEPRINT_COMPILER_EXECUTABLE blueprint-compiler)
    if(BLUEPRINT_COMPILER_EXECUTABLE)
        set(BLUEPRINTS
            "blueprints/preferences_dialog.blp"
            "blueprints/shortcuts_dialog.blp"
            "window.blp")
        add_custom_command(TARGET org.nickvision.application.gnome PRE_BUILD COMMAND "${BLUEPRINT_COMPILER_EXECUTABLE}" batch-compile "${CMAKE_CURRENT_BINARY_DIR}/ui" "${CMAKE_CURRENT_SOURCE_DIR}/blueprints" "${CMAKE_CURRENT_SOURCE_DIR}/blueprints/*.blp")
    endif()

    if(HAS_GETTEXT)
        foreach(LINGUA IN ITEMS ${LINGUAS})
            add_custom_command(TARGET org.nickvision.application.gnome PRE_BUILD COMMAND "${CMAKE_COMMAND}" -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/${LINGUA}")
            add_custom_command(TARGET org.nickvision.application.gnome PRE_BUILD COMMAND "${GETTEXT_MSGFMT_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/resources/po/${LINGUA}.po" --output-file="${CMAKE_CURRENT_BINARY_DIR}/${LINGUA}/${PROJECT_NAME}.mo")
        endforeach()
    endif()

    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/resources/${PROJECT_NAME}.desktop.in" "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.desktop" @ONLY)
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/resources/${PROJECT_NAME}.in" "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}" @ONLY)
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/resources/${PROJECT_NAME}.metainfo.xml.in" "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.metainfo.xml" @ONLY)
    install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/" DESTINATION "${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}")
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}" DESTINATION "${CMAKE_INSTALL_BINDIR}")
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.desktop" DESTINATION "share/applications")
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.metainfo.xml" DESTINATION "share/metainfo")
    install(FILES "${CMAKE_SOURCE_DIR}/resources/${PROJECT_NAME}.svg" "${CMAKE_SOURCE_DIR}/resources/${PROJECT_NAME}-devel.svg" DESTINATION "share/icons/hicolor/scalable/apps")
    install(FILES "${CMAKE_SOURCE_DIR}/resources/${PROJECT_NAME}-symbolic.svg" DESTINATION "share/icons/hicolor/symbolic/apps")
endif()