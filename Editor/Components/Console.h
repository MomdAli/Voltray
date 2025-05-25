#pragma once
#include "../UI/Panel.h"

/**
 * @file Console.h
 * @brief Defines the Console panel component for the Voltray editor.
 */

namespace Editor::Components
{
    /**
     * @class Console
     * @brief A panel component that provides console functionality in the editor.
     * @extends Panel
     */
    class Console : public Panel
    {
    public:
        /**
         * @brief Renders the console panel interface.
         * @override Implements the abstract method from the Panel base class.
         */
        void Draw() override;
    };
}
