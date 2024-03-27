# Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.
# This file is licensed under the "zlib License".

# Macro to define an enumeration of operating systems
# ${target}.operating_system_(id|string)_(unknown|windows|unix|linux|macos|cygwin|msys|mingw|ios|ios_simulator)
# Macro to define
# ${target}.operating_system_(id|string)
# @param target The target.
# @remarks
# - "mingw" and/or "msys" is a "toolchain". The "operating system" is still "windows".
# - We are not aware of a reliable procedure to detect what Apple "operating system" is used.
#   This configuration script detects "Mac OS" even if the "operating system" is "iOS", "iOS Simulator", "watchOS", ... from within CMake?
macro(detect_operating_system target)
  set(${target}.operating_system_id_unknown 0)
  set(${target}.operating_system_string_unknown "<unknown operating system>")

  set(${target}.operating_system_id_windows 1)
  set(${target}.operating_system_string_windows "WINDOWS")

  set(${target}.operating_system_id_unix 2)
  set(${target}.operating_system_string_unix "UNIX")

  set(${target}.operating_system_id_linux 3)
  set(${target}.operating_system_string_linux "LINUX")

  set(${target}.operating_system_id_macos 4)
  set(${target}.operating_system_string_macos "MACOS")

  set(${target}.operating_system_id_cygwin 5)
  set(${target}.operating_system_string_cygwin "CYGWIN")

  set(${target}.operating_system_id_msys 6)
  set(${target}.operating_system_string_msys "MSYS")

  set(${target}.operating_system_id_mingw 7)
  set(${target}.operating_system_string_mingw "MINGW")

  set(${target}.operating_system_id_ios 8)
  set(${target}.operating_system_string_ios "IOS")

  set(${target}.operating_system_id_ios_simulator 9)
  set(${target}.operating_system_string_ios_simulator "IOS SIMULATOR")

  if (NOT DEFINED ${target}.operating_system_id)  
    set(${target}.operating_system_id ${${target}.operating_system_id_unknown})
    set(${target}.operating_system_string ${${target}.operating_system_string_unknown})
    if (WIN32)
      set(${target}.operating_system_id ${${target}.operating_system_id_windows})
      set(${target}.operating_system_string ${${target}.operating_system_string_windows})
    elseif (CYGWIN)
      set(${target}.operating_system_id ${${target}.operating_system_id_cygwin})
      set(${target}.operating_system_string ${${target}.operating_system_string_cygwin}})  
    elseif (MSYS)
      set(${target}.operating_system_id ${${target}.operating_system_id_msys})
      set(${target}.operating_system_string ${${target}.operating_system_string_msys})
    endif()

    if (${${target}.operating_system_id} EQUAL ${${target}.operating_system_id_unknown})
      if("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
        set(${target}.operating_system_id ${${target}.operating_system_id_linux})
        set(${target}.operating_system_string ${${target}.operating_system_string_linux})
      elseif ("${CMAKE_SYSTEM_NAME}" MATCHES "Darwin")
        set(${target}.operating_system_id ${${target}.operating_system_id_macos})
        set(${target}.operating_system_string ${${target}.operating_system_string_macos})
      endif()
    endif()
  endif()
  message(STATUS " - ${target} operating system: ${${target}.operating_system_string}")
endmacro()
