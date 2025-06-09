# Utils module CMakeLists.txt
add_library(VoltrayUtils STATIC
    # Source files from Private directory
    Private/CrashLogger.cpp
    Private/ResourceManager.cpp
    Private/UserDataManager.cpp
    Private/Workspace.cpp

    # Header files from Public directory
    Public/CrashLogger.h
    Public/ResourceManager.h
    Public/UserDataManager.h
    Public/Workspace.h
)

# Link to Math module
target_link_libraries(VoltrayUtils PUBLIC
    VoltrayMath
    nlohmann_json::nlohmann_json
)

# Set include directories for this library
target_include_directories(VoltrayUtils PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Public
    ${CMAKE_SOURCE_DIR}
)

# Alias for consistent naming
add_library(Voltray::Utils ALIAS VoltrayUtils)
