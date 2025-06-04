# Utils module CMakeLists.txt
add_library(VoltrayUtils STATIC
    CrashLogger.cpp
    CrashLogger.h
    ResourceManager.cpp
    ResourceManager.h
    UserDataManager.cpp
    UserDataManager.h
    Workspace.cpp
    Workspace.h
)

# Link to Math module
target_link_libraries(VoltrayUtils PUBLIC
    VoltrayMath
)

# Set include directories for this library
target_include_directories(VoltrayUtils PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}
)

# Alias for consistent naming
add_library(Voltray::Utils ALIAS VoltrayUtils)
