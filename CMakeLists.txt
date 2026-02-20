cmake_minimum_required(VERSION 3.16)
project(murrikey)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
find_package(Qt6 REQUIRED COMPONENTS Widgets Gui)
find_package(X11 REQUIRED)
include_directories(include)
set(HEADERS
    src/config.h
    src/input_manager.h
    src/overlay.h
    src/preferences.h
    src/key_mapper.h
)
set(SOURCES
    src/main.cpp
    src/config.cpp
    src/input_manager.cpp
    src/overlay.cpp
    src/preferences.cpp
    src/key_mapper.cpp
)
add_executable(murrikey ${SOURCES} ${HEADERS})
target_link_libraries(murrikey Qt6::Widgets X11)
