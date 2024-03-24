# Copyright (c) 2024 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)

include(${CMAKE_SOURCE_DIR}/3rdparty/idlib-1.2/cmake/all.cmake)

set(name ${project_name}-OpenGl-Commons)
set(${name}.sources "")
set(${name}.headers "")
add_library(${name} STATIC)

define_languages(idlib)
detect_c_compiler(idlib)
detect_instruction_set_architecture(idlib)
detect_operating_system(idlib)

if (WIN32)
  list(APPEND ${name}.sources ServiceWgl.c)
  list(APPEND ${name}.headers ServiceWgl.h)
else()
  find_package(X11 REQUIRED)
  find_package(OpenGL REQUIRED)
  if (OpenGL_GLX_FOUND)
    list(APPEND ${name}.sources ServiceGlx.c)
    list(APPEND ${name}.headers ServiceGlx.h)
  else()
    message(FATAL_ERROR "unble to find GLX")
  endif()
endif()

list(APPEND ${name}.sources ServiceGl.c)
list(APPEND ${name}.headers ServiceGl.h)

list(APPEND ${name}.sources ServiceGl_Functions.i)

list(APPEND ${name}.sources Matrix4R32.c)
list(APPEND ${name}.headers Matrix4R32.h)

list(APPEND ${name}.sources Vector3R32.c)
list(APPEND ${name}.headers Vector3R32.h)

list(APPEND ${name}.sources KeyboardKeyMessage.c)
list(APPEND ${name}.headers KeyboardKeyMessage.h)

configure_warnings_and_errors(${name})
target_sources(${name} PRIVATE ${${name}.sources} ${${name}.headers})
target_link_libraries(${name} ${project_name}-Library)
target_include_directories(${name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if (WIN32)
	target_include_directories(${name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/External/OpenGl/Includes)
else()
	set_property(TARGET ${name} PROPERTY POSITION_INDEPENDENT_CODE ON)
	target_link_libraries(${name} OpenGL::GLX ${X11_LIBRARIES})
endif()
target_link_libraries(${name} idlib)
