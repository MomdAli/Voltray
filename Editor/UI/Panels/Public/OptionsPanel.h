#pragma once
#include "Panel.h"

namespace Voltray::Editor
{
    // Options/settings panel for the editor
    class OptionsPanel : public Panel
    {
    public:
        void Draw() override;
    };
} // namespace Voltray::Editor
