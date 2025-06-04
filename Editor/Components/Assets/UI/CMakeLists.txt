# Editor Components Assets UI module CMakeLists.txt
add_library(VoltrayEditorComponentsAssetsUI STATIC
    AssetBrowser.cpp
    AssetBrowser.h
    ContextMenu.cpp
    ContextMenu.h
    NavigationBar.cpp
    NavigationBar.h
)

# Set include directories for this library
target_include_directories(VoltrayEditorComponentsAssetsUI PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}
    ${IMGUI_DIR}
    ${IMGUI_BACKEND_DIR}
)

# Link dependencies
target_link_libraries(VoltrayEditorComponentsAssetsUI PUBLIC
    VoltrayMath
    VoltrayUtils
)

# Alias for consistent naming
add_library(Voltray::Editor::Components::Assets::UI ALIAS VoltrayEditorComponentsAssetsUI)
