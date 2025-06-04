#include "Settings.h"
#include "Console.h"
#include "Theme.h"
#include "ResourceManager.h"
#include "EditorApp.h"
#include "ViewportScene.h"
#include "BaseCamera.h"
#include "PerspectiveCamera.h"
#include "OrthographicCamera.h"
#include "EngineSettings.h"

using namespace Voltray::Utils;

#include <imgui.h>
#include <filesystem>

namespace Voltray::Editor::Components
{
    using EngineSettings = Voltray::Engine::EngineSettings;

    void Settings::Draw()
    {
        ImGui::Begin("Settings"); // Create collapsible sections
        if (ImGui::CollapsingHeader("Theme Settings"))
        {
            ImGui::Indent();
            ImGui::TextWrapped("Select Theme:");
            UI::ThemeManager::GetInstance().DrawThemeSelector();
            ImGui::Separator();
            ImGui::TextWrapped("Theme Information:");
            if (auto *activeTheme = UI::ThemeManager::GetInstance().GetActiveTheme())
            {
                ImGui::TextWrapped("Current Theme: %s", activeTheme->name.c_str());
            }

            ImGui::Unindent();
        }
        if (ImGui::CollapsingHeader("General Settings"))
        {
            ImGui::Indent();

            ImGui::TextWrapped("Application Settings");

            static bool vsync = true;
            ImGui::Checkbox("V-Sync", &vsync);

            static int targetFPS = 60;
            ImGui::TextWrapped("Target FPS:");
            ImGui::SliderInt("##TargetFPS", &targetFPS, 30, 144);

            static float uiScale = 1.0f;
            ImGui::TextWrapped("UI Scale:");
            ImGui::SliderFloat("##UIScale", &uiScale, 0.5f, 2.0f);

            ImGui::Unindent();
        }
        if (ImGui::CollapsingHeader("Editor Settings"))
        {
            ImGui::Indent();

            ImGui::TextWrapped("Editor Preferences");

            static bool autoSave = true;
            ImGui::Checkbox("Auto Save", &autoSave);

            static int autoSaveInterval = 300;
            ImGui::TextWrapped("Auto Save Interval (seconds):");
            ImGui::SliderInt("##AutoSaveInterval", &autoSaveInterval, 60, 600);

            static bool showLineNumbers = true;
            ImGui::Checkbox("Show Line Numbers", &showLineNumbers);

            static bool wordWrap = false;
            ImGui::Checkbox("Word Wrap", &wordWrap);

            ImGui::Unindent();
        }
        if (ImGui::CollapsingHeader("Debug Settings"))
        {
            ImGui::Indent();

            ImGui::TextWrapped("Console & Debug Options");

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

            ImGui::TextWrapped("Resource Manager");
            ImGui::TextWrapped("Font Path: %s",
                               ResourceManager::GetResourcePath("Editor/Resources/Fonts/Sora.ttf").c_str());
            ImGui::TextWrapped("App Directory: %s",
                               ResourceManager::GetApplicationDirectory().c_str());
            ImGui::Unindent();
        }
        if (ImGui::CollapsingHeader("Engine Settings"))
        {
            ImGui::Indent();

            ImGui::TextWrapped("Camera Controls");
            ImGui::Separator();

            // Camera orbit speed
            ImGui::TextWrapped("Camera Orbit Speed:");
            if (ImGui::SliderFloat("##CameraOrbitSpeed", &EngineSettings::CameraOrbitSpeed, 0.1f, 1.0f, "%.2f"))
            {
                // Settings automatically updated since we're modifying static members
            }

            // Camera pan speed
            ImGui::TextWrapped("Camera Pan Speed:");
            if (ImGui::SliderFloat("##CameraPanSpeed", &EngineSettings::CameraPanSpeed, 0.001f, 0.01f, "%.4f"))
            {
                // Settings automatically updated since we're modifying static members
            }

            // Camera zoom speed
            ImGui::TextWrapped("Camera Zoom Speed:");
            if (ImGui::SliderFloat("##CameraZoomSpeed", &EngineSettings::CameraZoomSpeed, 0.1f, 3.0f, "%.1f"))
            {
                // Settings automatically updated since we're modifying static members
            }

            // Camera distance limits
            ImGui::TextWrapped("Camera Distance Limits:");
            ImGui::TextWrapped("Min Distance:");
            if (ImGui::SliderFloat("##CameraMinDistance", &EngineSettings::CameraMinDistance, 0.1f, 5.0f, "%.1f"))
            {
                // Ensure min distance is not greater than max distance
                if (EngineSettings::CameraMinDistance > EngineSettings::CameraMaxDistance)
                {
                    EngineSettings::CameraMaxDistance = EngineSettings::CameraMinDistance;
                }
            }

            ImGui::TextWrapped("Max Distance:");
            if (ImGui::SliderFloat("##CameraMaxDistance", &EngineSettings::CameraMaxDistance, 5.0f, 200.0f, "%.1f"))
            {
                // Ensure max distance is not less than min distance
                if (EngineSettings::CameraMaxDistance < EngineSettings::CameraMinDistance)
                {
                    EngineSettings::CameraMinDistance = EngineSettings::CameraMaxDistance;
                }
            }

            // Mouse clamp delta
            ImGui::TextWrapped("Mouse Sensitivity Clamp:");
            if (ImGui::SliderFloat("##MouseClampDelta", &EngineSettings::MouseClampDelta, 5.0f, 50.0f, "%.1f"))
            {
                // Settings automatically updated since we're modifying static members
            }

            ImGui::Separator();
            ImGui::TextWrapped("Renderer Settings");

            // Clear color
            ImGui::TextWrapped("Background Color:");
            if (ImGui::ColorEdit4("##ClearColor", EngineSettings::ClearColor, ImGuiColorEditFlags_NoInputs))
            {
                // Settings automatically updated since we're modifying static members
            }

            ImGui::Separator();

            // Save/Load buttons
            if (ImGui::Button("Save Settings"))
            {
                EngineSettings::Save("engine_settings.cfg");
                Console::Print("Engine settings saved to engine_settings.cfg");
            }
            ImGui::SameLine();
            if (ImGui::Button("Load Settings"))
            {
                EngineSettings::Load("engine_settings.cfg");
                Console::Print("Engine settings loaded from engine_settings.cfg");
            }
            ImGui::SameLine();
            if (ImGui::Button("Reset to Defaults"))
            {
                // Reset to default values
                EngineSettings::CameraOrbitSpeed = 0.3f;
                EngineSettings::CameraPanSpeed = 0.002f;
                EngineSettings::CameraZoomSpeed = 1.0f;
                EngineSettings::CameraMinDistance = 0.5f;
                EngineSettings::CameraMaxDistance = 100.0f;
                EngineSettings::MouseClampDelta = 22.0f;
                EngineSettings::ClearColor[0] = 0.1f;
                EngineSettings::ClearColor[1] = 0.1f;
                EngineSettings::ClearColor[2] = 0.1f;
                EngineSettings::ClearColor[3] = 1.0f;
                Console::Print("Engine settings reset to defaults");
            }
            ImGui::Unindent();
        }

        if (ImGui::CollapsingHeader("Camera Settings"))
        {
            DrawCameraSettings();
        }
        ImGui::End();
    }
    void Settings::DrawCameraSettings()
    {
        ImGui::Indent();

        ImGui::TextWrapped("Camera Configuration");
        ImGui::Separator();

        // Get current viewport and camera state
        auto *editorApp = Editor::EditorApp::Get();
        if (editorApp && editorApp->GetViewport())
        {
            auto &viewportScene = editorApp->GetViewport()->GetScene();

            // Sync settings with current camera
            CameraType currentCameraType = viewportScene.GetCurrentCameraType();
            if (m_selectedCameraType != currentCameraType)
            {
                m_selectedCameraType = currentCameraType;
            }

            DrawCameraSelector();
            ImGui::Separator();
            DrawCameraSpecificSettings();
        }
        else
        {
            ImGui::TextWrapped("Viewport not available");
        }

        ImGui::Unindent();
    }
    void Settings::DrawCameraSelector()
    {
        ImGui::TextWrapped("Camera Type:");

        const char *currentCameraName = GetCameraTypeName(m_selectedCameraType);
        if (ImGui::BeginCombo("##CameraType", currentCameraName))
        {
            auto *editorApp = Editor::EditorApp::Get();

            // Perspective Camera
            if (ImGui::MenuItem("Perspective", nullptr, m_selectedCameraType == CameraType::PERSPECTIVE))
            {
                m_selectedCameraType = CameraType::PERSPECTIVE;
                if (editorApp && editorApp->GetViewport())
                {
                    editorApp->GetViewport()->GetScene().SwitchCamera(CameraType::PERSPECTIVE);
                }
            }

            // Orthographic Camera
            if (ImGui::MenuItem("Orthographic", nullptr, m_selectedCameraType == CameraType::ORTHOGRAPHIC))
            {
                m_selectedCameraType = CameraType::ORTHOGRAPHIC;
                if (editorApp && editorApp->GetViewport())
                {
                    editorApp->GetViewport()->GetScene().SwitchCamera(CameraType::ORTHOGRAPHIC);
                }
            }

            ImGui::EndCombo();
        }
    }
    void Settings::DrawCameraSpecificSettings()
    {
        ImGui::TextWrapped("Camera Properties:");

        auto *editorApp = Editor::EditorApp::Get();
        if (!editorApp || !editorApp->GetViewport())
        {
            ImGui::TextWrapped("Viewport not available");
            return;
        }

        auto &viewportScene = editorApp->GetViewport()->GetScene();

        // Show different settings based on camera type
        const char *category = GetCameraCategory(m_selectedCameraType);

        if (strcmp(category, "Perspective") == 0)
        {
            // Perspective camera settings
            ImGui::TextWrapped("Field of View:");
            if (ImGui::SliderFloat("##FOV", &m_perspectiveFOV, 10.0f, 120.0f, "%.1f°"))
            {
                viewportScene.UpdateCameraSettings(m_perspectiveFOV, m_orthographicSize, m_cameraNearPlane, m_cameraFarPlane);
            }
        }
        else if (strcmp(category, "Orthographic") == 0)
        {
            // Orthographic camera settings
            ImGui::TextWrapped("Orthographic Size:");
            if (ImGui::SliderFloat("##OrthoSize", &m_orthographicSize, 1.0f, 20.0f, "%.1f"))
            {
                viewportScene.UpdateCameraSettings(m_perspectiveFOV, m_orthographicSize, m_cameraNearPlane, m_cameraFarPlane);
            }
        } // Common camera information
        ImGui::Separator();
        ImGui::TextWrapped("Current Camera: %s", GetCameraTypeName(m_selectedCameraType));
        ImGui::TextWrapped("Category: %s", category);

        // Near/Far clipping planes (common to all cameras)
        ImGui::Separator();
        ImGui::TextWrapped("Clipping Planes:");

        bool nearChanged = ImGui::SliderFloat("Near Plane", &m_cameraNearPlane, 0.01f, 10.0f, "%.2f");
        bool farChanged = ImGui::SliderFloat("Far Plane", &m_cameraFarPlane, 10.0f, 1000.0f, "%.1f");

        if (nearChanged || farChanged)
        {
            // Update camera near/far planes
            viewportScene.UpdateCameraSettings(m_perspectiveFOV, m_orthographicSize, m_cameraNearPlane, m_cameraFarPlane);
        }
    }
    const char *Settings::GetCameraTypeName(CameraType type) const
    {
        switch (type)
        {
        case CameraType::PERSPECTIVE:
            return "Perspective";
        case CameraType::ORTHOGRAPHIC:
            return "Orthographic";
        default:
            return "Unknown";
        }
    }
    const char *Settings::GetCameraCategory(CameraType type) const
    {
        switch (type)
        {
        case CameraType::PERSPECTIVE:
            return "Perspective";
        case CameraType::ORTHOGRAPHIC:
            return "Orthographic";
        default:
            return "Unknown";
        }
    }
}
