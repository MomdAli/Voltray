#include "AssetsPanelRefactored.h"
#include "EditorApp.h"
#include "UserDataManager.h"
#include <imgui.h>

namespace Voltray::Editor::Components::Assets
{
    AssetsPanelRefactored::AssetsPanelRefactored()
        : m_initialized(false)
    {
        // Components will be initialized on first Draw call
    }

    void AssetsPanelRefactored::Draw()
    {
        ImGui::Begin("Assets (Refactored)");

        // Initialize components on first use
        if (!m_initialized)
        {
            InitializeComponents();
        }

        if (!m_assetBrowserWidget)
        {
            ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "Failed to initialize asset browser");
            ImGui::End();
            return;
        }

        // Draw the asset browser widget
        ImVec2 availableRegion = ImGui::GetContentRegionAvail();
        m_assetBrowserWidget->Draw(availableRegion);

        ImGui::End();
    }

    void AssetsPanelRefactored::InitializeComponents()
    {
        try
        {
            // Get editor app for workspace management
            auto *editorApp = EditorApp::Get();
            if (!editorApp)
            {
                return;
            } // Create asset providers
            m_globalProvider = std::make_shared<GlobalAssetProvider>();
            m_localProvider = std::make_shared<LocalAssetProvider>();

            // Initialize providers
            std::string globalPath = Voltray::Utils::UserDataManager::GetGlobalAssetsDirectory().string();
            m_globalProvider->Initialize(globalPath);

            // Set local provider path based on current workspace
            auto currentWorkspace = editorApp->GetCurrentWorkspace();
            if (currentWorkspace)
            {
                m_localProvider->Initialize(currentWorkspace->path.string());
            }
            else
            {
                // Default to current directory if no workspace
                m_localProvider->Initialize(".");
            } // Create renderer and operations components
            m_iconRenderer = std::make_unique<IconRenderer>();
            m_renderer = std::make_unique<AssetRenderer>(*m_iconRenderer);
            m_operations = std::make_unique<AssetOperations>();
            m_dragDrop = std::make_unique<AssetDragDrop>();

            // Create the main asset browser widget - start with global provider
            m_assetBrowserWidget = std::make_unique<AssetBrowserWidget>(
                m_globalProvider,
                *m_renderer,
                *m_operations,
                *m_dragDrop);

            m_initialized = true;
        }
        catch (const std::exception &e)
        {
            // Log error or handle initialization failure
            m_initialized = false;
        }
    }
}
