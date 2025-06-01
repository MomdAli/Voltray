#pragma once
#include "../Core/AssetItem.h"
#include <imgui.h>

/**
 * @file IconRenderer.h
 * @brief Handles rendering of visual icons for different file types
 */

namespace Editor
{
    namespace Assets
    {
        /**
         * @class IconRenderer
         * @brief Renders visual icons using Dear ImGui drawing primitives
         */
        class IconRenderer
        {
        public:
            /**
             * @brief Render a file icon at the specified position
             * @param drawList ImGui draw list to render to
             * @param center Center position for the icon
             * @param size Size of the icon
             * @param extension File extension
             * @param isDirectory Whether it's a directory
             * @param isParentDir Whether it's a parent directory entry
             * @param isSelected Whether the item is selected
             */
            static void RenderFileIcon(
                ImDrawList *drawList,
                ImVec2 center,
                float size,
                const std::string &extension,
                bool isDirectory,
                bool isParentDir,
                bool isSelected = false);

            /**
             * @brief Render a small icon for list view
             * @param drawList ImGui draw list
             * @param center Center position
             * @param extension File extension
             * @param isDirectory Whether it's a directory
             */
            static void RenderSmallIcon(
                ImDrawList *drawList,
                ImVec2 center,
                const std::string &extension,
                bool isDirectory);

            /**
             * @brief Get the color scheme for a file type
             * @param extension File extension
             * @param isDirectory Whether it's a directory
             * @return Primary color for the file type
             */
            static ImU32 GetFileTypeColor(const std::string &extension, bool isDirectory);

        private:
            /**
             * @brief Render folder icon
             */
            static void RenderFolderIcon(ImDrawList *drawList, ImVec2 center, float size, bool isSelected);

            /**
             * @brief Render parent directory icon (up arrow)
             */
            static void RenderParentDirIcon(ImDrawList *drawList, ImVec2 center, float size, bool isSelected);

            /**
             * @brief Render 3D model icon
             */
            static void Render3DModelIcon(ImDrawList *drawList, ImVec2 center, float size, bool isSelected);

            /**
             * @brief Render image file icon
             */
            static void RenderImageIcon(ImDrawList *drawList, ImVec2 center, float size, bool isSelected);

            /**
             * @brief Render audio file icon
             */
            static void RenderAudioIcon(ImDrawList *drawList, ImVec2 center, float size, bool isSelected);

            /**
             * @brief Render text file icon
             */
            static void RenderTextIcon(ImDrawList *drawList, ImVec2 center, float size, bool isSelected);

            /**
             * @brief Render code file icon
             */
            static void RenderCodeIcon(ImDrawList *drawList, ImVec2 center, float size, bool isSelected);

            /**
             * @brief Render scene file icon
             */
            static void RenderSceneIcon(ImDrawList *drawList, ImVec2 center, float size, bool isSelected);

            /**
             * @brief Render material file icon
             */
            static void RenderMaterialIcon(ImDrawList *drawList, ImVec2 center, float size, bool isSelected);

            /**
             * @brief Render generic file icon
             */
            static void RenderGenericFileIcon(ImDrawList *drawList, ImVec2 center, float size, bool isSelected);

            // Color constants
            static constexpr ImU32 FOLDER_COLOR = IM_COL32(255, 200, 100, 255);
            static constexpr ImU32 PARENT_COLOR = IM_COL32(100, 150, 255, 255);
            static constexpr ImU32 MODEL_COLOR = IM_COL32(150, 150, 255, 255);
            static constexpr ImU32 IMAGE_COLOR = IM_COL32(100, 255, 150, 255);
            static constexpr ImU32 AUDIO_COLOR = IM_COL32(255, 150, 100, 255);
            static constexpr ImU32 TEXT_COLOR = IM_COL32(200, 200, 200, 255);
            static constexpr ImU32 CODE_COLOR = IM_COL32(100, 255, 100, 255);
            static constexpr ImU32 SCENE_COLOR = IM_COL32(255, 100, 255, 255);
            static constexpr ImU32 MATERIAL_COLOR = IM_COL32(150, 100, 200, 255);
            static constexpr ImU32 GENERIC_COLOR = IM_COL32(180, 180, 180, 255);
        };
    }
}
