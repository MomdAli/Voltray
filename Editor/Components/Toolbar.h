#pragma once
#include "../UI/Panel.h"

/**
 * @brief Represents the toolbar panel component in the editor.
 *
 * The Toolbar class is a specialized Panel that provides
 * toolbar functionality within the editor UI.
 */
namespace Editor::Components
{
    class Toolbar : public Panel
    {
    public:
        void Draw() override;
    };
}
