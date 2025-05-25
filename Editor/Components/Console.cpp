#include "Console.h"
#include <imgui.h>

namespace Editor::Components
{
    void Console::Draw()
    {
        ImGui::Begin("Console");
        ImGui::Text("Console output will be shown here.");
        ImGui::End();
    }
}
