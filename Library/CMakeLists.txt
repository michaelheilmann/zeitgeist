# Copyright (c) 2024 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)

include(${CMAKE_SOURCE_DIR}/3rdparty/idlib-math-1.3/cmake/all.cmake)

set(name ${project_name}-Library)
set(${name}.sources "")
set(${name}.headers "")
add_library(${name} STATIC)

define_languages(${name})
detect_c_compiler(${name})
detect_instruction_set_architecture(${name})
detect_operating_system(${name})

list(APPEND ${name}.sources Zeitgeist/Configure.h.in)

list(APPEND ${name}.sources Zeitgeist.c)
list(APPEND ${name}.headers Zeitgeist.h)

list(APPEND ${name}.sources Zeitgeist/Value.c)
list(APPEND ${name}.headers Zeitgeist/Value.h)

list(APPEND ${name}.sources Zeitgeist/List.c)
list(APPEND ${name}.headers Zeitgeist/List.h)

list(APPEND ${name}.sources Zeitgeist/Map.c)
list(APPEND ${name}.headers Zeitgeist/Map.h)

list(APPEND ${name}.sources Zeitgeist/Stack.c)
list(APPEND ${name}.headers Zeitgeist/Stack.h)

list(APPEND ${name}.sources Zeitgeist/String.c)
list(APPEND ${name}.headers Zeitgeist/String.h)

list(APPEND ${name}.sources Zeitgeist/Object.c)
list(APPEND ${name}.headers Zeitgeist/Object.h)

list(APPEND ${name}.sources Zeitgeist/Gc.c)
list(APPEND ${name}.headers Zeitgeist/Gc.h)

list(APPEND ${name}.sources Zeitgeist/WeakReferences.c)
list(APPEND ${name}.headers Zeitgeist/WeakReferences.h)

list(APPEND ${name}.sources Zeitgeist/Locks.c)
list(APPEND ${name}.headers Zeitgeist/Locks.h)

list(APPEND ${name}.sources Zeitgeist/Rendition.c)
list(APPEND ${name}.headers Zeitgeist/Rendition.h)

list(APPEND ${name}.sources Zeitgeist/UpstreamRequests.c)
list(APPEND ${name}.headers Zeitgeist/UpstreamRequests.h)

# Configuration variables for 'Configure.h.in'.
if (${${name}.operating_system_id} EQUAL ${${name}.operating_system_id_windows})
  set(Zeitgeist_Configuration_OperatingSystem Zeitgeist_Configuration_OperatingSystem_Windows)
endif()
if (${${name}.operating_system_id} EQUAL ${${name}.operating_system_id_linux})
  set(Zeitgeist_Configuration_OperatingSystem Zeitgeist_Configuration_OperatingSystem_Linux)
endif()

if (${${name}.compiler_c_id} EQUAL ${${name}.compiler_c_id_msvc})
  set(Zeitgeist_Configuration_CompilerC Zeitgeist_Configuration_CompilerC_Msvc)
endif()
if (${${name}.compiler_c_id} EQUAL ${${name}.compiler_c_id_gcc})
  set(Zeitgeist_Configuration_CompilerC Zeitgeist_Configuration_CompilerC_Gcc)
endif()

if (${${name}.instruction_set_architecture_id} EQUAL ${${name}.instruction_set_architecture_id_x64})
  set(Zeitgeist_Configuration_InstructionSetArchitecture Zeitgeist_Configuration_InstructionSetArchitecture_X64)
endif()
if (${${name}.instruction_set_architecture_id} EQUAL ${${name}.instruction_set_architecture_id_x86})
  set(Zeitgeist_Configuration_InstructionSetArchitecture Zeitgeist_Configuration_InstructionSetArchitecture_X86)
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Zeitgeist/Configure.h.in Zeitgeist/Configure.h)
list(APPEND ${name}.sources Zeitgeist/Configure.h)

configure_warnings_and_errors(${name})
target_sources(${name} PRIVATE ${${name}.sources} ${${name}.headers})
if (${${name}.operating_system_id} EQUAL ${${name}.operating_system_id_linux})
	set_property(TARGET ${name} PROPERTY POSITION_INDEPENDENT_CODE ON)
endif()

target_include_directories(${name} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(${name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
