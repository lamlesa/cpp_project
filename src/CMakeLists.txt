cmake_minimum_required(VERSION 3.31.0)
project(project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SFML_SOURCE_DIR "SFML-2.6.2-sources/SFML-2.6.2")

add_subdirectory(${SFML_SOURCE_DIR} sfml-build EXCLUDE_FROM_ALL)

add_executable(project 
    main.cpp
    game_class.cpp
)

target_link_libraries(project PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
)

target_include_directories(project PRIVATE
    "${SFML_SOURCE_DIR}/include"
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
    
    add_custom_command(TARGET project POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SFML_SOURCE_DIR}/bin/Release/sfml-graphics-2.dll"
        "${SFML_SOURCE_DIR}/bin/Release/sfml-window-2.dll"
        "${SFML_SOURCE_DIR}/bin/Release/sfml-system-2.dll"
        "${SFML_SOURCE_DIR}/bin/Release/sfml-audio-2.dll"
        $<TARGET_FILE_DIR:project>
    )
endif()