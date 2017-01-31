cmake_minimum_required(VERSION 3.6)
project(proj1_ami)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        hashtable.c
        hashtable.h
        philspel.c
        philspel.h)

add_executable(proj1_ami ${SOURCE_FILES})