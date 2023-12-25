cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(RenderView)

find_package(VTK COMPONENTS 
  CommonColor
  CommonCore
  FiltersSources
  InteractionStyle
  ViewsQt
  RenderingContextOpenGL2
  RenderingCore
  RenderingFreeType
  RenderingGL2PSOpenGL2
  RenderingOpenGL2
)

# RenderingUI
# find_package(VTK COMPONENTS)

if (NOT VTK_FOUND)
  message(FATAL_ERROR "${CMAKE_PROJECT_NAME}: Unable to find the VTK build folder.")
endif()

aux_source_directory(./ SRC)

# Prevent a "command line is too long" failure in Windows.
set(CMAKE_NINJA_FORCE_RESPONSE_FILE "ON" CACHE BOOL "Force Ninja to use response files.")

add_executable(${CMAKE_PROJECT_NAME} MACOSX_BUNDLE ${SRC} )
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${VTK_LIBRARIES})
# vtk_module_autoinit is needed
vtk_module_autoinit(
  TARGETS ${CMAKE_PROJECT_NAME}
  MODULES ${VTK_LIBRARIES}
)

message("---libs name:"  ${VTK_LIBRARIES})

# code from https://examples.vtk.org/site/Cxx/SimpleOperations/DistanceBetweenPoints/
