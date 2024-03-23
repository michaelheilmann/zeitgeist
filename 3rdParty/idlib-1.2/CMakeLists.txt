# Copyright (c) 2023-2024 Michael Heilmann. All rights reserved.
# This file is licensed under MIT license.

cmake_minimum_required(VERSION 3.20)

project(idlib LANGUAGES C)

###############################################################################################################################################################

include(${CMAKE_CURRENT_LIST_DIR}/cmake/all.cmake)

set(name idlib)

add_library(${name} STATIC)
set(${name}.source_files "")
set(${name}.header_files "")
set(${name}.configuration_files "")

define_languages(${name})
detect_c_compiler(${name})
detect_instruction_set_architecture(${name})
detect_operating_system(${name})

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

target_include_directories(${name} PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/includes)
list(APPEND ${name}.configuration_files "${CMAKE_CURRENT_BINARY_DIR}/includes/configure.h")

list(APPEND ${name}.header_files "${CMAKE_CURRENT_SOURCE_DIR}/includes/idlib.h")
list(APPEND ${name}.source_files "${CMAKE_CURRENT_SOURCE_DIR}/sources/idlib.c")

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

configure_warnings_and_errors(${name})

target_sources(${name} PRIVATE ${${name}.configuration_files} ${${name}.header_files} ${${name}.source_files})

source_group(TREE ${CMAKE_CURRENT_BINARY_DIR} FILES ${${name}.configuration_files})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.header_files})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.source_files})

target_include_directories(${name} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/includes")
#target_source_files(${name} PRIVATE ${${name}.header_files} ${${name}.source_files})
