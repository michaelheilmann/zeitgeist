# Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.
# This file is licensed under the "zlib License".

# Macro to define an enumeration of languages.
# The enumeration constants ${target}.language_(string|id)_(unknown|c|cpp|masm) are defined where
# ${target}.language_id_* are numeric ID, which are guaranteed to be unique, and ${target}.language_string_* are a human-readable strings which is merely descriptive.
# @param target The target.
macro(define_languages target)
  # Unknown language.
  set(${target}.language_string_unknown "<unknown language>")
  set(${target}.language_id_unknown 0)
  # The "C" language.
  set(${target}.language_string_c "C")
  set(${target}.language_id_c 1)
  # The "C++" language.
  set(${target}.language_string_cpp "CPP")
  set(${target}.language_id_cpp 2)
  # The "MASM" language.
  set(${target}.language_string_masm "MASM")
  set(${target}.language_id_masm 3)
endmacro()
