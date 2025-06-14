cmake_minimum_required(VERSION 3.31.0)
project(project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

set(SFML_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SFML-2.6.2-sources/SFML-2.6.2")

add_subdirectory(${SFML_SOURCE_DIR} sfml-build
    EXCLUDE_FROM_ALL
)

add_executable(project main.cpp game_class.cpp)

target_link_libraries(project PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
)

add_executable(project_tests
    ../tests/test_main.cpp
    ../tests/test_list.cpp
    game_class.cpp
)

target_link_libraries(project_tests PRIVATE
    sfml-system
    gtest_main
)

target_include_directories(project_tests PRIVATE
    "${SFML_SOURCE_DIR}/include"
    ${googletest_SOURCE_DIR}/googletest/include
)

add_test(NAME project_tests COMMAND project_tests)

set_target_properties(project PROPERTIES
    BUILD_RPATH "${CMAKE_BINARY_DIR}/sfml-build/lib"
    INSTALL_RPATH "${CMAKE_BINARY_DIR}/sfml-build/lib"
)

if(WIN32)
    add_custom_command(TARGET project POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SFML_SOURCE_DIR}/bin/Debug/sfml-graphics-d-2.dll"
        "${SFML_SOURCE_DIR}/bin/Debug/sfml-window-d-2.dll"
        "${SFML_SOURCE_DIR}/bin/Debug/sfml-system-d-2.dll"
        "${SFML_SOURCE_DIR}/bin/Debug/sfml-audio-d-2.dll"
        $<TARGET_FILE_DIR:project>
    )
    
    add_custom_command(TARGET project_tests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SFML_SOURCE_DIR}/bin/Debug/sfml-system-d-2.dll"
        $<TARGET_FILE_DIR:project_tests>
    )
endif()