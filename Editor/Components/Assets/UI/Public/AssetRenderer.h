/**
 * @file AssetRenderer.h
 * @brief Handles rendering assets in different view modes
 * @author Voltray Engine Team
 * @date Created: 2025-06-06
 *
 * Responsible for rendering asset items in grid, list, and details view modes
 * with proper icons, context menus, and drag-drop support.
 */

#pragma once

#include "AssetItem.h"
#include "IconRenderer.h"
#include <vector>
#include <imgui.h>

namespace Voltray::Editor::Components::Assets
{
    /**
     * @class AssetRenderer
     * @brief Handles rendering of asset items in different view modes
     *
     * This class is responsible for drawing assets in the browser using
     * different view modes (Grid, List, Details) and handling visual feedback
     * for interactions like hover, selection, and drag operations.
     */
    class AssetRenderer
    {
    public:
        AssetRenderer(IconRenderer &iconRenderer);

        /**
         * @brief Render assets in grid view mode
         * @param items Asset items to render
         * @param iconSize Size of icons in pixels
         * @param availableWidth Available width for layout calculations
         * @return Selected item path (empty if none)
         */
        std::string RenderGridView(const std::vector<AssetItem> &items,
                                   float iconSize,
                                   float availableWidth);

        /**
         * @brief Render assets in list view mode
         * @param items Asset items to render
         * @return Selected item path (empty if none)
         */
        std::string RenderListView(const std::vector<AssetItem> &items);

        /**
         * @brief Render assets in details view mode
         * @param items Asset items to render
         * @return Selected item path (empty if none)
         */
        std::string RenderDetailsView(const std::vector<AssetItem> &items);

    private:
        IconRenderer &m_IconRenderer;

        /**
         * @brief Render a single asset item
         * @param item Asset item to render
         * @param isGrid Whether this is for grid view
         * @param iconSize Size of icon (for grid view)
         * @return True if item was clicked
         */
        bool RenderAssetItem(const AssetItem &item, bool isGrid, float iconSize = 64.0f);

        /**
         * @brief Calculate grid layout parameters
         * @param iconSize Icon size in pixels
         * @param availableWidth Available width
         * @return Number of columns that fit
         */
        int CalculateGridColumns(float iconSize, float availableWidth) const;

        /**
         * @brief Get appropriate text color for asset type
         * @param item Asset item
         * @return ImGui color for text
         */
        ImU32 GetAssetTextColor(const AssetItem &item) const;
    };
}
