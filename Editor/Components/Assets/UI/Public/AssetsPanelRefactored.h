#pragma once
#include "Panel.h"
#include "AssetBrowserWidget.h"
#include "AssetProvider.h"
#include "GlobalAssetProvider.h"
#include "LocalAssetProvider.h"
#include "AssetRenderer.h"
#include "AssetOperations.h"
#include "IconRenderer.h"
#include "AssetDragDrop.h"
#include <memory>

/**
 * @file AssetsPanelRefactored.h
 * @brief Defines the new refactored Assets panel component for the Editor.
 */

namespace Voltray::Editor::Components::Assets
{
    /**
     * @class AssetsPanelRefactored
     * @brief A panel component responsible for managing and displaying asset resources using the new clean architecture.
     * @extends Panel
     *
     * This panel uses the new refactored asset browser system with separated responsibilities:
     * - AssetProvider implementations for different asset sources
     * - AssetRenderer for view mode rendering
     * - AssetOperations for file operations
     * - AssetBrowserWidget as the main UI coordinator
     */
    class AssetsPanelRefactored : public Panel
    {
    public:
        /**
         * @brief Construct the refactored assets panel
         */
        AssetsPanelRefactored();

        /**
         * @brief Renders the Assets panel content.
         * @details Implements the abstract Draw method from the Panel base class.
         */
        void Draw() override;

    private:
        // Asset browser widget (main UI coordinator)
        std::unique_ptr<AssetBrowserWidget> m_assetBrowserWidget; // Asset providers
        std::shared_ptr<GlobalAssetProvider> m_globalProvider;
        std::shared_ptr<LocalAssetProvider> m_localProvider;

        // UI components
        std::unique_ptr<IconRenderer> m_iconRenderer;
        std::unique_ptr<AssetRenderer> m_renderer;
        std::unique_ptr<AssetOperations> m_operations;
        std::unique_ptr<AssetDragDrop> m_dragDrop;

        // Initialization flag
        bool m_initialized = false;

        /**
         * @brief Initialize all components on first use
         */
        void InitializeComponents();
    };
}
