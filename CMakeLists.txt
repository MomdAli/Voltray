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

# ImGui (for Editor UI)
FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG        docking
)
FetchContent_MakeAvailable(imgui)

# Assimp (for mesh loading)
FetchContent_Declare(
  assimp
  GIT_REPOSITORY https://github.com/assimp/assimp.git
  GIT_TAG        v5.4.3
)
set(ASSIMP_BUILD_TESTS OFF)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
set(ASSIMP_BUILD_SAMPLES OFF)
set(ASSIMP_INSTALL OFF)
FetchContent_MakeAvailable(assimp)

# Gather all source files from Engine and Editor (recursively, all .cpp and .h)
file(GLOB_RECURSE VOLTRAY_SRC CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/Engine/*.cpp"
    "${CMAKE_SOURCE_DIR}/Engine/*.h"
    "${CMAKE_SOURCE_DIR}/Editor/*.cpp"
    "${CMAKE_SOURCE_DIR}/Editor/*.h"
)

# Add Main.cpp from project root
list(APPEND VOLTRAY_SRC ${CMAKE_SOURCE_DIR}/Main.cpp)

# ImGui sources (use FetchContent's imgui_SOURCE_DIR)
set(IMGUI_DIR ${imgui_SOURCE_DIR})
set(IMGUI_BACKEND_DIR ${IMGUI_DIR}/backends)
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_BACKEND_DIR}/imgui_impl_glfw.cpp
    ${IMGUI_BACKEND_DIR}/imgui_impl_opengl3.cpp
)

add_executable(Voltray ${VOLTRAY_SRC} ${IMGUI_SOURCES} ${CMAKE_SOURCE_DIR}/Vendor/glad/src/gl.c)

# After including dependencies, find OpenGL
find_package(OpenGL REQUIRED)

# Link GLFW and OpenGL to the Voltray executable
target_link_libraries(Voltray PRIVATE glfw OpenGL::GL assimp)

# Collect all header directories from Engine and Editor recursively
file(GLOB_RECURSE ENGINE_HEADERS "${CMAKE_SOURCE_DIR}/Engine/*.h")
file(GLOB_RECURSE EDITOR_HEADERS "${CMAKE_SOURCE_DIR}/Editor/*.h")

set(ALL_HEADER_DIRS "")
foreach(header ${ENGINE_HEADERS} ${EDITOR_HEADERS})
    get_filename_component(dir ${header} DIRECTORY)
    list(APPEND ALL_HEADER_DIRS ${dir})
endforeach()
list(REMOVE_DUPLICATES ALL_HEADER_DIRS)

# Add ImGui, glad, glfw, assimp, and root Engine/Editor include dirs
list(APPEND ALL_HEADER_DIRS
    ${CMAKE_SOURCE_DIR}/Vendor/glad/include
    ${glfw_SOURCE_DIR}/include
    ${IMGUI_DIR}
    ${IMGUI_BACKEND_DIR}
    ${assimp_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/Engine
    ${CMAKE_SOURCE_DIR}/Editor
)

target_include_directories(Voltray PRIVATE ${ALL_HEADER_DIRS})


find_package(Doxygen QUIET)
if(DOXYGEN_FOUND)
    add_custom_target(voltray-docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
else()
    message(STATUS "Doxygen not found. Documentation target will not be available.")
endif()
