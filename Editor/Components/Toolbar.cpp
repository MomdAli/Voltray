#include "Toolbar.h"
#include "../EditorApp.h"

#include <glad/gl.h>
#include <imgui.h>

namespace Editor::Components
{
    void Toolbar::Draw()
    {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                ImGui::MenuItem("New Project");
                ImGui::MenuItem("Open...");
                ImGui::MenuItem("Save");
                ImGui::Separator();
                ImGui::MenuItem("Exit");
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit"))
            {
                ImGui::MenuItem("Undo");
                ImGui::MenuItem("Redo");
                ImGui::Separator();
                ImGui::MenuItem("Cut");
                ImGui::MenuItem("Copy");
                ImGui::MenuItem("Paste");
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("View"))
            {
                // Get EditorApp instance to access panel visibility flags
                auto *editorApp = ::Editor::EditorApp::Get();
                if (editorApp)
                {
                    ImGui::MenuItem("Viewport", nullptr, &editorApp->GetViewportVisible());
                    ImGui::MenuItem("Inspector", nullptr, &editorApp->GetInspectorVisible());
                    ImGui::MenuItem("Assets", nullptr, &editorApp->GetAssetsVisible());
                    ImGui::MenuItem("Console", nullptr, &editorApp->GetConsoleVisible());
                    ImGui::MenuItem("Settings", nullptr, &editorApp->GetSettingsVisible());
                    ImGui::Separator();
                }
                ImGui::MenuItem("Reset Layout");

                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Help"))
            {
                ImGui::MenuItem("About");
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }
}
