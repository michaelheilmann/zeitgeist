# Copyright (c) 2024 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)

set(sources "")
list(APPEND sources Main.c)

if (WIN32)
	list(APPEND sources ServiceWgl.c ServiceWgl.h)
	target_include_directories(${project_name}-Hello-World-OpenGl PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/External/OpenGl/Includes)
else()
	find_package(X11 REQUIRED)
	find_package(OpenGL REQUIRED)
	if (OpenGL_GLX_FOUND)
		list(APPEND sources ServiceGlx.c ServiceGlx.h)
	else()
		message(FATAL_ERROR "unble to find GLX")
	endif()
endif()

add_library(${project_name}-Hello-World-OpenGl MODULE ${sources})
target_link_libraries(${project_name}-Hello-World-OpenGl ${project_name}-Library)
add_dependencies(${project_name}-Hello-World-OpenGl ${project_name}-Interpreter)

if (WIN32)
else()
	target_link_libraries(${project_name}-Hello-World-OpenGl OpenGL::GLX ${X11_LIBRARIES})
endif()

copy_plugin(${project_name}-Interpreter ${project_name}-Hello-World-OpenGl)
