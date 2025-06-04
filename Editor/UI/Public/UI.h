#pragma once
#include <vector>
#include <memory>
#include "Panel.h"

/**
 * @file UI.h
 * @brief Defines the UI class for managing editor panels
 *
 * This file contains the declaration of the UI class which is responsible
 * for managing and rendering all UI panels within the editor.
 */

namespace Voltray::Editor
{
    /**
     * @class UI
     * @brief Manages all UI panels in the editor
     *
     * The UI class handles the collection and rendering of all editor panels.
     * It maintains ownership of panels through unique pointers and provides
     * methods to add new panels and draw all panels to the screen.
     */
    class UI
    {
    public:
        /**
         * @brief Adds a new panel to the UI
         * @param panel Unique pointer to a Panel object to be added
         *
         * Takes ownership of the provided panel and adds it to the collection
         * of panels managed by this UI instance.
         */
        void AddPanel(std::unique_ptr<Panel> panel);

        /**
         * @brief Renders all panels
         *
         * Iterates through all panels and calls their respective draw methods
         * to render them to the screen.
         */
        void Draw();

    private:
        std::vector<std::unique_ptr<Panel>> panels;
    };
}
