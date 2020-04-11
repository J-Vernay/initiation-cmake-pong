cmake_minimum_required(VERSION 3.0)
project(Pong VERSION 1.0.0)


find_package(SDL2 2.0 REQUIRED) # bibliothèque SDL2 avec version >= 2.0

add_library(libpong-logic
    pong/GameLogic.cpp
    pong/PaddleLogic.cpp
)
set_property(TARGET libpong-logic PROPERTY CXX_STANDARD 17)
target_include_directories(libpong-logic PRIVATE .)
target_link_libraries(libpong-logic SDL2::SDL2) # pour la détection des touches du clavier

add_executable(pong
    pong/main.cpp
    pong/Drawing.cpp
)
set_property(TARGET pong PROPERTY CXX_STANDARD 17)
target_include_directories(pong PRIVATE .)
target_link_libraries(pong SDL2::SDL2)
target_link_libraries(pong SDL2::SDL2main)
target_link_libraries(pong libpong-logic)