# Engine Config module CMakeLists.txt
add_library(VoltrayEngineConfig STATIC
    EngineSettings.cpp
    EngineSettings.h
)

# Set include directories for this library
target_include_directories(VoltrayEngineConfig PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}
)

# Link dependencies (if any)
target_link_libraries(VoltrayEngineConfig PUBLIC
    VoltrayMath
    VoltrayUtils
)

# Alias for consistent naming
add_library(Voltray::Engine::Config ALIAS VoltrayEngineConfig)
