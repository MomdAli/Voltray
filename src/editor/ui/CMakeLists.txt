# Editor UI module CMakeLists.txt

# Add subdirectories first
add_subdirectory(panels)

# Create the main UI library
add_library(VoltrayEditorUI STATIC
    private/Panel.cpp
    private/Theme.cpp
    private/UI.cpp
    private/WorkspaceDialog.cpp
)

# Set include directories for this library
target_include_directories(VoltrayEditorUI PUBLIC
    public
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
