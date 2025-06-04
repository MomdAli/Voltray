# Editor Components Assets Utils module CMakeLists.txt
add_library(VoltrayEditorComponentsAssetsUtils STATIC
    FileOperations.cpp
    FileOperations.h
    IconRenderer.cpp
    IconRenderer.h
)

# Set include directories for this library
target_include_directories(VoltrayEditorComponentsAssetsUtils PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}
    ${IMGUI_DIR}
    ${IMGUI_BACKEND_DIR}
)

# Link dependencies
target_link_libraries(VoltrayEditorComponentsAssetsUtils PUBLIC
    VoltrayMath
    VoltrayUtils
)

# Alias for consistent naming
add_library(Voltray::Editor::Components::Assets::Utils ALIAS VoltrayEditorComponentsAssetsUtils)
