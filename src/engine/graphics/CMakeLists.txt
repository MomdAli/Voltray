# Engine Graphics module CMakeLists.txt

# Add subdirectories first
add_subdirectory(camera)

# Create the main Graphics library
add_library(VoltrayEngineGraphics STATIC
    private/IndexBuffer.cpp
    private/Mesh.cpp
    private/Renderer.cpp
    private/Shader.cpp
    private/VertexArray.cpp
    private/VertexBuffer.cpp
)

# Set include directories for this library
target_include_directories(VoltrayEngineGraphics PUBLIC
    public
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/Vendor/glad/include
    ${glfw_SOURCE_DIR}/include
)

# Link dependencies
target_link_libraries(VoltrayEngineGraphics PUBLIC
    VoltrayMath
    VoltrayUtils
    VoltrayEngineGraphicsCamera
    glfw
    OpenGL::GL
)

# Alias for consistent naming
add_library(Voltray::Engine::Graphics ALIAS VoltrayEngineGraphics)
