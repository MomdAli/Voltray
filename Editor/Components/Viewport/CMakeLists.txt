# Editor Components Viewport module CMakeLists.txt
add_library(VoltrayEditorComponentsViewport STATIC
    Viewport.cpp
    Viewport.h
    ViewportFramebuffer.cpp
    ViewportFramebuffer.h
    ViewportInput.cpp
    ViewportInput.h
    ViewportRenderer.cpp
    ViewportRenderer.h
    ViewportScene.cpp
    ViewportScene.h
)

# Set include directories for this library
target_include_directories(VoltrayEditorComponentsViewport PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
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
