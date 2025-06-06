#pragma once

#include <memory>
#include <GLFW/glfw3.h>

#include "Toolbar.h"
#include "Viewport.h"
#include "Inspector.h"
#include "AssetsPanelRefactored.h"
#include "Console.h"
#include "Settings.h"
#include "Dockspace.h"
#include "WorkspaceDialog.h"
#include "Workspace.h"

using Voltray::Editor::Components::Assets::AssetsPanelRefactored;

/**
 * @file EditorApp.h
 * @brief Defines the main application class for the Voltray Editor.
 *
 * This file contains the EditorApp class which serves as the central controller
 * for the editor application, managing various UI components and the application * lifecycle.
 */

namespace Voltray::Editor
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

        /**
         * @brief Get panel visibility flags for menu toggles
         */
        bool &GetViewportVisible() { return m_ViewportVisible; }
        bool &GetInspectorVisible() { return m_InspectorVisible; }
        bool &GetAssetsVisible() { return m_AssetsVisible; }
        bool &GetConsoleVisible() { return m_ConsoleVisible; }
        bool &GetSettingsVisible() { return m_SettingsVisible; } /**
                                                                  * @brief Get the viewport component
                                                                  * @return Pointer to the viewport component
                                                                  */
        Components::Viewport *GetViewport() { return m_Viewport.get(); }

        /**
         * @brief Get the settings component
         * @return Pointer to the settings component
         */
        Components::Settings *GetSettings() { return m_Settings.get(); }

        /**
         * @brief Get the current workspace
         * @return Shared pointer to the current workspace, or nullptr if none selected
         */
        std::shared_ptr<Voltray::Utils::Workspace> GetCurrentWorkspace() { return m_CurrentWorkspace; }

    private:
        std::unique_ptr<Components::Toolbar> m_Toolbar;
        std::unique_ptr<Components::Viewport> m_Viewport;
        std::unique_ptr<Components::Inspector> m_Inspector;
        std::unique_ptr<AssetsPanelRefactored> m_Assets;
        std::unique_ptr<Components::Settings> m_Settings;

        // Panel visibility flags
        bool m_ViewportVisible = true;
        bool m_InspectorVisible = true;
        bool m_AssetsVisible = true;
        bool m_ConsoleVisible = true;
        bool m_SettingsVisible = true;
        bool m_WorkspaceInitialized = false;
        bool m_ShowWorkspaceDialogOnStartup = true;
        std::shared_ptr<Voltray::Utils::Workspace> m_CurrentWorkspace;

        static EditorApp *s_Instance;
    };
}
