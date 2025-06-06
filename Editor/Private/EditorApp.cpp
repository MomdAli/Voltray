#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <memory>
#include <filesystem>

#include "EditorApp.h"
#include "Input.h"
#include "ResourceManager.h"
#include "Workspace.h"
#include "Toolbar.h"
#include "Viewport.h"
#include "Inspector.h"
#include "AssetsPanelRefactored.h"
#include "Console.h"
#include "Settings.h"
#include "Dockspace.h"
#include "Theme.h"

using Voltray::Engine::Input;

namespace Voltray::Editor
{
    // Singleton instance
    EditorApp *EditorApp::s_Instance = nullptr; // Helper function to get layout file path
    std::filesystem::path GetLayoutFilePath()
    {
        const auto *currentWorkspace = Voltray::Utils::WorkspaceManager::GetCurrentWorkspace();
        if (currentWorkspace)
        {
            return currentWorkspace->path / "layout.ini";
        }
        // Fallback to current working directory if no workspace is active
        return std::filesystem::current_path() / "layout.ini";
    }

    // Helper function to get ImGui ini file path
    std::filesystem::path GetImGuiIniFilePath()
    {
        const auto *currentWorkspace = Voltray::Utils::WorkspaceManager::GetCurrentWorkspace();
        if (currentWorkspace)
        {
            return currentWorkspace->path / "imgui.ini";
        }
        // Fallback to current working directory if no workspace is active
        return std::filesystem::current_path() / "imgui.ini";
    }
    void EditorApp::Init(GLFWwindow *window)
    {
        // Set singleton
        s_Instance = this;

        // Initialize Input system with window
        Input::Init(window);
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void)io;

        // Set ImGui ini file path to workspace directory
        static std::string imguiIniPath = GetImGuiIniFilePath().string();
        io.IniFilename = imguiIniPath.c_str();

        // Enable keyboard navigation and docking
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_DockingEnable; // Setup Dear ImGui style        // Load font using ResourceManager - check global assets first
        std::string fontPath = Voltray::Utils::ResourceManager::GetGlobalResourcePath("Fonts/Lato.ttf");
        if (!fontPath.empty())
        {
            io.Fonts->AddFontFromFileTTF(fontPath.c_str(), 20.0f);
            Components::Console::PrintSuccess("Loaded font: " + fontPath);
        }
        else
        {
            Components::Console::PrintError("Failed to load font: Fonts/Lato.ttf from global assets");
            // Fallback to ImGui default font
        }
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 460"); // Initialize theme system
        UI::ThemeManager::GetInstance().Initialize();

        // Initialize components
        m_Toolbar = std::make_unique<Components::Toolbar>();
        m_Viewport = std::make_unique<Components::Viewport>();
        m_Inspector = std::make_unique<Components::Inspector>();
        m_Assets = std::make_unique<AssetsPanelRefactored>();
        m_Settings = std::make_unique<Components::Settings>(); // Register panels with default dock regions
        Components::Dockspace::RegisterPanel("Toolbar", m_Toolbar.get(), Components::Dockspace::Region::Top);
        Components::Dockspace::RegisterPanel("Inspector", m_Inspector.get(), Components::Dockspace::Region::Right);
        Components::Dockspace::RegisterPanel("Assets", m_Assets.get(), Components::Dockspace::Region::Bottom);
        Components::Dockspace::RegisterPanel("Console", &Components::Console::GetInstance(), Components::Dockspace::Region::Bottom);
        Components::Dockspace::RegisterPanel("Settings", m_Settings.get(), Components::Dockspace::Region::Right);
        Components::Dockspace::RegisterPanel("Viewport", m_Viewport.get(), Components::Dockspace::Region::Center); // Load saved layout or use default if none exists
        const auto layoutFile = GetLayoutFilePath();

        // Force reset the layout to fix any issues with docking
        Components::Dockspace::Reset(); // Uncomment this if you want to use saved layouts in the future
        if (std::filesystem::exists(layoutFile))
            Components::Dockspace::LoadLayout(layoutFile.string().c_str());
        else
            Components::Dockspace::Reset();
    }
    void EditorApp::RenderUI()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Show workspace dialog on first frame
        if (m_ShowWorkspaceDialogOnStartup && !m_WorkspaceInitialized)
        {
            UI::WorkspaceDialog::Show([this](const Voltray::Utils::Workspace *workspace)
                                      {
                if (workspace) {
                    m_CurrentWorkspace = std::make_shared<Voltray::Utils::Workspace>(*workspace);
                    Components::Console::PrintSuccess("Loaded workspace: " + workspace->name);
                    Components::Console::Print("Workspace path: " + workspace->path.string());
                } else {
                    Components::Console::PrintWarning("No workspace selected. Using default workspace.");
                }
                m_WorkspaceInitialized = true; });
            m_ShowWorkspaceDialogOnStartup = false;
        }

        // Render workspace dialog if it's open
        if (UI::WorkspaceDialog::IsOpen())
        {
            UI::WorkspaceDialog::Render();
        }

#if defined(IMGUI_HAS_DOCK)             // Docking workspace
        Components::Dockspace::Begin(); // Draw all registered panels
        m_Toolbar->Draw();
        if (m_InspectorVisible)
            m_Inspector->Draw();
        if (m_AssetsVisible)
            m_Assets->Draw();
        if (m_ConsoleVisible)
            Components::Console::GetInstance().Draw();
        if (m_SettingsVisible)
            m_Settings->Draw();
        if (m_ViewportVisible)
            m_Viewport->Draw();
        Components::Dockspace::End();
#else // Fallback: No docking, just show panels as floating windows
        if (m_InspectorVisible)
            m_Inspector->Draw();
        if (m_AssetsVisible)
            m_Assets->Draw();
        if (m_ConsoleVisible)
            Components::Console::GetInstance().Draw();
        if (m_SettingsVisible)
            m_Settings->Draw();
        if (m_ViewportVisible)
            m_Viewport->Draw();
#endif

        ImGui::Render();
        glDisable(GL_DEPTH_TEST);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glEnable(GL_DEPTH_TEST);
    }
    void EditorApp::Shutdown()
    { // Automatically save layout on exit
        const auto layoutPath = GetLayoutFilePath();
        Components::Dockspace::SaveLayout(layoutPath.string().c_str()); // First destroy all components before ImGui cleanup
        // This ensures no component tries to use ImGui after it's destroyed
        m_Viewport.reset();
        m_Inspector.reset();
        m_Assets.reset();
        m_Settings.reset();
        m_Toolbar.reset();

        // Then clean up ImGui
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        // Properly shut down the Input system to prevent use-after-free
        Input::Shutdown();
    }

    // Singleton access
    EditorApp *EditorApp::Get() { return s_Instance; }
}