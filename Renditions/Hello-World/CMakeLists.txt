# Copyright (c) 2024 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)

add_library(${project_name}-Hello-World MODULE Main.c)
target_link_libraries(${project_name}-Hello-World Zeitgeist-Library)
