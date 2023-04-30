cmake_minimum_required(VERSION 3.25)
project(ex1_mtm C)

set(CMAKE_C_STANDARD 99)

include_directories(.)

add_executable(ex1_mtm
        IsraeliQueue.c
        IsraeliQueue.h main.c)
