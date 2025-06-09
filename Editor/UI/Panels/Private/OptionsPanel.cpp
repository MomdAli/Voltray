#include "OptionsPanel.h"
#include <imgui.h>

namespace Voltray::Editor
{
    void OptionsPanel::Draw()
    {
        ImGui::Begin("Options");
        // TODO: Add controls to view and modify EngineSettings
        ImGui::TextWrapped("Engine options will go here.");
        ImGui::End();
    }

}
