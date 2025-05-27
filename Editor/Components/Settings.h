#pragma once
#include "../UI/Panel.h"

/**
 * @file Settings.h
 * @brief Defines the Settings panel component for the Voltray editor.
 */

namespace Editor::Components
{
    /**
     * @class Settings
     * @brief A panel component that provides settings interface in the editor.
     * @extends Panel
     */
    class Settings : public Panel
    {
    public:
        /**
         * @brief Renders the settings panel interface.
         * @override Implements the abstract method from the Panel base class.
         */
        void Draw() override;

    private:
        bool m_showGeneralSettings = true;
        bool m_showThemeSettings = true;
        bool m_showEditorSettings = true;
        bool m_showDebugSettings = true;
    };
}
