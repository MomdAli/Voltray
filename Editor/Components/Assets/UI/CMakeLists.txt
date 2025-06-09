# Editor Components Assets UI module CMakeLists.txt
add_library(VoltrayEditorComponentsAssetsUI STATIC
    Private/AssetRenderer.cpp
    Private/AssetBrowserWidget.cpp
    Private/AssetsPanel.cpp
)

# Set include directories for this library
target_include_directories(VoltrayEditorComponentsAssetsUI PUBLIC
    Public
    ${CMAKE_SOURCE_DIR}
    ${IMGUI_DIR}
    ${IMGUI_BACKEND_DIR}
)

# Link dependencies
target_link_libraries(VoltrayEditorComponentsAssetsUI PUBLIC
    VoltrayMath
    VoltrayUtils
    VoltrayEditorComponentsAssetsCore
    VoltrayEditorComponentsAssetsDragDrop
)

# Alias for consistent naming
add_library(Voltray::Editor::Components::Assets::UI ALIAS VoltrayEditorComponentsAssetsUI)
