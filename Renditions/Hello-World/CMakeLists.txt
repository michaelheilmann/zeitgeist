# Copyright (c) 2024 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)

# Copy the files of the rendition to the interpreter binary directory.
# rendition_name The rendition name.
macro(copy_plugin interpreter_name rendition_name)
  get_target_property(source_dir ${rendition_name} SOURCE_DIR)
  message(STATUS "copying ${rendition_name} to ${interpreter_name}")
  
  add_custom_command(
    TARGET ${rendition_name} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${interpreter_name}>/Renditions/${rendition_name}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different                                     # which executes "cmake - E copy_if_different..."
            ${source_dir}/README.txt                                                  # file to copy
            $<TARGET_FILE_DIR:${interpreter_name}>/Renditions/${rendition_name}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different                                     # which executes "cmake - E copy_if_different..."
            $<TARGET_FILE:${rendition_name}>                                          # file to copy
            $<TARGET_FILE_DIR:${project_name}-Interpreter>/Renditions/${rendition_name}/library.so
  )
endmacro()

add_library(${project_name}-Hello-World MODULE Main.c)
target_link_libraries(${project_name}-Hello-World ${project_name}-Library)
add_dependencies(${project_name}-Hello-World ${project_name}-Interpreter)
copy_plugin(${project_name}-Interpreter ${project_name}-Hello-World)
