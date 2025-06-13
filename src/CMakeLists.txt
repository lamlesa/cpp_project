cmake_minimum_required(VERSION 3.10)
project(project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Указываем путь к готовой сборке SFML (скачанной с сайта)
set(SFML_DIR "C:/SFML-2.6.2/lib/cmake/SFML")  # Путь к CMake-файлам SFML
find_package(SFML 2.6 COMPONENTS graphics window system audio REQUIRED)

# 2. Наш исполняемый файл
add_executable(project 
    main.cpp 
    game_class.cpp  # Добавляем все исходные файлы
)

# 3. Подключаем SFML
target_link_libraries(project PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio          # Добавляем аудио
)

# 4. Для Windows: копируем DLL-файлы рядом с исполняемым файлом
if(WIN32)
    # Копируем все нужные DLL из SFML/bin в папку сборки
    file(GLOB SFML_DLLS "C:/SFML-2.6.2/bin/*.dll")
    add_custom_command(TARGET project POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${SFML_DLLS} $<TARGET_FILE_DIR:project>
    )
endif()