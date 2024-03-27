# Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.
# This file is licensed under the "zlib License".

# Macro to define an enumeration of compilers:
# ${target}_compiler_c_(id|string)_(unknown|clang|msvc|gcc)
# Macro to define
# ${target}.compiler_c_(id|string)
# @param target The target.
macro(detect_c_compiler target)
  # An unknown compiler.
  set(${target}.compiler_c_string_unknown "<unknown compiler>")
  set(${target}.compiler_c_id_unknown 0)

  # CLANG.
  set(${target}.compiler_c_string_clang "CLANG")
  set(${target}.compiler_c_id_clang 1)

  # MSVC.
  set(${target}.compiler_c_string_msvc "MSVC")
  set(${target}.compiler_c_id_msvc 2)

  # GCC.
  set(${target}.compiler_c_string_gcc "GCC")
  set(${target}.compiler_c_id_gcc 3)

  # Initialize if not yet initialized.
  if (NOT DEFINED ${target_name}.compiler_c_id)
    set(${target}.compiler_c_id ${${target}.compiler_c_id_unknown})
    set(${target}.compiler_c_string ${${target}.compiler_c_string_unknown})
  endif()
  # Perform detection.
  if (CMAKE_C_COMPILER_ID)
    if (CMAKE_C_COMPILER_ID MATCHES ".*clang")
      set(${target}.compiler_c_string ${${target}.compiler_c_string_clang})
      set(${target}.compiler_c_id ${${target}.compiler_c_id_clang})
    endif()
    if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
      set(${target}.compiler_c_string ${${target}.compiler_c_string_gcc})
      set(${target}.compiler_c_id ${${target}.compiler_c_id_gcc})
    endif()
    if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
      set(${target}.compiler_c_string ${${target}.compiler_c_string_msvc})
      set(${target}.compiler_c_id ${${target}.compiler_c_id_msvc})
    endif()
  endif()
  message( STATUS " - ${target} C compiler: ${${target}.compiler_c_string}")
endmacro()
