#pragma once

#include <imgui.h>
#include <memory>

// Forward declarations
class BaseCamera;
class Scene;

namespace Editor::Components
{
    /**
     * @brief Handles input processing and object selection for the viewport
     */
    class ViewportInput
    {
    public:
        ViewportInput() = default;
        ~ViewportInput() = default;

        /**
         * @brief Process input for the viewport
         * @param scene Scene for object selection
         * @param camera Camera for ray casting
         * @param viewportPos Position of the viewport in screen space
         * @param viewportSize Size of the viewport
         */
        void ProcessInput(::Scene &scene, ::BaseCamera &camera, const ImVec2 &viewportPos, const ImVec2 &viewportSize);

    private:
        void handleObjectSelection(::Scene &scene, ::BaseCamera &camera, const ImVec2 &viewportPos, const ImVec2 &viewportSize);
        void handleCameraControls();

        bool isMouseInViewport(const ImVec2 &mousePos, const ImVec2 &viewportPos, const ImVec2 &viewportSize) const;
        ImVec2 getRelativeMousePosition(const ImVec2 &mousePos, const ImVec2 &viewportPos) const;
    };
}
