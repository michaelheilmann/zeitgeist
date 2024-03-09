# Copyright (c) 2024 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)

message(STATUS "defining target ${project_name}-Interpreter")
add_executable(${project_name}-Interpreter Main.c)

target_link_libraries(${project_name}-Interpreter ${project_name}-Library)

