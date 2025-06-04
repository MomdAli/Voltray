# Editor Components Assets Core module CMakeLists.txt
add_library(VoltrayEditorComponentsAssetsCore STATIC
    AssetItem.h
    AssetManager.cpp
    AssetManager.h
)

# Set include directories for this library
target_include_directories(VoltrayEditorComponentsAssetsCore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}
)

# Link dependencies
target_link_libraries(VoltrayEditorComponentsAssetsCore PUBLIC
    VoltrayMath
    VoltrayUtils
)

# Alias for consistent naming
add_library(Voltray::Editor::Components::Assets::Core ALIAS VoltrayEditorComponentsAssetsCore)
