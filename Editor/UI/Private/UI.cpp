#include "UI.h"

namespace Voltray::Editor
{

    void UI::AddPanel(std::unique_ptr<Panel> panel)
    {
        panels.push_back(std::move(panel));
    }

    void UI::Draw()
    {
        for (auto &panel : panels)
        {
            panel->Draw();
        }
    }

}
