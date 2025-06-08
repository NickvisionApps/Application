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
