cmake_minimum_required(VERSION 3.16)
project(murrikey)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON) # Ini wajib tetap ON

find_package(Qt6 REQUIRED COMPONENTS Widgets Gui)
find_package(X11 REQUIRED)

include_directories(include)

# Masukkan file .h ke sini agar AUTOMOC bisa memindainya!
set(HEADERS
    include/config.h
    include/input_manager.h
    include/overlay.h
    include/preferences.h
    include/key_mapper.h
)

set(SOURCES
    src/main.cpp
    src/config.cpp
    src/input_manager.cpp
    src/overlay.cpp
    src/preferences.cpp
    src/key_mapper.cpp
)

# Tambahkan HEADERS ke add_executable
add_executable(murrikey ${SOURCES} ${HEADERS})
target_link_libraries(murrikey Qt6::Widgets X11)
