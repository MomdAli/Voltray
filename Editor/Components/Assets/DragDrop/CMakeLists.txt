# Editor Components Assets DragDrop module CMakeLists.txt
add_library(VoltrayEditorComponentsAssetsDragDrop STATIC
    Private/AssetDragDrop.cpp
)

# Set include directories for this library
target_include_directories(VoltrayEditorComponentsAssetsDragDrop PUBLIC
    Public
    ${CMAKE_SOURCE_DIR}
    ${IMGUI_DIR}
    ${IMGUI_BACKEND_DIR}
)

# Link dependencies
target_link_libraries(VoltrayEditorComponentsAssetsDragDrop PUBLIC
    VoltrayMath
    VoltrayUtils
    VoltrayEditor
)

# Alias for consistent naming
add_library(Voltray::Editor::Components::Assets::DragDrop ALIAS VoltrayEditorComponentsAssetsDragDrop)
