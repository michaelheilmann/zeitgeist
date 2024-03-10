# Copyright (c) 2024 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)

set(target_name ${project_name}-Library)

set(sources "")
list(APPEND sources Zeitgeist/Value.c Zeitgeist/Value.h)
list(APPEND sources Zeitgeist/List.c Zeitgeist/List.h)
list(APPEND sources Zeitgeist/Object.c Zeitgeist/Object.h)
list(APPEND sources Zeitgeist/Rendition.c Zeitgeist/Rendition.h)
list(APPEND sources Zeitgeist/UpstreamRequests.c Zeitgeist/UpstreamRequests.h)
list(APPEND sources Zeitgeist.c Zeitgeist.h)
list(APPEND sources Zeitgeist/Configure.h.in)

###############################################################################################################################################################

# Enumeration of known/supported C compilers.
set(ZEITGEIST_COMPILER_C_STRING_UNKNOWN "<unknown compiler>")
set(ZEITGEIST_COMPILER_C_ID_UNKNOWN 0)

set(ZEITGEIST_COMPILER_C_STRING_CLANG "CLANG")
set(ZEITGEIST_COMPILER_C_ID_CLANG 1)

set(ZEITGEIST_COMPILER_C_STRING_MSVC "MSVC")
set(ZEITGEIST_COMPILER_C_ID_MSVC 2)

set(ZEITGEIST_COMPILER_C_STRING_GCC "GCC")
set(ZEITGEIST_COMPILER_C_ID_GCC 3)

# Initialize if not yet initialized.
if (NOT DEFINED ${target_name}.COMPILER_C_ID)
  set(${target_name}.COMPILER_C_ID ${ZEITGEIST_COMPILER_C_ID_UNKNOWN})
  set(${target_name}.COMPILER_C_STRING ${ZEITGEIST_COMPILER_C_STRING_UNKNOWN})
endif()
# Perform detection.
if (CMAKE_C_COMPILER_ID)
  if (CMAKE_C_COMPILER_ID MATCHES ".*clang")
    set(${target_name}.COMPILER_C_STRING ${ZEITGEIST_COMPILER_C_STRING_CLANG})
    set(${target_name}.COMPILER_C_ID ${ZEITGEIST_COMPILER_C_ID_CLANG})
  endif()
  if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    set(${target_name}.COMPILER_C_STRING ${ZEITGEIST_COMPILER_C_STRING_GCC})
    set(${target_name}.COMPILER_C_ID ${ZEITGEIST_COMPILER_C_ID_GCC})
  endif()
  if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(${target_name}.COMPILER_C_STRING ${ZEITGEIST_COMPILER_C_STRING_MSVC})
    set(${target_name}.COMPILER_C_ID ${ZEITGEIST_COMPILER_C_ID_MSVC})
  endif()
endif()
message( STATUS " - C compiler: ${${target_name}.COMPILER_C_STRING}")

###############################################################################################################################################################

# Enumeration of known/supported operating systems.
set(ZEITGEIST_OPERATING_SYSTEM_ID_UNKNOWN 0)
set(ZEITGEIST_OPERATING_SYSTEM_STRING_UNKNOWN "<unknown operating system>")

set(ZEITGEIST_OPERATING_SYSTEM_ID_WINDOWS 1)
set(ZEITGEIST_OPERATING_SYSTEM_STRING_WINDOWS "WINDOWS")

set(ZEITGEIST_OPERATING_SYSTEM_ID_UNIX 2)
set(ZEITGEIST_OPERATING_SYSTEM_STRING_UNIX "UNIX")

set(ZEITGEIST_OPERATING_SYSTEM_ID_LINUX 3)
set(ZEITGEIST_OPERATING_SYSTEM_STRING_LINUX "LINUX")

set(ZEITGEIST_OPERATING_SYSTEM_ID_MACOS 4)
set(ZEITGEIST_OPERATING_SYSTEM_STRING_MACOS "MACOS")

set(ZEITGEIST_OPERATING_SYSTEM_ID_CYGWIN 5)
set(ZEITGEIST_OPERATING_SYSTEM_STRING_CYGWIN "CYGWIN")

set(ZEITGEIST_OPERATING_SYSTEM_ID_MSYS 6)
set(ZEITGEIST_OPERATING_SYSTEM_STRING_MSYS "MSYS")

set(ZEITGEIST_OPERATING_SYSTEM_ID_MINGW 7)
set(ZEITGEIST_OPERATING_SYSTEM_STRING_MINGW "MINGW")

set(ZEITGEIST_OPERATING_SYSTEM_ID_IOS 8)
set(ZEITGEIST_OPERATING_SYSTEM_STRING_IOS "IOS")

set(ZEITGEIST_OPERATING_SYSTEM_ID_IOSSIMULATOR 9)
set(ZEITGEIST_OPERATING_SYSTEM_STRING_IOSSIMULATOR "IOS SIMULATOR")

# Initialize if not yet initialized.
if (NOT DEFINED ${target_name}.OPERATING_SYSTEM)
  set(${target}.OPERATING_SYSTEM_ID     ${ZEITGEIST_COMPILER_C_ID_UNKNOWN})
  set(${target}.OPERATING_SYSTEM_STRING ${ZEITGEIST_COMPILER_C_STRING_UNKNOWN})
endif()
if (WIN32)
  set(${target_name}.OPERATING_SYSTEM_ID     ${ZEITGEIST_OPERATING_SYSTEM_ID_WINDOWS})
  set(${target_name}.OPERATING_SYSTEM_STRING ${ZEITGEIST_OPERATING_SYSTEM_STRING_WINDOWS})
elseif (CYGWIN)
  set(${target_name}.OPERATING_SYSTEM_ID     ${ZEITGEIST_OPERATING_SYSTEM_ID_CYGWIN})
  set(${target_name}.OPERATING_SYSTEM_STRING ${ZEITGEIST_OPERATING_SYSTEM_STRING_CYGWIN})
elseif (MSYS)
  set(${target_name}.OPERATING_SYSTEM_ID     ${ZEITGEIST_OPERATING_SYSTEM_ID_MSYS})
  set(${target_name}.OPERATING_SYSTEM_STRING ${ZEITGEIST_OPERATING_SYSTEM_STRING_MSYS})
else()
  if("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
    set(${target_name}.OPERATING_SYSTEM_ID     ${ZEITGEIST_OPERATING_SYSTEM_ID_LINUX})
    set(${target_name}.OPERATING_SYSTEM_STRING ${ZEITGEIST_OPERATING_SYSTEM_STRING_LINUX})
  elseif ("${CMAKE_SYSTEM_NAME}" MATCHES "Darwin")
    set(${target_name}.OPERATING_SYSTEM_ID     ${ZEITGEIST_OPERATING_SYSTEM_ID_MACOS})
    set(${target_name}.OPERATING_SYSTEM_STRING ${ZEITGEIST_OPERATING_SYSTEM_STRING_MACOS})
  else()
    message(FATAL_ERROR "unable to determine operating system")
  endif()
endif()
message( STATUS " - operating system: ${${target_name}.OPERATING_SYSTEM_STRING}")

###############################################################################################################################################################

if (${target_name}.OPERATING_SYSTEM_ID EQUAL ${ZEITGEIST_OPERATING_SYSTEM_ID_WINDOWS})
  set(Zeitgeist_Configuration_OperatingSystem Zeitgeist_Configuration_OperatingSystem_Windows)
endif()
if (${target_name}.OPERATING_SYSTEM_ID EQUAL ${ZEITGEIST_OPERATING_SYSTEM_ID_LINUX})
  set(Zeitgeist_Configuration_OperatingSystem Zeitgeist_Configuration_OperatingSystem_Linux)
endif()

if (${target_name}.COMPILER_C_ID EQUAL ${ZEITGEIST_COMPILER_C_ID_MSVC})
  set(Zeitgeist_Configuration_CompilerC Zeitgeist_Configuration_CompilerC_Msvc)
endif()
if (${target_name}.COMPILER_C_ID EQUAL ${ZEITGEIST_COMPILER_C_ID_GCC})
  set(Zeitgeist_Configuration_CompilerC Zeitgeist_Configuration_CompilerC_Gcc)
endif()
set(Zeitgeist_Configuration_InstructionSetArchitecture Zeitgeist_Configuration_InstructionSetArchitecture_X64)

#message(STATUS "CMAKE_CURRENT_BINARY_DIR = ${CMAKE_CURRENT_BINARY_DIR}")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Zeitgeist/Configure.h.in Zeitgeist/Configure.h)
list(APPEND sources Zeitgeist/Configure.h)

add_library(${target_name} STATIC ${sources})

target_include_directories(${target_name} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(${target_name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

