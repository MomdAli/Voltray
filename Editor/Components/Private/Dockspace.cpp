#include "Dockspace.h"
#include <imgui.h>
#include <imgui_internal.h>

// Make sure docking is enabled in your ImGui config
#ifndef ImGuiDockNodeFlags_None
typedef int ImGuiDockNodeFlags;
#define ImGuiDockNodeFlags_None 0
#endif

namespace Voltray::Editor::Components
{
        // Static member definitions
        bool Dockspace::s_Initialized = false;
        std::vector<Dockspace::PanelInfo> Dockspace::s_Panels;
        std::array<ImGuiID, 5> Dockspace::s_Regions;

        void Dockspace::RegisterPanel(const char *name, Editor::Panel *panel, Region defaultRegion)
        {
                s_Panels.push_back({name, panel, defaultRegion});
        }

        void Dockspace::Reset()
        {
                s_Initialized = false;
        }

        void Dockspace::Begin()
        {
                const ImGuiViewport *viewport = ImGui::GetMainViewport();
                if (!viewport)
                        return;

                // Fullscreen window for dockspace
                ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
                                                ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                                ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
                ImGui::SetNextWindowPos(viewport->WorkPos);
                ImGui::SetNextWindowSize(viewport->WorkSize);
                ImGui::SetNextWindowViewport(viewport->ID);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
                ImGui::Begin("DockSpace", nullptr, window_flags);
                ImGui::PopStyleVar(3);

                ImGuiID rootID = ImGui::GetID("MainDockSpace");
                if (!s_Initialized)
                {
                        ImGui::DockBuilderRemoveNode(rootID);
                        ImGui::DockBuilderAddNode(rootID, ImGuiDockNodeFlags_DockSpace);
                        ImGui::DockBuilderSetNodeSize(rootID, viewport->Size);
                        ImGuiID mainID = rootID;
                        // Adjust docking regions to better suit the layout
                        s_Regions[(int)Region::Left] = ImGui::DockBuilderSplitNode(mainID, ImGuiDir_Left, 0.18f, nullptr, &mainID);
                        s_Regions[(int)Region::Right] = ImGui::DockBuilderSplitNode(mainID, ImGuiDir_Right, 0.25f, nullptr, &mainID); // Made Inspector wider
                        s_Regions[(int)Region::Bottom] = ImGui::DockBuilderSplitNode(mainID, ImGuiDir_Down, 0.25f, nullptr, &mainID);
                        s_Regions[(int)Region::Top] = ImGui::DockBuilderSplitNode(mainID, ImGuiDir_Up, 0.10f, nullptr, &mainID);
                        s_Regions[(int)Region::Center] = mainID;

                        // Dock registered panels into their regions
                        for (auto &info : s_Panels)
                        {
                                ImGui::DockBuilderDockWindow(info.name, s_Regions[(int)info.region]);
                        }
                        ImGui::DockBuilderFinish(rootID);
                        s_Initialized = true;
                }

                ImGui::DockSpace(rootID, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
        }

        void Dockspace::End()
        {
                ImGui::End();
        }

        void Dockspace::SaveLayout(const char *filename)
        {
                // Persist all ImGui window and docking settings
                ImGui::SaveIniSettingsToDisk(filename);
        }

        void Dockspace::LoadLayout(const char *filename)
        {
                // Load saved ImGui window and docking settings
                ImGui::LoadIniSettingsFromDisk(filename);
                // Prevent rebuilding default layout
                s_Initialized = true;
        }
}
