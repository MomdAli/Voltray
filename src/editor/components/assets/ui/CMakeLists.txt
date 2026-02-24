# Editor Components Assets UI module CMakeLists.txt
add_library(VoltrayEditorComponentsAssetsUI STATIC
    private/AssetRenderer.cpp
    private/AssetBrowserWidget.cpp
    private/AssetsPanel.cpp
)

# Set include directories for this library
target_include_directories(VoltrayEditorComponentsAssetsUI PUBLIC
    public
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
