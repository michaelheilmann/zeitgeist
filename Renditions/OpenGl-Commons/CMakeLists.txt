# Copyright (c) 2024 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)

set(sources "")

if (WIN32)
	list(APPEND sources ServiceWgl.c ServiceWgl.h)
else()
	find_package(X11 REQUIRED)
	find_package(OpenGL REQUIRED)
	if (OpenGL_GLX_FOUND)
		list(APPEND sources ServiceGlx.c ServiceGlx.h)
	else()
		message(FATAL_ERROR "unble to find GLX")
	endif()
endif()

list(APPEND sources ServiceGl.c ServiceGl.h)
list(APPEND sources ServiceGl_Functions.i)

add_library(${project_name}-OpenGl-Commons STATIC ${sources})
target_link_libraries(${project_name}-OpenGl-Commons ${project_name}-Library)
target_include_directories(${project_name}-OpenGl-Commons PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if (WIN32)
	target_include_directories(${project_name}-OpenGl-Commons PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/External/OpenGl/Includes)
else()
	target_link_libraries(${project_name}-OpenGl-Commons OpenGL::GLX ${X11_LIBRARIES})
endif()
