# Engine Loader module CMakeLists.txt
add_library(VoltrayEngineLoader STATIC
    Private/AssimpLoader.cpp
    Private/MeshLoader.cpp
)

# Set include directories for this library
target_include_directories(VoltrayEngineLoader PUBLIC
    Public
    ${CMAKE_SOURCE_DIR}
    ${assimp_SOURCE_DIR}/include
)

# Link dependencies
target_link_libraries(VoltrayEngineLoader PUBLIC
    VoltrayMath
    VoltrayUtils
    assimp
    VoltrayEngineGraphics
    VoltrayEditorComponents
)

# Alias for consistent naming
add_library(Voltray::Engine::Loader ALIAS VoltrayEngineLoader)
