# Engine Loader module CMakeLists.txt
add_library(VoltrayEngineLoader STATIC
    AssimpLoader.cpp
    IFormatLoader.h
    MeshLoader.cpp
    MeshLoader.h
)

# Set include directories for this library
target_include_directories(VoltrayEngineLoader PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}
    ${assimp_SOURCE_DIR}/include
)

# Link dependencies
target_link_libraries(VoltrayEngineLoader PUBLIC
    VoltrayMath
    VoltrayUtils
    assimp
)

# Alias for consistent naming
add_library(Voltray::Engine::Loader ALIAS VoltrayEngineLoader)
