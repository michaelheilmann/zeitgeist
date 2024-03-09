# Copyright (c) 2024 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)

set(project_name "Zeitgeist")

project(${project_name})

add_subdirectory(Library)
add_subdirectory(Interpreter)
add_subdirectory(Renditions/Hello-World)
