# Copyright (c) 2023-2024 Michael Heilmann. All rights reserved.
# This file is licensed under the "zlib License".

cmake_minimum_required(VERSION 3.20)

project(idlib-math LANGUAGES C)

set(idlib-math-cmake-files-dir ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

add_subdirectory(library)

enable_testing()
add_subdirectory(test/matrix_4x4)
add_subdirectory(test/vector_2)
add_subdirectory(test/vector_3)
