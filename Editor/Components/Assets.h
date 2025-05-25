#pragma once
#include "../UI/Panel.h"

/**
 * @file Assets.h
 * @brief Defines the Assets panel component for the Editor.
 */

namespace Editor::Components
{
    /**
     * @class Assets
     * @brief A panel component responsible for managing and displaying asset resources.
     * @extends Panel
     */
    class Assets : public Panel
    {
    public:
        /**
         * @brief Renders the Assets panel content.
         * @details Implements the abstract Draw method from the Panel base class.
         */
        void Draw() override;
    };
}