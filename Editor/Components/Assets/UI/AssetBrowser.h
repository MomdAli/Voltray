#pragma once

#include "../Core/AssetItem.h"
#include "../Core/AssetManager.h"
#include "ContextMenu.h"
#include "NavigationBar.h"
#include "../Utils/IconRenderer.h"
#include "../DragDrop/AssetDragDrop.h"
#include <string>
#include <memory>
#include <vector>
#include <imgui.h>

namespace Editor
{
    namespace Assets
    {

        /**
         * @brief Core asset browser component for displaying and managing assets
         *
         * This component handles the main file browser functionality including:
         * - Different view modes (Grid, List, Details)
         * - Asset item rendering with visual icons
         * - Search and filtering capabilities
         * - Directory navigation
         * - Integration with context menus and drag-drop
         */
        class AssetBrowser
        {
        public:
            /**
             * @brief Construct a new Asset Browser
             * @param assetManager Reference to the asset manager
             * @param iconRenderer Reference to the icon renderer
             * @param contextMenu Reference to the context menu system
             * @param navigationBar Reference to the navigation bar
             * @param dragDrop Reference to the drag and drop system
             */
            AssetBrowser(AssetManager &assetManager,
                         IconRenderer &iconRenderer,
                         ContextMenu &contextMenu,
                         NavigationBar &navigationBar,
                         AssetDragDrop &dragDrop);

            /**
             * @brief Draw the asset browser interface
             * @param windowSize Size of the containing window
             */
            void Draw(const ImVec2 &windowSize);

            /**
             * @brief Set the current directory path
             * @param path New directory path
             */
            void SetCurrentPath(const std::string &path);

            /**
             * @brief Get the current directory path
             * @return Current directory path
             */
            const std::string &GetCurrentPath() const { return m_currentPath; } /**
                                                                                 * @brief Set the view mode
                                                                                 * @param mode New view mode
                                                                                 */
            void SetViewMode(AssetViewMode mode) { m_viewMode = mode; }

            /**
             * @brief Get the current view mode
             * @return Current view mode
             */
            AssetViewMode GetViewMode() const { return m_viewMode; }

            /**
             * @brief Set the icon size
             * @param size New icon size (32-128 pixels)
             */
            void SetIconSize(float size);

            /**
             * @brief Get the current icon size
             * @return Current icon size
             */
            float GetIconSize() const { return m_iconSize; }

            /**
             * @brief Set the search filter
             * @param filter Search string
             */
            void SetSearchFilter(const std::string &filter);

            /**
             * @brief Get the current search filter
             * @return Current search filter
             */
            const std::string &GetSearchFilter() const { return m_searchFilter; }

            /**
             * @brief Refresh the current directory contents
             */
            void RefreshCurrentDirectory();

        private: // Core components
            AssetManager &m_assetManager;
            IconRenderer &m_iconRenderer;
            ContextMenu &m_contextMenu;
            NavigationBar &m_navigationBar;
            AssetDragDrop &m_dragDrop; // Browser state
            std::string m_currentPath;
            AssetViewMode m_viewMode = AssetViewMode::Grid;
            float m_iconSize = 64.0f;
            std::string m_searchFilter;

            // Cached data
            std::vector<AssetItem> m_currentItems;
            bool m_needsRefresh = true;

            // UI methods
            void RenderToolbar(float windowWidth);
            void RenderAssetGrid();
            void RenderAssetList();
            void RenderAssetDetails();
            void RenderAssetItem(const AssetItem &item, bool isGrid = true);

            // Navigation methods
            void NavigateToDirectory(const std::string &path);
            void OnDirectoryDoubleClick(const std::string &path);

            // Helper methods
            ImVec2 CalculateGridItemSize() const;
            float CalculateColumnsForGrid(float availableWidth) const;
            bool PassesSearchFilter(const AssetItem &item) const;
            void HandleItemSelection(const AssetItem &item);
            void HandleItemDoubleClick(const AssetItem &item);
        };
    }
}