# Copyright (c) 2024 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)

include(${CMAKE_SOURCE_DIR}/3rdparty/idlib-math-1.3/cmake/all.cmake)

set(name ${project_name}-Hello-World-OpenGl)
set(${name}.sources "")
set(${name}.headers "")
add_library(${name} MODULE)

list(APPEND ${name}.sources Sources/Main.c)

configure_warnings_and_errors(${name})
target_sources(${name} PRIVATE ${${name}.sources} ${${name}.headers})
target_link_libraries(${name} ${project_name}-OpenGl-Commons ${project_name}-Library)
add_dependencies(${name} ${project_name}-Interpreter)

copy_plugin(${project_name}-Interpreter ${name})
