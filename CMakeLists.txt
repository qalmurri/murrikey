cmake_minimum_required(VERSION 3.16)
project(murrikey)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
find_package(Qt6 REQUIRED COMPONENTS Widgets Gui)
find_package(X11 REQUIRED)
include_directories(src src/core src/ui src/utils)
set(HEADERS
    src/utils/config.h
    src/utils/hardware_sync.h
    src/ui/overlay.h
    src/ui/preferences.h
    src/ui/screen_selector.h
    src/core/key_mapper.h
    src/core/input_manager.h
    src/core/app.h
)
set(SOURCES
    src/main.cpp
    src/utils/config.cpp
    src/utils/hardware_sync.cpp
    src/ui/overlay_core.cpp
    src/ui/overlay_ui.cpp
    src/ui/overlay_logic.cpp
    src/ui/preferences.cpp
    src/ui/preferences_ui.cpp
    src/ui/screen_selector.cpp
    src/core/app.cpp
    src/core/key_mapper.cpp
    src/core/key_mapper_data.cpp
    src/core/input_manager.cpp
    src/core/input_x11.cpp
)
add_executable(murrikey ${SOURCES} ${HEADERS})
target_link_libraries(murrikey Qt6::Widgets X11)
