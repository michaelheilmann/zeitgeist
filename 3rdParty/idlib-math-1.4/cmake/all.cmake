# Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.
# This file is licensed under the "zlib License".

include(${CMAKE_CURRENT_LIST_DIR}/configure_warnings_and_errors.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/define_languages.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/detect_instruction_set_architecture.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/detect_operating_system.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/detect_void_pointer_size.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/detect_compiler.cmake)

macro(begin_library)

  set(${name}.source_files "")
  set(${name}.header_files "")
  set(${name}.configuration_files "")

  define_languages(${name})
  detect_c_compiler(${name})
  detect_instruction_set_architecture(${name})
  detect_operating_system(${name})

endmacro()

macro(end_library)

  add_library(${name} STATIC)

  target_include_directories(${name} PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/includes")
  target_include_directories(${name} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/includes")

  target_sources(${name} PRIVATE ${${name}.configuration_files} ${${name}.header_files} ${${name}.source_files})

  configure_warnings_and_errors(${name})

endmacro()

macro(begin_executable)

  set(${name}.source_files "")
  set(${name}.header_files "")
  set(${name}.configuration_files "")

  define_languages(${name})
  detect_c_compiler(${name})
  detect_instruction_set_architecture(${name})
  detect_operating_system(${name})

endmacro()

macro(end_executable)

  add_executable(${name} ${${name}.configuration_files} ${${name}.header_files} ${${name}.source_files})

  target_include_directories(${name} PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/includes")
  target_include_directories(${name} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/includes")

  configure_warnings_and_errors(${name})

endmacro()
