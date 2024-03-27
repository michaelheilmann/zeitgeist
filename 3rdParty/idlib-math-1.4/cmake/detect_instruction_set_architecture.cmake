# Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.
# This file is licensed under the "zlib License".

# Macro to define an enumeration of instruction set architectures
# ${target}.instruction_set_architecture_(string|id)_(x86|x64)
# Macro to define
# ${target}.instruction_set_architecture_(string|id)
# @param target The target.
macro(detect_instruction_set_architecture target)
  # Unknown instruction set architecture.
  set(${target}.instruction_set_architecture_string_unknown "<unknown instruction set architecture>")
  set(${target}.instruction_set_architecture_id_unknown 0)

  # x86.
  set(${target}.instruction_set_architecture_string_x86 "x86")
  set(${target}.instruction_set_architecture_id_x86 1)

  # x64.
  set(${target}.instruction_set_architecture_string_x64 "x64")
  set(${target}.instruction_set_architecture_id_x64 2)

  # Initialize if not yet initialized.
  if (NOT DEFINED ${target_name}.compiler_c_id)
    set(${target}.instruction_set_architecture_id ${${target}.instruction_set_architecture_id_unknown})
    set(${target}.instruction_set_architecture_string ${${target}.instruction_set_architecture_string_unknown})
  endif()
  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(${target}.instruction_set_architecture_id ${${target}.instruction_set_architecture_id_x64})
    set(${target}.instruction_set_architecture_string ${${target}.instruction_set_architecture_string_x64})
  elseif (CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(${target}.instruction_set_architecture_id ${${target}.instruction_set_architecture_id_x86})
    set(${target}.instruction_set_architecture_string ${${target}.instruction_set_architecture_string_x86})
  endif()
  message( STATUS " - ${target} instruction set architecture: ${${target}.instruction_set_architecture_string}")
endmacro()
