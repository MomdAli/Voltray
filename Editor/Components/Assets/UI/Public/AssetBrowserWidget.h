/**
 * @file AssetBrowserWidget.h
 * @brief Main asset browser UI widget
 * @author Voltray Engine Team
 * @date Created: 2025-06-06
 *
 * The main asset browser component that combines asset providers,
 * rendering, and operations into a cohesive user interface.
 */

#pragma once

#include "AssetProvider.h"
#include "AssetRenderer.h"
#include "AssetOperations.h"
#include "AssetDragDrop.h"
#include "AssetItem.h"
#include <memory>
#include <string>
#include <imgui.h>

namespace Voltray::Editor::Components::Assets
{
    /**
     * @enum AssetView
     * @brief Different types of asset views
     */
    enum class AssetView
    {
        Global, ///< Global assets (shared across all projects)
        Local,  ///< Local workspace assets
        Scene   ///< Scene-specific assets
    };

    /**
     * @class AssetBrowserWidget
     * @brief Main asset browser user interface component
     *
     * This is the primary widget that users interact with. It coordinates
     * between asset providers, rendering, operations, and navigation to
     * provide a complete asset browsing experience.
     */
    class AssetBrowserWidget
    {
    public:
        AssetBrowserWidget(std::shared_ptr<AssetProvider> provider,
                           AssetRenderer &renderer,
                           AssetOperations &operations,
                           AssetDragDrop &dragDrop);

        /**
         * @brief Draw the asset browser interface
         * @param availableSize Available space for the widget
         */
        void Draw(const ImVec2 &availableSize);

        /**
         * @brief Set the current directory
         * @param directory Directory path
         */
        void SetCurrentDirectory(const std::filesystem::path &directory);

        /**
         * @brief Get the current directory
         * @return Current directory path
         */
        const std::filesystem::path &GetCurrentDirectory() const { return m_CurrentDirectory; }

        /**
         * @brief Set view mode
         * @param mode New view mode
         */
        void SetViewMode(AssetViewMode mode) { m_ViewMode = mode; }

        /**
         * @brief Get current view mode
         * @return Current view mode
         */
        AssetViewMode GetViewMode() const { return m_ViewMode; }

        /**
         * @brief Set icon size for grid view
         * @param size Icon size in pixels (32-128)
         */
        void SetIconSize(float size);

        /**
         * @brief Get current icon size
         * @return Icon size in pixels
         */
        float GetIconSize() const { return m_IconSize; }

        /**
         * @brief Refresh current directory contents
         */
        void Refresh();

        /**
         * @brief Set the current asset view (global, local, scene)
         * @param view New asset view type
         */
        void SetAssetView(AssetView view);

        /**
         * @brief Get the current asset view
         * @return Current asset view type
         */
        AssetView GetAssetView() const;

    private:
        std::shared_ptr<AssetProvider> m_Provider;
        AssetRenderer &m_Renderer;
        AssetOperations &m_Operations;
        AssetDragDrop &m_DragDrop;

        // Navigation state
        std::filesystem::path m_CurrentDirectory;
        std::vector<AssetItem> m_CurrentItems;
        bool m_NeedsRefresh = true; // View settings
        AssetViewMode m_ViewMode = AssetViewMode::Grid;
        float m_IconSize = 64.0f;
        std::string m_SearchFilter;
        bool m_RecursiveSearch = false;

        // Asset view tracking
        AssetView m_CurrentAssetView = AssetView::Global;
        std::shared_ptr<AssetProvider> m_GlobalProvider;
        std::shared_ptr<AssetProvider> m_LocalProvider;

        /**
         * @brief Render the toolbar with view controls and search
         */
        void RenderToolbar();

        /**
         * @brief Render the navigation breadcrumb
         */
        void RenderNavigation();

        /**
         * @brief Handle directory navigation
         * @param path New directory path
         */
        void NavigateTo(const std::filesystem::path &path);

        /**
         * @brief Filter items based on search criteria
         * @param items Input items
         * @return Filtered items
         */
        std::vector<AssetItem> FilterItems(const std::vector<AssetItem> &items) const;

        /**
         * @brief Handle item selection and double-click
         * @param selectedPath Path of selected item (empty if none)
         */
        void HandleItemInteraction(const std::string &selectedPath);
    };
}
