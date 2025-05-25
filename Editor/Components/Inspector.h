#pragma once
#include "../UI/Panel.h"

/// @brief Represents the Inspector panel component in the Editor.
///
/// The Inspector class is responsible for rendering and managing the inspector panel,
/// which displays and allows editing of properties for selected objects within the editor.
/// Inherits from the Panel base class.
namespace Editor::Components
{
    class Inspector : public Panel
    {
    public:
        void Draw() override;
    };
}
