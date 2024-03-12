# Copyright (c) 2024 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)

set(sources "")
list(APPEND sources Sources/Main.c)

add_library(${project_name}-Hello-World MODULE ${sources})
target_link_libraries(${project_name}-Hello-World ${project_name}-Library)
add_dependencies(${project_name}-Hello-World ${project_name}-Interpreter)

copy_plugin(${project_name}-Interpreter ${project_name}-Hello-World)
