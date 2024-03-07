# Copyright (c) 2024 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)

add_executable(${project_name}-Interpreter Main.c)
target_link_libraries(${project_name}-Interpreter ${project_name}-Library)

# Copy the files of the rendition to the interpreter binary directory.
# rendition_name The rendition name.
function(copy_plugin rendition_name)
  get_target_property(source_dir ${rendition_name} SOURCE_DIR)
  
  # (1) create the `Renditions/${rendition_name}` folder in the binary directory.
  add_custom_target(
    ${rendition_name}-directory ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${project_name}-Interpreter>/Renditions/${rendition_name}
    COMMENT "-> CREATING $<TARGET_FILE_DIR:${project_name}-Interpreter>/Renditions/${rendition_name}"
  )

  # (2.1) Copy the readme into `Renditions/${rendition_name}`.
  add_custom_target(
    ${rendition_name}-copy-readme ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different                                     # which executes "cmake - E copy_if_different..."
            "${source_dir}/README.txt"                                                # file to copy
            "$<TARGET_FILE_DIR:${project_name}-Interpreter>/Renditions/${rendition_name}"
    DEPENDS ${rendition_name} ${rendition_name}-create-directory
    COMMENT "-> COPYING ${source_dir}/README.txt to $<TARGET_FILE_DIR:${project_name}-Interpreter>/Renditions/${rendition_name}/"
  )

  # (2.2) Copy the library into `Renditions/${rendition_name}`.
  add_custom_target(
    ${rendition_name}-copy-library ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different                                     # which executes "cmake - E copy_if_different..."
            $<TARGET_FILE:${rendition_name}>                                          # file to copy
            $<TARGET_FILE_DIR:${project_name}-Interpreter>/Renditions/${rendition_name}/library.dll
    DEPENDS ${rendition_name} ${rendition_name}-create-directory
    COMMENT "-> COPYING $<TARGET_FILE:${rendition_name}> to $<TARGET_FILE_DIR:${project_name}-Interpreter>/Renditions/${rendition_name}/library.dll" 
  )

endfunction()

copy_plugin(${project_name}-Hello-World)
