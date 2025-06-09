# Editor Components module CMakeLists.txt

# Add subdirectories first
add_subdirectory(Assets)
add_subdirectory(Viewport)

# Create the main Components library
add_library(VoltrayEditorComponents STATIC
    Private/Console.cpp
    Private/Dockspace.cpp
    Private/Inspector.cpp
    Private/Settings.cpp
    Private/Toolbar.cpp
)

# Set include directories for this library
target_include_directories(VoltrayEditorComponents PUBLIC
    Public
    ${CMAKE_SOURCE_DIR}
    ${IMGUI_DIR}
    ${IMGUI_BACKEND_DIR}
)

# Link dependencies
target_link_libraries(VoltrayEditorComponents PUBLIC
    VoltrayMath
    VoltrayUtils
    VoltrayEngine
    VoltrayEditorComponentsAssets
    VoltrayEditorComponentsViewport
)

# Alias for consistent naming
add_library(Voltray::Editor::Components ALIAS VoltrayEditorComponents)
