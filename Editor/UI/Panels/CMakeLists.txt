# Editor UI Panels module CMakeLists.txt
add_library(VoltrayEditorUIPanels STATIC
    OptionsPanel.cpp
    OptionsPanel.h
)

# Set include directories for this library
target_include_directories(VoltrayEditorUIPanels PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}
    ${IMGUI_DIR}
    ${IMGUI_BACKEND_DIR}
)

# Link dependencies
target_link_libraries(VoltrayEditorUIPanels PUBLIC
    VoltrayMath
    VoltrayUtils
)

# Alias for consistent naming
add_library(Voltray::Editor::UI::Panels ALIAS VoltrayEditorUIPanels)
