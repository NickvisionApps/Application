if(WIN32)
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

add_library(libapplication
    "include/controllers/mainwindowcontroller.h"
    "include/controllers/preferencesviewcontroller.h"
    "include/models/configuration.h"
    "include/models/startupinformation.h"
    "include/models/theme.h"
    "src/controllers/mainwindowcontroller.cpp"
    "src/controllers/preferencesviewcontroller.cpp"
    "src/models/configuration.cpp"
    "src/models/startupinformation.cpp")
target_include_directories(libapplication PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
set_target_properties(libapplication PROPERTIES VERSION "${PROJECT_VERSION}" SOVERSION "${PROJECT_VERSION}")
if(MSVC)
    target_compile_options(libapplication PRIVATE /W4)
else()
    target_compile_options(libapplication PRIVATE -Wall -Wextra -Wpedantic)
endif()

find_package(libnick CONFIG REQUIRED)
target_link_libraries(libapplication PUBLIC libnick::libnick)

if(HAS_GETTEXT)
    add_custom_command(TARGET libapplication PRE_BUILD COMMAND "${GETTEXT_XGETTEXT_EXECUTABLE}" --from-code=utf-8 --c++ --force-po --output="${POT_FILE}" --keyword=_ --keyword=_f --keyword=_n:1,2 --keyword=_fn:1,2 --keyword=_p:1c,2 --keyword=_pn:1c,2,3 --keyword=C_:1c,2 --width=80 ${TRANSLATE_FILES} WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
    foreach(LINGUA IN ITEMS ${LINGUAS})
        add_custom_command(TARGET libapplication PRE_BUILD COMMAND "${GETTEXT_MSGMERGE_EXECUTABLE}" --backup=off --update "${CMAKE_SOURCE_DIR}/resources/po/${LINGUA}.po" "${POT_FILE}")
    endforeach()
else()
    message(WARNING "gettext not found, translations will not be generated")
endif()
