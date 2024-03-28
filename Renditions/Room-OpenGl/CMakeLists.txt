# Copyright (c) 2024 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)

include(${CMAKE_SOURCE_DIR}/3rdParty/idlib-math-1.4/cmake/all.cmake)

set(name ${project_name}-Room-OpenGl)
set(${name}.sources "")
set(${name}.headers "")
add_library(${name} MODULE)

define_languages(${name})
detect_c_compiler(${name})
detect_instruction_set_architecture(${name})
detect_operating_system(${name})

list(APPEND ${name}.sources Sources/Main.c)
list(APPEND ${name}.sources Sources/World.c)
list(APPEND ${name}.headers Sources/World.h)

configure_warnings_and_errors(${name})
target_sources(${name} PRIVATE ${${name}.sources} ${${name}.headers})
target_link_libraries(${name} ${project_name}-OpenGl-Commons ${project_name}-Library)
add_dependencies(${name} ${project_name}-Interpreter)

copy_plugin(${project_name}-Interpreter ${name})
