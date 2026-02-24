# Utils module CMakeLists.txt
add_library(VoltrayUtils STATIC
    # Source files from private directory
    private/CrashLogger.cpp
    private/ResourceManager.cpp
    private/UserDataManager.cpp
    private/Workspace.cpp

    # Header files from public directory
    public/CrashLogger.h
    public/ResourceManager.h
    public/UserDataManager.h
    public/Workspace.h
)

# Link to Math module
target_link_libraries(VoltrayUtils PUBLIC
    VoltrayMath
    nlohmann_json::nlohmann_json
)

# Set include directories for this library
target_include_directories(VoltrayUtils PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/public
    ${CMAKE_SOURCE_DIR}
)

# Alias for consistent naming
add_library(Voltray::Utils ALIAS VoltrayUtils)
