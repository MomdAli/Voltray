#include "Assets.h"
#include <imgui.h>

namespace Editor::Components
{
    void Assets::Draw()
    {
        ImGui::Begin("Assets");
        ImGui::TextWrapped("Project assets will be shown here.");
        ImGui::End();
    }
}
