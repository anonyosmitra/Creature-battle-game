cmake_minimum_required(VERSION 3.21)
project(Test)

set(CMAKE_CXX_STANDARD 20)
add_executable(Test main.cpp randoms.cpp randoms.hpp creature.cpp creature.hpp enemy.cpp enemy.hpp game.cpp game.hpp)
