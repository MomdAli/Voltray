# Editor Components Assets module CMakeLists.txt

# Add subdirectories first
add_subdirectory(Core)
add_subdirectory(DragDrop)
add_subdirectory(UI)
add_subdirectory(Utils)

# Create the main Assets library
add_library(VoltrayEditorComponentsAssets STATIC
    Private/Assets.cpp
    Private/AssetsPanel.cpp
)

# Set include directories for this library
target_include_directories(VoltrayEditorComponentsAssets PUBLIC
    Public
    ${CMAKE_SOURCE_DIR}
    ${IMGUI_DIR}
    ${IMGUI_BACKEND_DIR}
)

# Link dependencies
target_link_libraries(VoltrayEditorComponentsAssets PUBLIC
    VoltrayMath
    VoltrayUtils
    VoltrayEditorComponentsAssetsCore
    VoltrayEditorComponentsAssetsDragDrop
    VoltrayEditorComponentsAssetsUI
    VoltrayEditorComponentsAssetsUtils
)

# Alias for consistent naming
add_library(Voltray::Editor::Components::Assets ALIAS VoltrayEditorComponentsAssets)
