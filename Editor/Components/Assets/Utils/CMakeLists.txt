# Editor Components Assets Utils module CMakeLists.txt
add_library(VoltrayEditorComponentsAssetsUtils STATIC
    Private/AssetOperations.cpp
    Private/FileOperations.cpp
    Private/IconRenderer.cpp
)

# Set include directories for this library
target_include_directories(VoltrayEditorComponentsAssetsUtils PUBLIC
    Public
    ${CMAKE_SOURCE_DIR}
    ${IMGUI_DIR}
    ${IMGUI_BACKEND_DIR}
)

# Link dependencies
target_link_libraries(VoltrayEditorComponentsAssetsUtils PUBLIC
    VoltrayMath
    VoltrayUtils
    VoltrayEditorComponentsAssetsCore
)

# Alias for consistent naming
add_library(Voltray::Editor::Components::Assets::Utils ALIAS VoltrayEditorComponentsAssetsUtils)
