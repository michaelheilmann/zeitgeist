# Copyright (c) 2023-2024 Michael Heilmann. All rights reserved.
# This file is licensed under the "zlib License".

cmake_minimum_required(VERSION 3.20)

include(${idlib-math-cmake-files-dir}/all.cmake)

set(name idlib-math)
begin_library()

if (${${name}.compiler_c_id} EQUAL ${${name}.compiler_c_id_msvc})
  set("IDLIB_COMPILER_C" "IDLIB_COMPILER_C_MSVC")
elseif (${${name}.compiler_c_id} EQUAL ${${name}.compiler_c_id_gcc})
  set("IDLIB_COMPILER_C" "IDLIB_COMPILER_C_GCC")
elseif (${${name}.COMPILER_C_ID} EQUAL ${${name}.compiler_c_id_unknown})
  set("IDLIB_COMPILER_C" "IDLIB_COMPILER_C_UNKNOWN")
else()
  message(FATAL_ERROR "C compiler detection not executed")
endif()

if (${${name}.instruction_set_architecture_id} EQUAL ${${name}.instruction_set_architecture_id_x64})
  set("IDLIB_INSTRUCTION_SET_ARCHITECTURE" "IDLIB_INSTRUCTION_SET_ARCHITECTURE_X64")
elseif (${${name}.instruction_set_architecture_id} EQUAL ${${name}.instruction_set_architecture_id_x86})
  set("IDLIB_INSTRUCTION_SET_ARCHITECTURE" "IDLIB_INSTRUCTION_SET_ARCHITECTURE_X86")
elseif (${${name}.instruction_set_architecture_id} EQUAL ${${name}.instruction_set_architecture_id_unknown})
  set("IDLIB_INSTRUCTION_SET_ARCHITECTURE" "IDLIB_INSTRUCTION_SET_ARCHITECTURE_UNKNOWN")
else()
  message(FATAL_ERROR "instruction set architecture not executed")
endif()

if (${${name}.operating_system_id} EQUAL ${${name}.operating_system_id_windows})
  set("IDLIB_OPERATING_SYSTEM" "IDLIB_OPERATING_SYSTEM_WINDOWS")
elseif (${${name}.operating_system_id} EQUAL ${${name}.operating_system_id_linux})
  set("IDLIB_OPERATING_SYSTEM" "IDLIB_OPERATING_SYSTEM_LINUX")
elseif (${${name}.operating_system_id} EQUAL ${${name}.operating_system_id_unknown})
  set("IDLIB_OPERATING_SYSTEM" "IDLIB_OPERATING_SYSTEM_UNKNOWN")
else()
  message(FATAL_ERROR "operating system detection not executed")
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/includes/configure.h.in ${CMAKE_CURRENT_BINARY_DIR}/includes/configure.h)

list(APPEND ${name}.configuration_files "${CMAKE_CURRENT_BINARY_DIR}/includes/configure.h")

list(APPEND ${name}.header_files "${CMAKE_CURRENT_SOURCE_DIR}/includes/idlib-math.h")
list(APPEND ${name}.source_files "${CMAKE_CURRENT_SOURCE_DIR}/sources/idlib-math.c")

list(APPEND ${name}.header_files "${CMAKE_CURRENT_SOURCE_DIR}/includes/scalar.h")
list(APPEND ${name}.source_files "${CMAKE_CURRENT_SOURCE_DIR}/sources/scalar.c")

list(APPEND ${name}.header_files "${CMAKE_CURRENT_SOURCE_DIR}/includes/matrix_4x4.h")
list(APPEND ${name}.source_files "${CMAKE_CURRENT_SOURCE_DIR}/sources/matrix_4x4.c")

list(APPEND ${name}.header_files "${CMAKE_CURRENT_SOURCE_DIR}/includes/vector_2.h")
list(APPEND ${name}.source_files "${CMAKE_CURRENT_SOURCE_DIR}/sources/vector_2.c")

list(APPEND ${name}.header_files "${CMAKE_CURRENT_SOURCE_DIR}/includes/vector_3.h")
list(APPEND ${name}.source_files "${CMAKE_CURRENT_SOURCE_DIR}/sources/vector_3.c")

list(APPEND ${name}.header_files "${CMAKE_CURRENT_SOURCE_DIR}/includes/color.h")
list(APPEND ${name}.source_files "${CMAKE_CURRENT_SOURCE_DIR}/sources/color.c")

list(APPEND ${name}.header_files "${CMAKE_CURRENT_SOURCE_DIR}/includes/version.h")
list(APPEND ${name}.source_files "${CMAKE_CURRENT_SOURCE_DIR}/sources/version.c")

list(APPEND ${name}.header_files "${CMAKE_CURRENT_SOURCE_DIR}/includes/colors.i")
list(APPEND ${name}.header_files "${CMAKE_CURRENT_SOURCE_DIR}/includes/colors.h")
list(APPEND ${name}.source_files "${CMAKE_CURRENT_SOURCE_DIR}/sources/colors.c")

end_library()

source_group(TREE ${CMAKE_CURRENT_BINARY_DIR} FILES ${${name}.configuration_files})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.header_files})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.source_files})
