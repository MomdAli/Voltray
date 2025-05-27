#include "Settings.h"
#include "Console.h"
#include "../UI/Theme.h"
#include "../../Utils/ResourceManager.h"

#include <imgui.h>
#include <filesystem>

namespace Editor::Components
{
    void Settings::Draw()
    {
        ImGui::Begin("Settings");

        // Create collapsible sections
        if (ImGui::CollapsingHeader("Theme Settings", &m_showThemeSettings))
        {
            ImGui::Indent();

            ImGui::Text("Select Theme:");
            UI::ThemeManager::GetInstance().DrawThemeSelector();

            ImGui::Separator();
            ImGui::Text("Theme Information:");
            if (auto *activeTheme = UI::ThemeManager::GetInstance().GetActiveTheme())
            {
                ImGui::Text("Current Theme: %s", activeTheme->name.c_str());
            }

            ImGui::Unindent();
        }

        if (ImGui::CollapsingHeader("General Settings", &m_showGeneralSettings))
        {
            ImGui::Indent();

            ImGui::Text("Application Settings");

            static bool vsync = true;
            ImGui::Checkbox("V-Sync", &vsync);

            static int targetFPS = 60;
            ImGui::SliderInt("Target FPS", &targetFPS, 30, 144);

            static float uiScale = 1.0f;
            ImGui::SliderFloat("UI Scale", &uiScale, 0.5f, 2.0f);

            ImGui::Unindent();
        }

        if (ImGui::CollapsingHeader("Editor Settings", &m_showEditorSettings))
        {
            ImGui::Indent();

            ImGui::Text("Editor Preferences");

            static bool autoSave = true;
            ImGui::Checkbox("Auto Save", &autoSave);

            static int autoSaveInterval = 300;
            ImGui::SliderInt("Auto Save Interval (seconds)", &autoSaveInterval, 60, 600);

            static bool showLineNumbers = true;
            ImGui::Checkbox("Show Line Numbers", &showLineNumbers);

            static bool wordWrap = false;
            ImGui::Checkbox("Word Wrap", &wordWrap);

            ImGui::Unindent();
        }

        if (ImGui::CollapsingHeader("Debug Settings", &m_showDebugSettings))
        {
            ImGui::Indent();

            ImGui::Text("Console & Debug Options");

            if (ImGui::Button("Test Console Messages"))
            {
                Console::Print("This is a test info message");
                Console::PrintWarning("This is a test warning message");
                Console::PrintError("This is a test error message");
                Console::PrintDebug("This is a test debug message");
                Console::PrintSuccess("This is a test success message");
            }

            if (ImGui::Button("Clear Console"))
            {
                Console::GetInstance().Clear();
            }

            static bool showImGuiDemo = false;
            ImGui::Checkbox("Show ImGui Demo Window", &showImGuiDemo);
            if (showImGuiDemo)
            {
                ImGui::ShowDemoWindow(&showImGuiDemo);
            }
            static bool showImGuiMetrics = false;
            ImGui::Checkbox("Show ImGui Metrics", &showImGuiMetrics);
            if (showImGuiMetrics)
            {
                ImGui::ShowMetricsWindow(&showImGuiMetrics);
            }

            ImGui::Separator();

            ImGui::Text("Resource Manager");
            ImGui::TextWrapped("Font Path: %s",
                               ResourceManager::GetResourcePath("Editor/Resources/Fonts/Sora.ttf").c_str());
            ImGui::TextWrapped("App Directory: %s",
                               ResourceManager::GetApplicationDirectory().c_str());

            ImGui::Unindent();
        }

        ImGui::End();
    }
}
