
#pragma once

/**
 * @file /c:/Users/mohsa/Github/Voltray/Editor/UI/Panel.h
 * @brief Contains the base class for editor UI panels.
 */

namespace Voltray::Editor
{
    /**
     * @class Panel
     * @brief Base class for all UI panels in the editor.
     *
     * This abstract class defines the interface for UI panels in the editor.
     * All panel types should inherit from this class and implement the Draw method.
     */
    class Panel
    {
    public:
        /**
         * @brief Virtual destructor for proper cleanup of derived classes.
         */
        virtual ~Panel() = default;

        /**
         * @brief Pure virtual function to render the panel.
         *
         * This method must be implemented by all derived panel classes to
         * handle their specific rendering logic.
         */
        virtual void Draw() = 0;
    };
}
