# Editor Components Viewport module CMakeLists.txt
add_library(VoltrayEditorComponentsViewport STATIC
    private/Viewport.cpp
    private/ViewportFramebuffer.cpp
    private/ViewportInput.cpp
    private/ViewportRenderer.cpp
    private/ViewportScene.cpp
)

# Set include directories for this library
target_include_directories(VoltrayEditorComponentsViewport PUBLIC
    public
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/Vendor/glad/include
    ${IMGUI_DIR}
    ${IMGUI_BACKEND_DIR}
    ${glfw_SOURCE_DIR}/include
)

# Link dependencies
target_link_libraries(VoltrayEditorComponentsViewport PUBLIC
    VoltrayMath
    VoltrayUtils
    VoltrayEngine
    glfw
    OpenGL::GL
)

# Alias for consistent naming
add_library(Voltray::Editor::Components::Viewport ALIAS VoltrayEditorComponentsViewport)
