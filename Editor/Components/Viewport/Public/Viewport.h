#pragma once

#include <imgui.h>
#include <memory>
#include "Panel.h"
#include "ViewportFramebuffer.h"
#include "ViewportRenderer.h"
#include "ViewportInput.h"
#include "ViewportScene.h"

namespace Voltray::Editor::Components
{
    /**
     * @brief Main viewport component for 3D scene editing
     *
     * This class has been refactored to use focused utility classes:
     * - ViewportFramebuffer: Manages OpenGL framebuffer operations
     * - ViewportRenderer: Handles scene rendering
     * - ViewportInput: Processes input and object selection
     * - ViewportScene: Manages scene initialization and lifecycle
     */
    class Viewport : public Panel
    {
    public:
        Viewport();
        ~Viewport() = default;

        void Draw() override;

        /**
         * @brief Get the viewport scene component
         * @return Reference to the ViewportScene
         */
        ViewportScene &GetScene() { return m_Scene; }

    private:
        void initialize();
        bool isInitialized() const;

        // Utility classes for focused responsibilities
        ViewportFramebuffer m_Framebuffer;
        ViewportRenderer m_Renderer;
        ViewportInput m_Input;
        ViewportScene m_Scene;

        // Initialization state
        bool m_Initialized = false;
    };
}
