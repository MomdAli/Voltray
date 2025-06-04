#pragma once
#include <imgui.h>
#include <filesystem>
#include <string>

/**
 * @file AssetDragDrop.h
 * @brief Handles drag and drop operations for assets into the viewport and other panels
 */

namespace Voltray::Editor::Components::Assets
{
    /**
     * @struct DragDropPayload
     * @brief Contains information about a dragged asset
     */
    struct DragDropPayload
    {
        std::filesystem::path assetPath;
        std::string assetType;
        std::string fileName;
        bool isGlobalAsset;

        DragDropPayload() = default;
        DragDropPayload(const std::filesystem::path &path, const std::string &type, bool global)
            : assetPath(path), assetType(type), fileName(path.filename().string()), isGlobalAsset(global) {}
    };

    /**
     * @class AssetDragDrop
     * @brief Manages drag and drop operations for assets
     */
    class AssetDragDrop
    {
    public:
        /**
         * @brief Begin a drag operation for an asset
         * @param assetPath Path to the asset being dragged
         * @param assetType Type of the asset (model, texture, script, etc.)
         * @param isGlobal Whether this is a global asset
         * @return True if drag operation started successfully
         */
        static bool BeginDrag(const std::filesystem::path &assetPath, const std::string &assetType, bool isGlobal);

        /**
         * @brief Check if there's an asset being dragged and handle drop
         * @return Pointer to payload if drop occurred, nullptr otherwise
         */
        static const DragDropPayload *AcceptDrop();

        /**
         * @brief Check if we can accept a drop at the current location
         * @param targetType Type of target (viewport, scene, etc.)
         * @return True if drop is valid
         */
        static bool CanAcceptDrop(const std::string &targetType);

        /**
         * @brief Get the current drag payload without consuming it
         * @return Pointer to payload if dragging, nullptr otherwise
         */
        static const DragDropPayload *GetDragPayload();

        /**
         * @brief Handle asset drop into viewport
         * @param payload The drag drop payload
         * @param dropPosition Screen position where drop occurred
         * @return True if drop was handled successfully
         */
        static bool HandleViewportDrop(const DragDropPayload &payload, const ImVec2 &dropPosition);

        /**
         * @brief Handle asset drop into scene hierarchy
         * @param payload The drag drop payload
         * @return True if drop was handled successfully
         */
        static bool HandleSceneDrop(const DragDropPayload &payload);

        /**
         * @brief Render drag preview overlay
         */
        static void RenderDragPreview();

    private:
        static constexpr const char *DRAG_DROP_ID = "ASSET_DRAG_DROP";
        static DragDropPayload s_currentPayload;
        static bool s_isDragging;

        /**
         * @brief Create a scene object from the dropped asset
         * @param payload The drag drop payload
         * @param position World position to place the object
         * @return True if object was created successfully
         */
        static bool CreateSceneObjectFromAsset(const DragDropPayload &payload, const ImVec2 &position);

        /**
         * @brief Load a 3D model asset into the scene
         * @param assetPath Path to the model file
         * @param position World position
         * @return True if model was loaded successfully
         */
        static bool LoadModelAsset(const std::filesystem::path &assetPath, const ImVec2 &position);

        /**
         * @brief Apply texture asset to selected object
         * @param assetPath Path to the texture file
         * @return True if texture was applied successfully
         */
        static bool ApplyTextureAsset(const std::filesystem::path &assetPath);

        /**
         * @brief Attach script asset to selected object
         * @param assetPath Path to the script file
         * @return True if script was attached successfully            */
        static bool AttachScriptAsset(const std::filesystem::path &assetPath);
    };
}
