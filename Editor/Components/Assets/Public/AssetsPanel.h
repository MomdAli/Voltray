#pragma once
#include "Panel.h"
#include "AssetItem.h"
#include "AssetManager.h"
#include "AssetBrowser.h"
#include "ContextMenu.h"
#include "NavigationBar.h"
#include "IconRenderer.h"
#include "AssetDragDrop.h"
#include <memory>

/**
 * @file Assets.h
 * @brief Defines the main Assets panel component for the Editor.
 */

namespace Voltray::Editor::Components::Assets
{

    /**
     * @class Assets
     * @brief A panel component responsible for managing and displaying asset resources.
     * @extends Panel
     */
    class AssetsPanel : public Panel
    {
    public:
        /**
         * @brief Renders the Assets panel content.
         * @details Implements the abstract Draw method from the Panel base class.
         */
        void Draw() override;

    private:
        // Core components
        AssetManager m_assetManager;
        IconRenderer m_iconRenderer;
        ContextMenu m_contextMenu;
        NavigationBar m_navigationBar;
        AssetDragDrop m_dragDrop;

        // Browser components for each tab
        std::unique_ptr<AssetBrowser> m_globalBrowser;
        std::unique_ptr<AssetBrowser> m_sceneBrowser;

        // UI state
        int m_selectedTab = 0; // 0 = Global Assets, 1 = Scene Assets
    };
}
