# Engine Scene module CMakeLists.txt
add_library(VoltrayEngineScene STATIC
    private/PrimitiveGenerator.cpp
    private/Scene.cpp
    private/SceneObject.cpp
    private/SceneObjectFactory.cpp
)

# Set include directories for this library
target_include_directories(VoltrayEngineScene PUBLIC
    public
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
