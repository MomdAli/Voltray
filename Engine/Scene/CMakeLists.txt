# Engine Scene module CMakeLists.txt
add_library(VoltrayEngineScene STATIC
    Private/PrimitiveGenerator.cpp
    Private/Scene.cpp
    Private/SceneObject.cpp
    Private/SceneObjectFactory.cpp
)

# Set include directories for this library
target_include_directories(VoltrayEngineScene PUBLIC
    Public
    ${CMAKE_SOURCE_DIR}
)

# Link dependencies
target_link_libraries(VoltrayEngineScene PUBLIC
    VoltrayMath
    VoltrayUtils
    VoltrayEngineGraphics
    VoltrayEngineLoader
    VoltrayEditorComponents
)

# Alias for consistent naming
add_library(Voltray::Engine::Scene ALIAS VoltrayEngineScene)
