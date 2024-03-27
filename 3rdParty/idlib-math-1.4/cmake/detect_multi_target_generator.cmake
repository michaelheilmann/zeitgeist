# Copyright (c) 2018-2024 Michael Heilmann
# This file is licensed under the "zlib License".

# Macro to detect a multi target generator is used.
# Set ${target}.is_multi_target_generator to "YES" (a multi target generator is used) or "NO" (a single target generator is used).
# @param target The target.
macro(detect_multi_target_generator target)
  if (NOT DEFINED ${target}.compiler_c_id)
    message(FATAL_ERROR "please execute detect_compiler before detect_multi_target_generator")
  endif()
  if (${${target}.c_compiler_id} EQUAL ${${${target}.c_compiler_id_msvc}})
    set(${target}.is_multi_target_generator YES)
  else()
    set(${target}.is_multi_target_generator NO)
  endif()
  message(" - ${target} multi-target generator: ${${target}.is_multi_target_generator}")
endmacro(detect_multi_target_generator)
