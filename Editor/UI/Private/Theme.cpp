#include "Theme.h"
#include <imgui.h>

namespace Voltray::Editor::UI
{
    // Static instance initialization
    std::unique_ptr<ThemeManager> ThemeManager::s_instance = nullptr;

    void Theme::Apply() const
    {
        ImGuiStyle &style = ImGui::GetStyle();
        ImVec4 *colors = style.Colors;

        // Apply colors
        colors[ImGuiCol_Text] = this->colors.Text;
        colors[ImGuiCol_TextDisabled] = this->colors.TextDisabled;
        colors[ImGuiCol_WindowBg] = this->colors.WindowBg;
        colors[ImGuiCol_ChildBg] = this->colors.ChildBg;
        colors[ImGuiCol_PopupBg] = this->colors.PopupBg;
        colors[ImGuiCol_Border] = this->colors.Border;
        colors[ImGuiCol_BorderShadow] = this->colors.BorderShadow;
        colors[ImGuiCol_FrameBg] = this->colors.FrameBg;
        colors[ImGuiCol_FrameBgHovered] = this->colors.FrameBgHovered;
        colors[ImGuiCol_FrameBgActive] = this->colors.FrameBgActive;
        colors[ImGuiCol_TitleBg] = this->colors.TitleBg;
        colors[ImGuiCol_TitleBgActive] = this->colors.TitleBgActive;
        colors[ImGuiCol_TitleBgCollapsed] = this->colors.TitleBgCollapsed;
        colors[ImGuiCol_MenuBarBg] = this->colors.MenuBarBg;
        colors[ImGuiCol_ScrollbarBg] = this->colors.ScrollbarBg;
        colors[ImGuiCol_ScrollbarGrab] = this->colors.ScrollbarGrab;
        colors[ImGuiCol_ScrollbarGrabHovered] = this->colors.ScrollbarGrabHovered;
        colors[ImGuiCol_ScrollbarGrabActive] = this->colors.ScrollbarGrabActive;
        colors[ImGuiCol_CheckMark] = this->colors.CheckMark;
        colors[ImGuiCol_SliderGrab] = this->colors.SliderGrab;
        colors[ImGuiCol_SliderGrabActive] = this->colors.SliderGrabActive;
        colors[ImGuiCol_Button] = this->colors.Button;
        colors[ImGuiCol_ButtonHovered] = this->colors.ButtonHovered;
        colors[ImGuiCol_ButtonActive] = this->colors.ButtonActive;
        colors[ImGuiCol_Header] = this->colors.Header;
        colors[ImGuiCol_HeaderHovered] = this->colors.HeaderHovered;
        colors[ImGuiCol_HeaderActive] = this->colors.HeaderActive;
        colors[ImGuiCol_Separator] = this->colors.Separator;
        colors[ImGuiCol_SeparatorHovered] = this->colors.SeparatorHovered;
        colors[ImGuiCol_SeparatorActive] = this->colors.SeparatorActive;
        colors[ImGuiCol_ResizeGrip] = this->colors.ResizeGrip;
        colors[ImGuiCol_ResizeGripHovered] = this->colors.ResizeGripHovered;
        colors[ImGuiCol_ResizeGripActive] = this->colors.ResizeGripActive;
        colors[ImGuiCol_Tab] = this->colors.Tab;
        colors[ImGuiCol_TabHovered] = this->colors.TabHovered;
        colors[ImGuiCol_TabActive] = this->colors.TabActive;
        colors[ImGuiCol_TabUnfocused] = this->colors.TabUnfocused;
        colors[ImGuiCol_TabUnfocusedActive] = this->colors.TabUnfocusedActive;
        colors[ImGuiCol_DockingPreview] = this->colors.DockingPreview;
        colors[ImGuiCol_DockingEmptyBg] = this->colors.DockingEmptyBg;
        colors[ImGuiCol_PlotLines] = this->colors.PlotLines;
        colors[ImGuiCol_PlotLinesHovered] = this->colors.PlotLinesHovered;
        colors[ImGuiCol_PlotHistogram] = this->colors.PlotHistogram;
        colors[ImGuiCol_PlotHistogramHovered] = this->colors.PlotHistogramHovered;
        colors[ImGuiCol_TableHeaderBg] = this->colors.TableHeaderBg;
        colors[ImGuiCol_TableBorderStrong] = this->colors.TableBorderStrong;
        colors[ImGuiCol_TableBorderLight] = this->colors.TableBorderLight;
        colors[ImGuiCol_TableRowBg] = this->colors.TableRowBg;
        colors[ImGuiCol_TableRowBgAlt] = this->colors.TableRowBgAlt;
        colors[ImGuiCol_TextSelectedBg] = this->colors.TextSelectedBg;
        colors[ImGuiCol_DragDropTarget] = this->colors.DragDropTarget;
        colors[ImGuiCol_NavHighlight] = this->colors.NavHighlight;
        colors[ImGuiCol_NavWindowingHighlight] = this->colors.NavWindowingHighlight;
        colors[ImGuiCol_NavWindowingDimBg] = this->colors.NavWindowingDimBg;
        colors[ImGuiCol_ModalWindowDimBg] = this->colors.ModalWindowDimBg;

        // Apply style settings
        style.Alpha = this->style.Alpha;
        style.DisabledAlpha = this->style.DisabledAlpha;
        style.WindowPadding = this->style.WindowPadding;
        style.WindowRounding = this->style.WindowRounding;
        style.WindowBorderSize = this->style.WindowBorderSize;
        style.WindowMinSize = this->style.WindowMinSize;
        style.WindowTitleAlign = this->style.WindowTitleAlign;
        style.WindowMenuButtonPosition = this->style.WindowMenuButtonPosition;
        style.ChildRounding = this->style.ChildRounding;
        style.ChildBorderSize = this->style.ChildBorderSize;
        style.PopupRounding = this->style.PopupRounding;
        style.PopupBorderSize = this->style.PopupBorderSize;
        style.FramePadding = this->style.FramePadding;
        style.FrameRounding = this->style.FrameRounding;
        style.FrameBorderSize = this->style.FrameBorderSize;
        style.ItemSpacing = this->style.ItemSpacing;
        style.ItemInnerSpacing = this->style.ItemInnerSpacing;
        style.CellPadding = this->style.CellPadding;
        style.TouchExtraPadding = this->style.TouchExtraPadding;
        style.IndentSpacing = this->style.IndentSpacing;
        style.ColumnsMinSpacing = this->style.ColumnsMinSpacing;
        style.ScrollbarSize = this->style.ScrollbarSize;
        style.ScrollbarRounding = this->style.ScrollbarRounding;
        style.GrabMinSize = this->style.GrabMinSize;
        style.GrabRounding = this->style.GrabRounding;
        style.LogSliderDeadzone = this->style.LogSliderDeadzone;
        style.TabRounding = this->style.TabRounding;
        style.TabBorderSize = this->style.TabBorderSize;
        style.ColorButtonPosition = this->style.ColorButtonPosition;
        style.ButtonTextAlign = this->style.ButtonTextAlign;
        style.SelectableTextAlign = this->style.SelectableTextAlign;
        style.SeparatorTextBorderSize = this->style.SeparatorTextBorderSize;
        style.SeparatorTextAlign = this->style.SeparatorTextAlign;
        style.SeparatorTextPadding = this->style.SeparatorTextPadding;
        style.DisplayWindowPadding = this->style.DisplayWindowPadding;
        style.DisplaySafeAreaPadding = this->style.DisplaySafeAreaPadding;
        style.MouseCursorScale = this->style.MouseCursorScale;
        style.AntiAliasedLines = this->style.AntiAliasedLines;
        style.AntiAliasedLinesUseTex = this->style.AntiAliasedLinesUseTex;
        style.AntiAliasedFill = this->style.AntiAliasedFill;
        style.CurveTessellationTol = this->style.CurveTessellationTol;
        style.CircleTessellationMaxError = this->style.CircleTessellationMaxError;
    }

    std::unique_ptr<Theme> Theme::Clone() const
    {
        auto cloned = std::make_unique<Theme>(this->name);
        cloned->colors = this->colors;
        cloned->style = this->style;
        return cloned;
    }
    ThemeManager &ThemeManager::GetInstance()
    {
        if (!s_instance)
        {
            s_instance = std::unique_ptr<ThemeManager>(new ThemeManager());
        }
        return *s_instance;
    }

    void ThemeManager::Initialize()
    {
        // Register default themes
        RegisterTheme(CreateDarkTheme());
        RegisterTheme(CreateLightTheme());
        RegisterTheme(CreateBlueTheme());
        RegisterTheme(CreateHighContrastTheme());

        // Set dark theme as default
        SetActiveTheme("Dark");
    }

    void ThemeManager::RegisterTheme(std::unique_ptr<Theme> theme)
    {
        m_themes[theme->name] = std::move(theme);
    }

    bool ThemeManager::SetActiveTheme(const std::string &themeName)
    {
        if (m_themes.find(themeName) != m_themes.end())
        {
            m_activeThemeName = themeName;
            ApplyActiveTheme();
            return true;
        }
        return false;
    }

    Theme *ThemeManager::GetActiveTheme() const
    {
        auto it = m_themes.find(m_activeThemeName);
        return (it != m_themes.end()) ? it->second.get() : nullptr;
    }

    std::vector<std::string> ThemeManager::GetThemeNames() const
    {
        std::vector<std::string> names;
        names.reserve(m_themes.size());
        for (const auto &theme : m_themes)
        {
            names.push_back(theme.first);
        }
        return names;
    }

    Theme *ThemeManager::GetTheme(const std::string &themeName) const
    {
        auto it = m_themes.find(themeName);
        return (it != m_themes.end()) ? it->second.get() : nullptr;
    }

    void ThemeManager::ApplyActiveTheme() const
    {
        if (Theme *activeTheme = GetActiveTheme())
        {
            activeTheme->Apply();
        }
    }

    void ThemeManager::DrawThemeSelector()
    {
        if (ImGui::BeginCombo("Theme", m_activeThemeName.c_str()))
        {
            for (const auto &themePair : m_themes)
            {
                const std::string &themeName = themePair.first;
                bool isSelected = (themeName == m_activeThemeName);

                if (ImGui::Selectable(themeName.c_str(), isSelected))
                {
                    SetActiveTheme(themeName);
                }

                if (isSelected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
    }

    std::unique_ptr<Theme> ThemeManager::CreateDarkTheme()
    {
        auto theme = std::make_unique<Theme>("Dark");

        // Dark theme colors (already set in the default ThemeColors struct)
        // No need to modify colors for dark theme as defaults are dark

        return theme;
    }

    std::unique_ptr<Theme> ThemeManager::CreateLightTheme()
    {
        auto theme = std::make_unique<Theme>("Light");

        // Light theme colors
        theme->colors.WindowBg = ImVec4(0.94f, 0.94f, 0.94f, 1.0f);
        theme->colors.ChildBg = ImVec4(0.96f, 0.96f, 0.96f, 0.0f);
        theme->colors.PopupBg = ImVec4(1.00f, 1.00f, 1.00f, 0.98f);
        theme->colors.Border = ImVec4(0.00f, 0.00f, 0.00f, 0.30f);
        theme->colors.BorderShadow = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        theme->colors.FrameBg = ImVec4(1.00f, 1.00f, 1.00f, 1.0f);
        theme->colors.FrameBgHovered = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
        theme->colors.FrameBgActive = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
        theme->colors.TitleBg = ImVec4(0.96f, 0.96f, 0.96f, 1.0f);
        theme->colors.TitleBgActive = ImVec4(0.82f, 0.82f, 0.82f, 1.0f);
        theme->colors.TitleBgCollapsed = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
        theme->colors.MenuBarBg = ImVec4(0.86f, 0.86f, 0.86f, 1.0f);
        theme->colors.ScrollbarBg = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
        theme->colors.ScrollbarGrab = ImVec4(0.69f, 0.69f, 0.69f, 0.80f);
        theme->colors.ScrollbarGrabHovered = ImVec4(0.49f, 0.49f, 0.49f, 0.80f);
        theme->colors.ScrollbarGrabActive = ImVec4(0.49f, 0.49f, 0.49f, 1.0f);
        theme->colors.CheckMark = ImVec4(0.26f, 0.59f, 0.98f, 1.0f);
        theme->colors.SliderGrab = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
        theme->colors.SliderGrabActive = ImVec4(0.46f, 0.54f, 0.80f, 0.60f);
        theme->colors.Button = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
        theme->colors.ButtonHovered = ImVec4(0.26f, 0.59f, 0.98f, 1.0f);
        theme->colors.ButtonActive = ImVec4(0.06f, 0.53f, 0.98f, 1.0f);
        theme->colors.Header = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
        theme->colors.HeaderHovered = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
        theme->colors.HeaderActive = ImVec4(0.26f, 0.59f, 0.98f, 1.0f);
        theme->colors.Separator = ImVec4(0.39f, 0.39f, 0.39f, 0.62f);
        theme->colors.SeparatorHovered = ImVec4(0.14f, 0.44f, 0.80f, 0.78f);
        theme->colors.SeparatorActive = ImVec4(0.14f, 0.44f, 0.80f, 1.0f);
        theme->colors.ResizeGrip = ImVec4(0.35f, 0.35f, 0.35f, 0.17f);
        theme->colors.ResizeGripHovered = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
        theme->colors.ResizeGripActive = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
        theme->colors.Tab = ImVec4(0.76f, 0.80f, 0.84f, 0.93f);
        theme->colors.TabHovered = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
        theme->colors.TabActive = ImVec4(0.60f, 0.73f, 0.88f, 1.0f);
        theme->colors.TabUnfocused = ImVec4(0.92f, 0.93f, 0.94f, 0.99f);
        theme->colors.TabUnfocusedActive = ImVec4(0.74f, 0.82f, 0.91f, 1.0f);
        theme->colors.DockingPreview = ImVec4(0.26f, 0.59f, 0.98f, 0.22f);
        theme->colors.DockingEmptyBg = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        theme->colors.PlotLines = ImVec4(0.39f, 0.39f, 0.39f, 1.0f);
        theme->colors.PlotLinesHovered = ImVec4(1.00f, 0.43f, 0.35f, 1.0f);
        theme->colors.PlotHistogram = ImVec4(0.90f, 0.70f, 0.00f, 1.0f);
        theme->colors.PlotHistogramHovered = ImVec4(1.00f, 0.45f, 0.00f, 1.0f);
        theme->colors.TableHeaderBg = ImVec4(0.78f, 0.87f, 0.98f, 1.0f);
        theme->colors.TableBorderStrong = ImVec4(0.57f, 0.57f, 0.64f, 1.0f);
        theme->colors.TableBorderLight = ImVec4(0.68f, 0.68f, 0.74f, 1.0f);
        theme->colors.TableRowBg = ImVec4(0.00f, 0.00f, 0.00f, 0.0f);
        theme->colors.TableRowBgAlt = ImVec4(0.30f, 0.30f, 0.30f, 0.09f);
        theme->colors.Text = ImVec4(0.00f, 0.00f, 0.00f, 1.0f);
        theme->colors.TextDisabled = ImVec4(0.60f, 0.60f, 0.60f, 1.0f);
        theme->colors.TextSelectedBg = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
        theme->colors.DragDropTarget = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
        theme->colors.NavHighlight = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
        theme->colors.NavWindowingHighlight = ImVec4(0.70f, 0.70f, 0.70f, 0.70f);
        theme->colors.NavWindowingDimBg = ImVec4(0.20f, 0.20f, 0.20f, 0.20f);
        theme->colors.ModalWindowDimBg = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

        return theme;
    }

    std::unique_ptr<Theme> ThemeManager::CreateBlueTheme()
    {
        auto theme = std::make_unique<Theme>("Blue");

        // Blue theme colors - a cool blue-tinted dark theme
        theme->colors.WindowBg = ImVec4(0.05f, 0.07f, 0.12f, 1.0f);
        theme->colors.ChildBg = ImVec4(0.03f, 0.05f, 0.10f, 0.0f);
        theme->colors.PopupBg = ImVec4(0.06f, 0.08f, 0.14f, 0.94f);
        theme->colors.FrameBg = ImVec4(0.12f, 0.20f, 0.35f, 0.54f);
        theme->colors.FrameBgHovered = ImVec4(0.20f, 0.35f, 0.60f, 0.40f);
        theme->colors.FrameBgActive = ImVec4(0.25f, 0.45f, 0.80f, 0.67f);
        theme->colors.TitleBg = ImVec4(0.03f, 0.04f, 0.08f, 1.0f);
        theme->colors.TitleBgActive = ImVec4(0.12f, 0.20f, 0.35f, 1.0f);
        theme->colors.MenuBarBg = ImVec4(0.08f, 0.10f, 0.16f, 1.0f);
        theme->colors.Button = ImVec4(0.15f, 0.30f, 0.60f, 0.40f);
        theme->colors.ButtonHovered = ImVec4(0.20f, 0.40f, 0.80f, 1.0f);
        theme->colors.ButtonActive = ImVec4(0.10f, 0.25f, 0.50f, 1.0f);
        theme->colors.Header = ImVec4(0.15f, 0.30f, 0.60f, 0.31f);
        theme->colors.HeaderHovered = ImVec4(0.20f, 0.40f, 0.80f, 0.80f);
        theme->colors.HeaderActive = ImVec4(0.25f, 0.50f, 1.00f, 1.0f);
        theme->colors.Tab = ImVec4(0.10f, 0.20f, 0.40f, 0.86f);
        theme->colors.TabHovered = ImVec4(0.20f, 0.40f, 0.80f, 0.80f);
        theme->colors.TabActive = ImVec4(0.15f, 0.30f, 0.60f, 1.0f);

        return theme;
    }

    std::unique_ptr<Theme> ThemeManager::CreateHighContrastTheme()
    {
        auto theme = std::make_unique<Theme>("High Contrast");

        // High contrast theme colors - black and white with bright accents
        theme->colors.WindowBg = ImVec4(0.00f, 0.00f, 0.00f, 1.0f);
        theme->colors.ChildBg = ImVec4(0.05f, 0.05f, 0.05f, 0.0f);
        theme->colors.PopupBg = ImVec4(0.00f, 0.00f, 0.00f, 0.98f);
        theme->colors.Border = ImVec4(1.00f, 1.00f, 1.00f, 1.0f);
        theme->colors.FrameBg = ImVec4(0.10f, 0.10f, 0.10f, 1.0f);
        theme->colors.FrameBgHovered = ImVec4(1.00f, 1.00f, 0.00f, 0.40f);
        theme->colors.FrameBgActive = ImVec4(1.00f, 1.00f, 0.00f, 0.67f);
        theme->colors.TitleBg = ImVec4(0.00f, 0.00f, 0.00f, 1.0f);
        theme->colors.TitleBgActive = ImVec4(1.00f, 1.00f, 1.00f, 1.0f);
        theme->colors.MenuBarBg = ImVec4(0.05f, 0.05f, 0.05f, 1.0f);
        theme->colors.Button = ImVec4(0.20f, 0.20f, 0.20f, 1.0f);
        theme->colors.ButtonHovered = ImVec4(1.00f, 1.00f, 0.00f, 1.0f);
        theme->colors.ButtonActive = ImVec4(0.00f, 1.00f, 0.00f, 1.0f);
        theme->colors.Header = ImVec4(0.30f, 0.30f, 0.30f, 1.0f);
        theme->colors.HeaderHovered = ImVec4(1.00f, 1.00f, 0.00f, 0.80f);
        theme->colors.HeaderActive = ImVec4(0.00f, 1.00f, 0.00f, 1.0f);
        theme->colors.Text = ImVec4(1.00f, 1.00f, 1.00f, 1.0f);
        theme->colors.TextDisabled = ImVec4(0.70f, 0.70f, 0.70f, 1.0f);
        theme->colors.CheckMark = ImVec4(0.00f, 1.00f, 0.00f, 1.0f);
        theme->colors.SliderGrab = ImVec4(1.00f, 1.00f, 0.00f, 1.0f);
        theme->colors.SliderGrabActive = ImVec4(0.00f, 1.00f, 0.00f, 1.0f);

        return theme;
    }
}
