# Copyright (c) 2024 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)

include(${CMAKE_SOURCE_DIR}/3rdParty/idlib-math-1.4/cmake/all.cmake)

set(name ${project_name}-Interpreter)
set(${name}.sources "")
set(${name}.headers "")
add_executable(${name})

list(APPEND ${name}.sources Main.c)

target_sources(${name} PRIVATE ${${name}.sources} ${${name}.headers})
target_link_libraries(${name} ${project_name}-Library)

