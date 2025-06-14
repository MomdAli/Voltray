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

# Doxygen Awesome CSS (for documentation styling)
FetchContent_Declare(
    doxygen-awesome-css
    URL https://github.com/jothepro/doxygen-awesome-css/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(doxygen-awesome-css)

# Save the location where doxygen-awesome-css was downloaded
FetchContent_GetProperties(doxygen-awesome-css)
if(NOT doxygen-awesome-css_POPULATED)
    FetchContent_Populate(doxygen-awesome-css)
    set(DOXYGEN_AWESOME_CSS_DIR ${doxygen-awesome-css_SOURCE_DIR})
endif()

# Setup ImGui sources
set(IMGUI_DIR ${imgui_SOURCE_DIR})
set(IMGUI_BACKEND_DIR ${IMGUI_DIR}/backends)
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_BACKEND_DIR}/imgui_impl_glfw.cpp    ${IMGUI_BACKEND_DIR}/imgui_impl_opengl3.cpp
)

# Find OpenGL before adding subdirectories that need it
find_package(OpenGL REQUIRED)

# Add subdirectories in order of dependencies
add_subdirectory(Math)
add_subdirectory(Utils)
add_subdirectory(Engine)
add_subdirectory(Editor)

# Create the main executable
add_executable(Voltray
    Main.cpp
    ${IMGUI_SOURCES}
    ${CMAKE_SOURCE_DIR}/Vendor/glad/src/gl.c
)

# Copy Resources to build output directory for runtime access
add_custom_command(TARGET Voltray POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/$<CONFIG>/Resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Resources ${CMAKE_BINARY_DIR}/$<CONFIG>/Resources
    COMMENT "Copying Resources folder to build output directory"
    VERBATIM
)

# Link all libraries to the main executable
target_link_libraries(Voltray PRIVATE
    VoltrayMath
    VoltrayUtils
    VoltrayEngine
    VoltrayEditor
    glfw
    OpenGL::GL
    assimp
    nlohmann_json::nlohmann_json
)

# Setup include directories for the main executable
target_include_directories(Voltray PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/Vendor/glad/include
    ${glfw_SOURCE_DIR}/include
    ${IMGUI_DIR}
    ${IMGUI_BACKEND_DIR}
    ${assimp_SOURCE_DIR}/include
)

# Add compiler options for main target
if(NOT WIN32)
    target_compile_options(Voltray PRIVATE
        -Wno-macro-redefined
        -Wno-unused-parameter
        -Wno-format
    )
endif()

# Documentation with Doxygen and Awesome CSS
find_package(Doxygen QUIET)
if(DOXYGEN_FOUND)
    # Configure Doxyfile with Doxygen Awesome CSS paths
    set(DOXYFILE_IN ${CMAKE_SOURCE_DIR}/Doxyfile)
    set(DOXYFILE_OUT ${CMAKE_BINARY_DIR}/Doxyfile)    # Replace placeholder with actual path to Doxygen Awesome CSS
    file(READ ${DOXYFILE_IN} DOXYFILE_CONTENT)
    string(REPLACE "@DOXYGEN_AWESOME_CSS_PATH@" "${doxygen-awesome-css_SOURCE_DIR}" DOXYFILE_CONTENT "${DOXYFILE_CONTENT}")
    string(REPLACE "@CMAKE_SOURCE_DIR@" "${CMAKE_SOURCE_DIR}" DOXYFILE_CONTENT "${DOXYFILE_CONTENT}")
    file(WRITE ${DOXYFILE_OUT} ${DOXYFILE_CONTENT})    # Create documentation target
    add_custom_target(voltray-docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Generating Voltray API documentation with sidebar-only theme"
        VERBATIM
        DEPENDS ${DOXYFILE_OUT} ${CMAKE_SOURCE_DIR}/docs/header.html ${CMAKE_SOURCE_DIR}/docs/footer.html ${CMAKE_SOURCE_DIR}/docs/voltray-custom.css
    )

    # Add a target to prepare docs for GitHub Pages (copies from docs/ to docs-github-pages)
    add_custom_target(voltray-docs-pages
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/docs-github-pages
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/docs ${CMAKE_SOURCE_DIR}/docs-github-pages
        COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_SOURCE_DIR}/docs-github-pages/.nojekyll
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Preparing documentation for GitHub Pages deployment"
        DEPENDS voltray-docs
    )

    message(STATUS "Doxygen found. Documentation target 'voltray-docs' available with custom Voltray theme.")
else()
    message(STATUS "Doxygen not found. Documentation target will not be available.")
endif()
