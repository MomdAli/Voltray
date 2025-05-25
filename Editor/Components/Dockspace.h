/**
 * @file Dockspace.h
 * @brief Defines the Dockspace component for managing ImGui dock layouts and panels
 *
 * The Dockspace class provides functionality for:
 * - Managing UI panels and their docking regions
 * - Controlling the ImGui dockspace lifecycle
 * - Persisting and restoring docking layouts
 *
 * It serves as a central manager for organizing the editor's UI panels
 * into a dockable interface with predefined regions (Left, Right, Bottom, Top, Center).
 */
#pragma once
#include <vector>
#include <utility>
#include <array>
#include "../UI/Panel.h"
#include <imgui.h>

namespace Editor::Components
{
    /**
     * @class Dockspace
     * @brief Manages the dockable layout system for the editor interface.
     *
     * The Dockspace class provides functionality to organize and manage editor panels
     * in a dockable interface. It allows panels to be registered, arranged in specific
     * regions, and handles saving/loading of layout configurations.
     */
    class Dockspace

    {
    public:
        /**
         * @enum Dockspace::Region
         * @brief Defines possible docking regions within the editor interface.
         *
         * @var Region::Left
         * Left side of the editor interface
         * @var Region::Right
         * Right side of the editor interface
         * @var Region::Bottom
         * Bottom area of the editor interface
         * @var Region::Top
         * Top area of the editor interface
         * @var Region::Center
         * Central area of the editor interface
         */
        enum class Region
        {
            Left = 0,
            Right,
            Bottom,
            Top,
            Center
        };
        /**
         * @brief Register a panel to be managed by the dockspace.
         *
         * @param name The display name of the panel
         * @param panel Pointer to the panel to register
         * @param defaultRegion The default region where the panel should be docked
         */
        static void RegisterPanel(const char *name, Editor::Panel *panel, Region defaultRegion);

        /**
         * @brief Initialize and begin a new dockspace frame.
         *
         * Must be called at the beginning of each frame before rendering any panels.
         */
        static void Begin();

        /**
         * @brief Finalize the dockspace for the current frame.
         *
         * Must be called at the end of each frame after all panels have been rendered.
         */
        static void End();

        /**
         * @brief Save the current dockspace layout configuration to a file.
         *
         * @param filename Path to the file where layout settings will be saved
         */
        static void SaveLayout(const char *filename);

        /**
         * @brief Load a previously saved dockspace layout configuration from a file.
         *
         * @param filename Path to the file containing layout settings to load
         */
        static void LoadLayout(const char *filename);

        /**
         * @brief Reset the dockspace layout to default settings on the next Begin() call.
         *
         * This will clear all current panel arrangements and force a rebuild
         * of the default layout on the next Begin() call.
         */
        static void Reset();

    private:
        struct PanelInfo
        {
            const char *name;
            Editor::Panel *panel;
            Region region;
        };
        static bool s_Initialized;
        static std::vector<PanelInfo> s_Panels;
        // ImGuiIDs for each region in the order of Region enum
        static std::array<ImGuiID, 5> s_Regions;
    };
}
