cmake_minimum_required(VERSION 3.21)
project(Micromouse C)

set(CMAKE_C_STANDARD 99)

add_executable(Micromouse main.c
        API.h
        API.c


        )
