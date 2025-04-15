cmake_minimum_required(VERSION 3.28)
project(RPG_Project)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(RPG_Project
    Charaktere.cpp
    Charaktere.h)
