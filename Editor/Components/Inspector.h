#pragma once
#include "../UI/Panel.h"
#include <memory>

// Forward declarations
class SceneObject;

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

    private:
        /// @brief Draw properties for the selected object
        void DrawObjectProperties(std::shared_ptr<SceneObject> object);

        /// @brief Draw transform properties (position, rotation, scale)
        void DrawTransformProperties(std::shared_ptr<SceneObject> object);

        /// @brief Draw material and shader properties
        void DrawMaterialProperties(std::shared_ptr<SceneObject> object);

        /// @brief Draw object visibility and other basic properties
        void DrawObjectVisibility(std::shared_ptr<SceneObject> object);
    };
}
