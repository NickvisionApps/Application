if(APPLE)
    add_executable(org.nickvision.application.osx
        "src/main.cpp")
    target_include_directories(org.nickvision.application.osx PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
    find_package(wxWidgets CONFIG REQUIRED)
    target_link_libraries(org.nickvision.application.osx PRIVATE libapplication wx::core wx::base)
    
    add_custom_target(gnome_commands ALL DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/__mac.h")
    add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/__mac.h" "${CMAKE_CURRENT_BINARY_DIR}/mac.h" COMMAND "${CMAKE_COMMAND}" -E touch "${CMAKE_CURRENT_BINARY_DIR}/mac.h")

    if(HAS_GETTEXT)
        foreach(LINGUA IN ITEMS ${LINGUAS})
            add_custom_command(TARGET gnome_commands PRE_BUILD COMMAND "${CMAKE_COMMAND}" -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/${LINGUA}")
            add_custom_command(TARGET gnome_commands PRE_BUILD COMMAND "${GETTEXT_MSGFMT_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/resources/po/${LINGUA}.po" --output-file="${CMAKE_CURRENT_BINARY_DIR}/${LINGUA}/${SHORT_NAME}.mo")
        endforeach()
    endif()
endif()
