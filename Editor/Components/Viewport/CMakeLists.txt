# Editor Components Viewport module CMakeLists.txt
add_library(VoltrayEditorComponentsViewport STATIC
    Private/Viewport.cpp
    Private/ViewportFramebuffer.cpp
    Private/ViewportInput.cpp
    Private/ViewportRenderer.cpp
    Private/ViewportScene.cpp
)

# Set include directories for this library
target_include_directories(VoltrayEditorComponentsViewport PUBLIC
    Public
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
