# Copyright (c) 2024 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)

set(sources "")
list(APPEND sources ServiceOpenGl.c ServiceOpenGl.h)
list(APPEND sources Main.c)
add_library(${project_name}-Hello-World-OpenGl MODULE ${sources})
target_include_directories(${project_name}-Hello-World-OpenGl PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/External/OpenGl/Includes)
target_link_libraries(${project_name}-Hello-World-OpenGl ${project_name}-Library)
add_dependencies(${project_name}-Hello-World-OpenGl ${project_name}-Interpreter)
copy_plugin(${project_name}-Interpreter ${project_name}-Hello-World-OpenGl)
