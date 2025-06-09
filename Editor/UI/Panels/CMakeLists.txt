# Editor UI Panels module CMakeLists.txt
add_library(VoltrayEditorUIPanels STATIC
    Private/OptionsPanel.cpp
)

# Set include directories for this library
target_include_directories(VoltrayEditorUIPanels PUBLIC
    Public
    ${CMAKE_SOURCE_DIR}
    ${IMGUI_DIR}
    ${IMGUI_BACKEND_DIR}
)

# Link dependencies
target_link_libraries(VoltrayEditorUIPanels PUBLIC
    VoltrayMath
    VoltrayUtils
    VoltrayEditorUI
)

# Alias for consistent naming
add_library(Voltray::Editor::UI::Panels ALIAS VoltrayEditorUIPanels)
