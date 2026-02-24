# Editor Components module CMakeLists.txt

# Add subdirectories first
add_subdirectory(assets)
add_subdirectory(viewport)

# Create the main Components library
add_library(VoltrayEditorComponents STATIC
    private/Console.cpp
    private/Dockspace.cpp
    private/Inspector.cpp
    private/Settings.cpp
    private/Toolbar.cpp
)

# Set include directories for this library
target_include_directories(VoltrayEditorComponents PUBLIC
    public
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
