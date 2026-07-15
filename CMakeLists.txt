cmake_minimum_required(VERSION 3.5)
project(glavity_simulation C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(GLFW_BUILD_WAYLAND ON)
set(GLFW_BUILD_X11 OFF)
set(OpenGL_GL_PREFERENCE GLVND)
set(TARGET gravity)
set(SOURCE
  ./src/game/main.c
  ./src/game/window.c
  ./src/game/input.c
  ./src/core/time.c
  ./src/core/utility.c
  ./src/physics/physics.c
  ./src/grid/grid.c
  ./src/renderer/camera.c
  ./src/renderer/renderer.c
  ./src/renderer/text.c
  ./src/renderer/hud.c
  ./src/resources/font.c
  ./src/resources/mesh.c
  ./src/resources/shader.c
  ./src/resources/sphere.c
  ./src/resources/texture.c
  ./src/resources/resource_manager.c
)

add_executable(${TARGET} ${SOURCE})
find_package(OpenGL REQUIRED)
add_subdirectory(./third_party/glfw)
add_subdirectory(./third_party/cglm)
add_subdirectory(./third_party/glad)
add_subdirectory(./third_party/freetype)

target_include_directories(${TARGET} PRIVATE 
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ./third_party/stb
)

target_link_libraries(${TARGET} PRIVATE 
  OpenGL::GL
  glad
  cglm
  glfw
  freetype
)
