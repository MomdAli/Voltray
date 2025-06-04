# Editor Components module CMakeLists.txt

# Add subdirectories first
add_subdirectory(Assets)
add_subdirectory(Viewport)

# Create the main Components library
add_library(VoltrayEditorComponents STATIC
    Console.cpp
    Console.h
    Dockspace.cpp
    Dockspace.h
    Inspector.cpp
    Inspector.h
    Settings.cpp
    Settings.h
    Toolbar.cpp
    Toolbar.h
)

# Set include directories for this library
target_include_directories(VoltrayEditorComponents PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
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
