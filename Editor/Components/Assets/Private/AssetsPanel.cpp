#include "AssetsPanel.h"
#include "EditorApp.h"
#include "UserDataManager.h"
#include <imgui.h>

namespace Voltray::Editor::Components::Assets
{

    void AssetsPanel::Draw()
    {
        ImGui::Begin("Assets");

        // Get current workspace from EditorApp
        auto *editorApp = EditorApp::Get();
        if (!editorApp)
        {
            ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "Editor not initialized");
            ImGui::End();
            return;
        }

        // Initialize components if needed
        static bool initialized = false;
        if (!initialized)
        {
            m_assetManager.Initialize(); // Initialize browser components
            m_globalBrowser = std::make_unique<AssetBrowser>(
                m_assetManager, m_iconRenderer, m_contextMenu, m_navigationBar, m_dragDrop);
            m_sceneBrowser = std::make_unique<AssetBrowser>(
                m_assetManager, m_iconRenderer, m_contextMenu, m_navigationBar, m_dragDrop); // Set initial paths
            m_globalBrowser->SetCurrentPath(Voltray::Utils::UserDataManager::GetGlobalAssetsDirectory().string());

            auto currentWorkspace = editorApp->GetCurrentWorkspace();
            if (currentWorkspace)
            {
                m_sceneBrowser->SetCurrentPath(currentWorkspace->path.string());
            }

            initialized = true;
        }

        // Tab bar for Global/Scene assets
        if (ImGui::BeginTabBar("AssetTabs"))
        {
            if (ImGui::BeginTabItem("Global Assets"))
            {
                if (m_globalBrowser)
                {
                    m_globalBrowser->Draw(ImGui::GetContentRegionAvail());
                }
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Scene Assets"))
            {
                auto currentWorkspace = editorApp->GetCurrentWorkspace();
                if (currentWorkspace)
                {
                    if (m_sceneBrowser)
                    {
                        // Update scene path if workspace changed
                        if (m_sceneBrowser->GetCurrentPath() != currentWorkspace->path.string())
                        {
                            m_sceneBrowser->SetCurrentPath(currentWorkspace->path.string());
                        }
                        m_sceneBrowser->Draw(ImGui::GetContentRegionAvail());
                    }
                }
                else
                {
                    ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.4f, 1.0f), "No workspace open");
                    if (ImGui::Button("Create Workspace"))
                    {
                        // TODO: Open workspace creation dialog
                    }
                }
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        ImGui::End();
    }

}
