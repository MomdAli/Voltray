#include "OptionsPanel.h"
#include <imgui.h>

namespace Editor
{

    void OptionsPanel::Draw()
    {
        ImGui::Begin("Options");
        // TODO: Add controls to view and modify EngineSettings
        ImGui::Text("Engine options will go here.");
        ImGui::End();
    }

}
