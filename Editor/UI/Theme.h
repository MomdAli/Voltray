#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include <imgui.h>

/**
 * @file Theme.h
 * @brief Defines the theme system for the Voltray editor.
 */

namespace Editor::UI
{
    /**
     * @struct ThemeColors
     * @brief Contains all color definitions for a theme
     */
    struct ThemeColors
    {
        // Window colors
        ImVec4 WindowBg = ImVec4(0.06f, 0.06f, 0.06f, 1.0f);
        ImVec4 ChildBg = ImVec4(0.00f, 0.00f, 0.00f, 0.0f);
        ImVec4 PopupBg = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);

        // Border colors
        ImVec4 Border = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
        ImVec4 BorderShadow = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

        // Frame colors
        ImVec4 FrameBg = ImVec4(0.16f, 0.29f, 0.48f, 0.54f);
        ImVec4 FrameBgHovered = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
        ImVec4 FrameBgActive = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);

        // Title colors
        ImVec4 TitleBg = ImVec4(0.04f, 0.04f, 0.04f, 1.0f);
        ImVec4 TitleBgActive = ImVec4(0.16f, 0.29f, 0.48f, 1.0f);
        ImVec4 TitleBgCollapsed = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);

        // Menu colors
        ImVec4 MenuBarBg = ImVec4(0.14f, 0.14f, 0.14f, 1.0f);

        // Scrollbar colors
        ImVec4 ScrollbarBg = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
        ImVec4 ScrollbarGrab = ImVec4(0.31f, 0.31f, 0.31f, 1.0f);
        ImVec4 ScrollbarGrabHovered = ImVec4(0.41f, 0.41f, 0.41f, 1.0f);
        ImVec4 ScrollbarGrabActive = ImVec4(0.51f, 0.51f, 0.51f, 1.0f);

        // Button colors
        ImVec4 Button = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
        ImVec4 ButtonHovered = ImVec4(0.26f, 0.59f, 0.98f, 1.0f);
        ImVec4 ButtonActive = ImVec4(0.06f, 0.53f, 0.98f, 1.0f);

        // Header colors
        ImVec4 Header = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
        ImVec4 HeaderHovered = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
        ImVec4 HeaderActive = ImVec4(0.26f, 0.59f, 0.98f, 1.0f);

        // Separator colors
        ImVec4 Separator = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
        ImVec4 SeparatorHovered = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
        ImVec4 SeparatorActive = ImVec4(0.10f, 0.40f, 0.75f, 1.0f);

        // Resize grip colors
        ImVec4 ResizeGrip = ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
        ImVec4 ResizeGripHovered = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
        ImVec4 ResizeGripActive = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);

        // Tab colors
        ImVec4 Tab = ImVec4(0.18f, 0.35f, 0.58f, 0.86f);
        ImVec4 TabHovered = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
        ImVec4 TabActive = ImVec4(0.20f, 0.41f, 0.68f, 1.0f);
        ImVec4 TabUnfocused = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
        ImVec4 TabUnfocusedActive = ImVec4(0.14f, 0.26f, 0.42f, 1.0f);

        // Docking colors
        ImVec4 DockingPreview = ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
        ImVec4 DockingEmptyBg = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);

        // Plot colors
        ImVec4 PlotLines = ImVec4(0.61f, 0.61f, 0.61f, 1.0f);
        ImVec4 PlotLinesHovered = ImVec4(1.00f, 0.43f, 0.35f, 1.0f);
        ImVec4 PlotHistogram = ImVec4(0.90f, 0.70f, 0.00f, 1.0f);
        ImVec4 PlotHistogramHovered = ImVec4(1.00f, 0.60f, 0.00f, 1.0f);

        // Table colors
        ImVec4 TableHeaderBg = ImVec4(0.19f, 0.19f, 0.20f, 1.0f);
        ImVec4 TableBorderStrong = ImVec4(0.31f, 0.31f, 0.35f, 1.0f);
        ImVec4 TableBorderLight = ImVec4(0.23f, 0.23f, 0.25f, 1.0f);
        ImVec4 TableRowBg = ImVec4(0.00f, 0.00f, 0.00f, 0.0f);
        ImVec4 TableRowBgAlt = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);

        // Text colors
        ImVec4 Text = ImVec4(1.00f, 1.00f, 1.00f, 1.0f);
        ImVec4 TextDisabled = ImVec4(0.50f, 0.50f, 0.50f, 1.0f);
        ImVec4 TextSelectedBg = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);

        // Drag drop colors
        ImVec4 DragDropTarget = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);

        // Navigation colors
        ImVec4 NavHighlight = ImVec4(0.26f, 0.59f, 0.98f, 1.0f);
        ImVec4 NavWindowingHighlight = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        ImVec4 NavWindowingDimBg = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);

        // Modal colors
        ImVec4 ModalWindowDimBg = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

        // Check mark
        ImVec4 CheckMark = ImVec4(0.26f, 0.59f, 0.98f, 1.0f);

        // Slider colors
        ImVec4 SliderGrab = ImVec4(0.24f, 0.52f, 0.88f, 1.0f);
        ImVec4 SliderGrabActive = ImVec4(0.26f, 0.59f, 0.98f, 1.0f);
    };

    /**
     * @struct ThemeStyle
     * @brief Contains style settings for a theme
     */
    struct ThemeStyle
    {
        float Alpha = 1.0f;
        float DisabledAlpha = 0.60f;
        ImVec2 WindowPadding = ImVec2(8.0f, 8.0f);
        float WindowRounding = 0.0f;
        float WindowBorderSize = 1.0f;
        ImVec2 WindowMinSize = ImVec2(32.0f, 32.0f);
        ImVec2 WindowTitleAlign = ImVec2(0.0f, 0.5f);
        ImGuiDir WindowMenuButtonPosition = ImGuiDir_Left;
        float ChildRounding = 0.0f;
        float ChildBorderSize = 1.0f;
        float PopupRounding = 0.0f;
        float PopupBorderSize = 1.0f;
        ImVec2 FramePadding = ImVec2(4.0f, 3.0f);
        float FrameRounding = 0.0f;
        float FrameBorderSize = 0.0f;
        ImVec2 ItemSpacing = ImVec2(8.0f, 4.0f);
        ImVec2 ItemInnerSpacing = ImVec2(4.0f, 4.0f);
        ImVec2 CellPadding = ImVec2(4.0f, 2.0f);
        ImVec2 TouchExtraPadding = ImVec2(0.0f, 0.0f);
        float IndentSpacing = 21.0f;
        float ColumnsMinSpacing = 6.0f;
        float ScrollbarSize = 14.0f;
        float ScrollbarRounding = 9.0f;
        float GrabMinSize = 12.0f;
        float GrabRounding = 0.0f;
        float LogSliderDeadzone = 4.0f;
        float TabRounding = 4.0f;
        float TabBorderSize = 0.0f;
        float TabMinWidthForCloseButton = 0.0f;
        ImGuiDir ColorButtonPosition = ImGuiDir_Right;
        ImVec2 ButtonTextAlign = ImVec2(0.5f, 0.5f);
        ImVec2 SelectableTextAlign = ImVec2(0.0f, 0.0f);
        float SeparatorTextBorderSize = 3.0f;
        ImVec2 SeparatorTextAlign = ImVec2(0.0f, 0.5f);
        ImVec2 SeparatorTextPadding = ImVec2(20.0f, 3.0f);
        ImVec2 DisplayWindowPadding = ImVec2(19.0f, 19.0f);
        ImVec2 DisplaySafeAreaPadding = ImVec2(3.0f, 3.0f);
        float MouseCursorScale = 1.0f;
        bool AntiAliasedLines = true;
        bool AntiAliasedLinesUseTex = true;
        bool AntiAliasedFill = true;
        float CurveTessellationTol = 1.25f;
        float CircleTessellationMaxError = 0.30f;
    };

    /**
     * @class Theme
     * @brief Represents a complete UI theme with colors and style settings
     */
    class Theme
    {
    public:
        std::string name;
        ThemeColors colors;
        ThemeStyle style;

        Theme(const std::string &themeName) : name(themeName) {}

        /**
         * @brief Applies this theme to ImGui
         */
        void Apply() const;

        /**
         * @brief Creates a deep copy of this theme
         * @return Unique pointer to the cloned theme
         */
        std::unique_ptr<Theme> Clone() const;
    };

    /**
     * @class ThemeManager
     * @brief Manages all themes and provides theme switching functionality
     */
    class ThemeManager
    {
    public:
        /**
         * @brief Gets the singleton instance of the theme manager
         * @return Reference to the theme manager instance
         */
        static ThemeManager &GetInstance();

        /**
         * @brief Initializes the theme manager with default themes
         */
        void Initialize();

        /**
         * @brief Registers a new theme
         * @param theme Unique pointer to the theme to register
         */
        void RegisterTheme(std::unique_ptr<Theme> theme);

        /**
         * @brief Sets the active theme by name
         * @param themeName Name of the theme to activate
         * @return True if theme was found and applied, false otherwise
         */
        bool SetActiveTheme(const std::string &themeName);

        /**
         * @brief Gets the currently active theme
         * @return Pointer to the active theme, or nullptr if none set
         */
        Theme *GetActiveTheme() const;

        /**
         * @brief Gets all available theme names
         * @return Vector of theme names
         */
        std::vector<std::string> GetThemeNames() const;

        /**
         * @brief Gets a theme by name
         * @param themeName Name of the theme to get
         * @return Pointer to the theme, or nullptr if not found
         */
        Theme *GetTheme(const std::string &themeName) const;

        /**
         * @brief Applies the currently active theme
         */
        void ApplyActiveTheme() const;

        /**
         * @brief Draws the theme selection UI
         */
        void DrawThemeSelector();

    private:
        std::unordered_map<std::string, std::unique_ptr<Theme>> m_themes;
        std::string m_activeThemeName = "Dark";
        static std::unique_ptr<ThemeManager> s_instance;

        ThemeManager() = default;

        /**
         * @brief Creates the default dark theme
         * @return Unique pointer to the dark theme
         */
        std::unique_ptr<Theme> CreateDarkTheme();

        /**
         * @brief Creates the light theme
         * @return Unique pointer to the light theme
         */
        std::unique_ptr<Theme> CreateLightTheme();

        /**
         * @brief Creates a blue theme
         * @return Unique pointer to the blue theme
         */
        std::unique_ptr<Theme> CreateBlueTheme();

        /**
         * @brief Creates a high contrast theme
         * @return Unique pointer to the high contrast theme
         */
        std::unique_ptr<Theme> CreateHighContrastTheme();
    };
}
