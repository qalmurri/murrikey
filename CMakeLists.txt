cmake_minimum_required(VERSION 3.10)
project(MurriKey)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON) # Penting untuk Qt

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(X11 REQUIRED)

add_executable(murrikey main.cpp)

target_link_libraries(murrikey PRIVATE 
    Qt6::Widgets 
    ${X11_LIBRARIES}
)
