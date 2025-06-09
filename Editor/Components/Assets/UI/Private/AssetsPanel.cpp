#include "AssetsPanel.h"
#include "EditorApp.h"
#include "UserDataManager.h"
#include <imgui.h>

namespace Voltray::Editor::Components::Assets
{
    AssetsPanel::AssetsPanel()
        : m_initialized(false)
    {
        // Components will be initialized on first Draw call
    }

    void AssetsPanel::Draw()
    {
        ImGui::Begin("Assets");

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

    void AssetsPanel::InitializeComponents()
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
            }

            std::string globalPath = Voltray::Utils::UserDataManager::GetGlobalAssetsDirectory().string();
            m_globalProvider->Initialize(globalPath);

            m_iconRenderer = std::make_unique<IconRenderer>();
            m_renderer = std::make_unique<AssetRenderer>(*m_iconRenderer);
            m_operations = std::make_unique<AssetOperations>();
            m_dragDrop = std::make_unique<AssetDragDrop>(); // Create the main asset browser widget with both providers
            m_assetBrowserWidget = std::make_unique<AssetBrowserWidget>(
                m_globalProvider,
                m_localProvider,
                *m_renderer,
                *m_operations,
                *m_dragDrop);

            m_initialized = true;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error initializing AssetsPanel: " << e.what() << std::endl;
            m_initialized = false;
        }
    }
    void AssetsPanel::OnWorkspaceChanged(const Voltray::Utils::Workspace &workspace)
    {
        if (!m_initialized)
        {
            InitializeComponents();
        }

        // Update local asset provider with new workspace path
        if (m_localProvider)
        {
            std::filesystem::path assetsPath = workspace.path / "Assets";
            m_localProvider->Initialize(assetsPath.string());
        }

        // Update the asset browser widget's local provider and switch to local view
        if (m_assetBrowserWidget)
        {
            m_assetBrowserWidget->UpdateLocalProvider(workspace.path);
            m_assetBrowserWidget->SetAssetView(AssetView::Local);
            m_assetBrowserWidget->Refresh();
        }
    }
}
