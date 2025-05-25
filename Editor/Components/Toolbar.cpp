#include "Toolbar.h"
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
