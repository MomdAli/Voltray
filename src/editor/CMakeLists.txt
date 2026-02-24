# Editor module CMakeLists.txt

# Add subdirectories first
add_subdirectory(components)
add_subdirectory(ui)

# Create the main Editor library
add_library(VoltrayEditor STATIC
    # Main Editor
    private/EditorApp.cpp
)

# Set include directories for this library
target_include_directories(VoltrayEditor PUBLIC
    public
    ${CMAKE_SOURCE_DIR}
    ${IMGUI_DIR}
    ${IMGUI_BACKEND_DIR}
    ${glfw_SOURCE_DIR}/include
)

# Link dependencies
target_link_libraries(VoltrayEditor PUBLIC
    VoltrayEngine
    VoltrayUtils
    VoltrayMath
    VoltrayEditorComponents
    VoltrayEditorUI
    glfw
    OpenGL::GL
)

# Set include directories for this library
target_include_directories(VoltrayEditor PUBLIC
    public
    ${CMAKE_SOURCE_DIR}
    ${IMGUI_DIR}
    ${IMGUI_BACKEND_DIR}
    ${glfw_SOURCE_DIR}/include
)

# Alias for consistent naming
add_library(Voltray::Editor ALIAS VoltrayEditor)
