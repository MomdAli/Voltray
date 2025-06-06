# Editor UI module CMakeLists.txt

# Add subdirectories first
add_subdirectory(Panels)

# Create the main UI library
add_library(VoltrayEditorUI STATIC
    Private/Panel.cpp
    Private/Theme.cpp
    Private/UI.cpp
    Private/WorkspaceDialog.cpp
)

# Set include directories for this library
target_include_directories(VoltrayEditorUI PUBLIC
    Public
    ${CMAKE_SOURCE_DIR}
    ${IMGUI_DIR}
    ${IMGUI_BACKEND_DIR}
)

# Link dependencies
target_link_libraries(VoltrayEditorUI PUBLIC
    VoltrayMath
    VoltrayUtils
    VoltrayEditorUIPanels
)

# Alias for consistent naming
add_library(Voltray::Editor::UI ALIAS VoltrayEditorUI)
