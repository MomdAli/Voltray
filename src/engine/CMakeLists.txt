# Engine module CMakeLists.txt

# Add subdirectories in order of dependencies
add_subdirectory(config)
add_subdirectory(input)
add_subdirectory(loader)
add_subdirectory(scene)
add_subdirectory(graphics)
add_subdirectory(animation)

# Create the main Engine library (interface library that links all modules)
add_library(VoltrayEngine INTERFACE)

# Link all Engine modules
target_link_libraries(VoltrayEngine INTERFACE
    VoltrayMath
    VoltrayUtils
    VoltrayEngineConfig
    VoltrayEngineInput
    VoltrayEngineLoader
    VoltrayEngineScene
    VoltrayEngineGraphics
)

# Set include directories for this library
target_include_directories(VoltrayEngine INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}
)

# Alias for consistent naming
add_library(Voltray::Engine ALIAS VoltrayEngine)
