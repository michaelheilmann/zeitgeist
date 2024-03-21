# Copyright (c) 2024 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)

set(name ${project_name}-Room-OpenGl)
set(sources "")
list(APPEND sources Sources/Main.c)
list(APPEND sources Sources/World.c Sources/World.h)

add_library(${name} MODULE ${sources})
target_link_libraries(${name} ${project_name}-OpenGl-Commons ${project_name}-Library)
add_dependencies(${name} ${project_name}-Interpreter)

copy_plugin(${project_name}-Interpreter ${name})
