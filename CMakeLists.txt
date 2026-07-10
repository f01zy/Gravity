cmake_minimum_required(VERSION 3.5)
project(glavity_simulation C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(GLFW_BUILD_WAYLAND ON)
set(GLFW_BUILD_X11 OFF)
set(OpenGL_GL_PREFERENCE GLVND)
set(TARGET gravity)

file(GLOB SOURCE ./src/*.c)
add_executable(${TARGET} ${SOURCE})
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

target_include_directories(${TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(${TARGET} PRIVATE 
  OpenGL::GL
  GLEW::GLEW
  cglm
  glfw
)

add_subdirectory(./third_party/glfw)
add_subdirectory(./third_party/cglm)
