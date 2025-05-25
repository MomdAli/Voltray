#pragma once
#include <memory>
#include <GLFW/glfw3.h>
#include "Components/Toolbar.h"
#include "Components/Viewport.h"
#include "Components/Inspector.h"
#include "Components/Assets.h"
#include "Components/Console.h"
#include "Components/Dockspace.h"

/**
 * @file EditorApp.h
 * @brief Defines the main application class for the Voltray Editor.
 *
 * This file contains the EditorApp class which serves as the central controller
 * for the editor application, managing various UI components and the application
 * lifecycle.
 */

namespace Editor
{
    /**
     * @class EditorApp
     * @brief Entry point and main controller for the Voltray Editor application.
     *
     * This class manages the lifecycle of the editor application and coordinates
     * all editor components. It follows a singleton pattern to ensure only one
     * instance exists throughout the application.
     */
    class EditorApp
    {
    public:
        /**
         * @brief Initializes the editor application with the given GLFW window.
         *
         * Sets up ImGui context, styles, and creates all UI components.
         *
         * @param window Pointer to the GLFW window to render the editor in.
         */
        void Init(GLFWwindow *window);

        /**
         * @brief Renders all editor UI components.
         *
         * Called every frame to render the editor interface including toolbar,
         * viewport, inspector, assets panel, and console.
         */
        void RenderUI();

        /**
         * @brief Cleans up resources used by the editor.
         *
         * Destroys ImGui context and releases any allocated resources.
         */
        void Shutdown();

        /**
         * @brief Returns the singleton instance of the EditorApp.
         *
         * @return Pointer to the EditorApp instance.
         */
        static EditorApp *Get();

    private:
        std::unique_ptr<Components::Toolbar> m_Toolbar;
        std::unique_ptr<Components::Viewport> m_Viewport;
        std::unique_ptr<Components::Inspector> m_Inspector;
        std::unique_ptr<Components::Assets> m_Assets;
        std::unique_ptr<Components::Console> m_Console;
        static EditorApp *s_Instance;
    };
}
