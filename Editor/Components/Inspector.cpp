#include "Inspector.h"
#include <imgui.h>

namespace Editor::Components
{
    void Inspector::Draw()
    {
        // Only set window position and size when not docked
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 4.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.13f, 0.14f, 0.18f, 1.0f)); // Subtle dark

        // Using a visible boolean to make sure the window is shown
        bool visible = true;
        ImGui::Begin("Inspector", &visible, flags);
        ImGui::PopStyleColor();
        ImGui::PopStyleVar(2);

        ImGui::Text("Inspector / Properties");
        ImGui::Separator();
        ImGui::Spacing();

        // Add inspector logic here

        ImGui::End();
    }
}
