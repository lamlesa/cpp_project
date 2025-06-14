cmake_minimum_required(VERSION 3.10)
project(game)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(SFML 2 COMPONENTS graphics window system audio REQUIRED)

add_executable(game
    main.cpp
    game_class.cpp
)

target_include_directories(game PRIVATE include)
target_link_libraries(game PRIVATE sfml-graphics sfml-window sfml-system sfml-audio)

option(BUILD_TESTS "Build unit tests" ON)

if(BUILD_TESTS)
    enable_testing()
    
    find_package(GTest REQUIRED)
    
    add_executable(runUnitTests
        ../tests/test_main.cpp
        ../tests/test_list.cpp
        game_class.cpp
    )

    target_include_directories(runUnitTests PRIVATE include)
    
    target_link_libraries(runUnitTests PRIVATE 
        GTest::GTest 
        GTest::Main 
        pthread 
        sfml-graphics sfml-window sfml-system sfml-audio
    )
    
    add_test(NAME GameOfLifeCoreTest COMMAND runUnitTests)
endif()