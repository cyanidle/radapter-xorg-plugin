cmake_minimum_required(VERSION 3.16)
project(radapter-xorg-plugin)

find_package(Qt5 REQUIRED COMPONENTS Core)
set(CMAKE_AUTOMOC ON)

add_subdirectory(radapter)

create_radapter_plugin(radapter_xorg_plugin
    FILES xorg.cpp
)
target_link_libraries(radapter_xorg_plugin PRIVATE Xtst)