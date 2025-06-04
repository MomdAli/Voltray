# Engine Scene module CMakeLists.txt
add_library(VoltrayEngineScene STATIC
    PrimitiveGenerator.cpp
    PrimitiveGenerator.h
    Scene.cpp
    Scene.h
    SceneObject.cpp
    SceneObject.h
    SceneObjectFactory.cpp
    SceneObjectFactory.h
)

# Set include directories for this library
target_include_directories(VoltrayEngineScene PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}
)

# Link dependencies
target_link_libraries(VoltrayEngineScene PUBLIC
    VoltrayMath
    VoltrayUtils
)

# Alias for consistent naming
add_library(Voltray::Engine::Scene ALIAS VoltrayEngineScene)
