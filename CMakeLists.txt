cmake_minimum_required(VERSION 3.17)
project(Voltray)

set(CMAKE_CXX_STANDARD 17)

# Prevent GLFW from using Wayland
set(GLFW_BUILD_WAYLAND OFF)

# Enable warnings
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

add_subdirectory(Engine)

include(FetchContent)

# GLFW
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG        latest
)
FetchContent_MakeAvailable(glfw)

# GLAD
FetchContent_Declare(
  glad
  GIT_REPOSITORY https://github.com/Dav1dde/glad.git
  GIT_TAG        v2.0.8
)
FetchContent_MakeAvailable(glad)
