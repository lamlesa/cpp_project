cmake_minimum_required(VERSION 3.10)
project(MyProject VERSION 1.0)

set(CXX_STANDARD 17)
set(SFML_DIR "C:/SFML-2.6.2")

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories("C:/SFML-2.6.2/include")

link_directories("C:/SFML-2.6.2/lib")

set(SFML_DIR "C:/SFML-2.6.2/lib/cmake/SFML")

find_package(SFML 2.6.2 COMPONENTS system window graphics audio network REQUIRED)

file(GLOB_RECURSE SRC_FILES cpp_project/*.cpp)

add_executable(MyProject ${SRC_FILES})

target_link_libraries(MyProject
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
    sfml-network
    openal32
    winmm
    gdi32
    user32
)
