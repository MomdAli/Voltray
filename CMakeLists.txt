cmake_minimum_required(VERSION 3.17)
project(Voltray)

set(CMAKE_CXX_STANDARD 17)

# Prevent GLFW from using Wayland
set(GLFW_BUILD_WAYLAND OFF)

# Enable warnings for our code only, not dependencies
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
    # Don't treat warnings as errors for external dependencies
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        add_compile_options(-Wno-error=deprecated-copy)
    endif()
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        add_compile_options(-Wno-error=implicit-fallthrough)
    endif()
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

# Assimp (for mesh loading) - Optimized for faster builds
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "Disable Assimp tests")
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "Disable Assimp tools")
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "Disable Assimp samples")
set(ASSIMP_INSTALL OFF CACHE BOOL "Disable Assimp install")
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "Disable warnings as errors for Assimp")
set(ASSIMP_NO_EXPORT ON CACHE BOOL "Disable all exporters to speed up build")
set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "Build zlib from source")
set(BUILD_TESTING OFF CACHE BOOL "Disable testing")
set(ASSIMP_UNIT_TESTS OFF CACHE BOOL "Disable unit tests")
set(ASSIMP_HUNTER_ENABLED OFF CACHE BOOL "Disable Hunter package manager")

# Disable unnecessary importers to speed up build (keep only commonly used ones)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "Disable all importers by default")
set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "Enable OBJ importer")
set(ASSIMP_BUILD_FBX_IMPORTER ON CACHE BOOL "Enable FBX importer")
set(ASSIMP_BUILD_GLTF_IMPORTER ON CACHE BOOL "Enable GLTF importer")
set(ASSIMP_BUILD_COLLADA_IMPORTER ON CACHE BOOL "Enable COLLADA importer")
set(ASSIMP_BUILD_PLY_IMPORTER ON CACHE BOOL "Enable PLY importer")

FetchContent_Declare(
  assimp
  GIT_REPOSITORY https://github.com/assimp/assimp.git
  GIT_TAG        v5.3.1  # Use a more stable version
)
FetchContent_MakeAvailable(assimp)

# nlohmann/json (for workspace management)
FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG        v3.11.3
)
FetchContent_MakeAvailable(nlohmann_json)

# Apply platform-specific fixes for Assimp compilation issues
if(TARGET assimp)
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        # Fix macOS deprecated-copy warnings
        target_compile_options(assimp PRIVATE -Wno-deprecated-copy)
    endif()
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        # Fix Linux implicit-fallthrough warnings and array-bounds issues in GCC 13+
        target_compile_options(assimp PRIVATE
            -Wno-implicit-fallthrough
            -Wno-array-bounds
            -Wno-error=array-bounds
            -Wno-stringop-overflow
            -Wno-error=stringop-overflow
        )
    endif()
endif()

# Gather all source files from Engine, Editor, Math, and Utils (recursively, all .cpp and .h)
file(GLOB_RECURSE VOLTRAY_SRC CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/Engine/*.cpp"
    "${CMAKE_SOURCE_DIR}/Engine/*.h"
    "${CMAKE_SOURCE_DIR}/Editor/*.cpp"
    "${CMAKE_SOURCE_DIR}/Editor/*.h"
    "${CMAKE_SOURCE_DIR}/Math/*.cpp"
    "${CMAKE_SOURCE_DIR}/Math/*.h"
    "${CMAKE_SOURCE_DIR}/Utils/*.cpp"
    "${CMAKE_SOURCE_DIR}/Utils/*.h"
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
target_link_libraries(Voltray PRIVATE glfw OpenGL::GL assimp nlohmann_json::nlohmann_json)

# Collect all header directories from Engine, Editor, Math, and Utils recursively
file(GLOB_RECURSE ENGINE_HEADERS "${CMAKE_SOURCE_DIR}/Engine/*.h")
file(GLOB_RECURSE EDITOR_HEADERS "${CMAKE_SOURCE_DIR}/Editor/*.h")
file(GLOB_RECURSE MATH_HEADERS "${CMAKE_SOURCE_DIR}/Math/*.h")
file(GLOB_RECURSE UTILS_HEADERS "${CMAKE_SOURCE_DIR}/Utils/*.h")

set(ALL_HEADER_DIRS "")
foreach(header ${ENGINE_HEADERS} ${EDITOR_HEADERS} ${MATH_HEADERS} ${UTILS_HEADERS})
    get_filename_component(dir ${header} DIRECTORY)
    list(APPEND ALL_HEADER_DIRS ${dir})
endforeach()
list(REMOVE_DUPLICATES ALL_HEADER_DIRS)

# Add ImGui, glad, glfw, assimp, and root directories
list(APPEND ALL_HEADER_DIRS
    ${CMAKE_SOURCE_DIR}/Vendor/glad/include
    ${glfw_SOURCE_DIR}/include
    ${IMGUI_DIR}
    ${IMGUI_BACKEND_DIR}
    ${assimp_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/Engine
    ${CMAKE_SOURCE_DIR}/Editor
    ${CMAKE_SOURCE_DIR}/Math
    ${CMAKE_SOURCE_DIR}/Utils
)

target_include_directories(Voltray PRIVATE ${ALL_HEADER_DIRS})

# Add compiler options for main target
if(NOT WIN32)
    target_compile_options(Voltray PRIVATE
        -Wno-macro-redefined
        -Wno-unused-parameter
        -Wno-format
    )
endif()

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
