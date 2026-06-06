enable_testing()

find_package(GTest CONFIG REQUIRED)
find_package(libdesktop CONFIG REQUIRED)
include(GoogleTest)

add_executable(${TESTS_PROJECT_NAME}
    main.cpp
)

if(ENABLE_CLANG_TIDY AND CLANG_TIDY)
    if(MSVC)
        set_target_properties(${TESTS_PROJECT_NAME} PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY};--extra-arg=/EHsc")
    elseif(LINUX)
        set_target_properties(${TESTS_PROJECT_NAME} PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY};--extra-arg=-fexceptions --extra-arg=-std=c++20")
    else()
        set_target_properties(${TESTS_PROJECT_NAME} PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY};--extra-arg=-fexceptions")
    endif()
endif()

if(ENABLE_CLANG_FORMAT AND CLANG_FORMAT)
    file(GLOB_RECURSE FORMAT_SOURCE_FILES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/**/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.cpp")
    add_custom_target(tests_format
        COMMAND ${CLANG_FORMAT} -i ${FORMAT_SOURCE_FILES}
        COMMENT "Running clang-format")
    add_dependencies(${TESTS_PROJECT_NAME} tests_format)
endif()

target_link_libraries(${TESTS_PROJECT_NAME} PRIVATE GTest::gtest GTest::gmock libdesktop::libdesktop)

gtest_discover_tests(${TESTS_PROJECT_NAME})