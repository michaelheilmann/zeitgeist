# Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.
# This file is licensed under the "zlib License".

# For MSVC: Turn on several warnings.
macro(configure_warnings_and_errors target)

  if(MSVC)

    set(${target}.compile_options "")

    # C4090:  'opertion' : different 'modifier' qualifiers
    list(APPEND ${target}.compile_options "/we4090")
    # C4133: 'function' incompatible types - from 'type1' to 'type2'
    list(APPEND ${target}.compile_options "/we4133")
    # C4020_ 'function': too many actual parameters
    list(APPEND ${target}.compile_options "/we4020")
    # C4013: 'function': undefined; assuming extern returning int
    list(APPEND ${target}.compile_options "/we4013")
    # C4024: 'function': different types for formal and actual parameter 'number'
    list(APPEND ${target}.compile_options "/we4024")
    # C4716: 'function' must return a value
    list(APPEND ${target}.compile_options "/we4716")
    # C4013: 'function' undefined; assuming extern returning int
    list(APPEND ${target}.compile_options "/we4013")

    # The quotes around "${${target}.compile_options}" concatenate the list elements to a single string separated by semicolons.
    set_source_files_properties(${${target}.sources} PROPERTIES COMPILE_OPTIONS "${${target}.compile_options}")
    set_source_files_properties(${${target}.headers} PROPERTIES COMPILE_OPTIONS "${${target}.compile_options}")

  endif()

endmacro()