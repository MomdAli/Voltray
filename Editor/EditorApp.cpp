#include <glad/gl.h>
#include <stdio.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <filesystem>

#include "EditorApp.h"
#include "../Engine/Input/Input.h"
#include "Components/Toolbar.h"
#include "Components/Viewport.h"
#include "Components/Inspector.h"
#include "Components/Assets.h"
#include "Components/Console.h"
#include "Components/Dockspace.h"

namespace Editor
{
    // Singleton instance
    EditorApp *EditorApp::s_Instance = nullptr;
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
        // Enable keyboard navigation and docking
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_DockingEnable;

        // Setup Dear ImGui style
        io.Fonts->AddFontFromFileTTF("Editor/Resources/Fonts/Sora.ttf", 20.0f);
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 460");

        // Initialize components
        m_Toolbar = std::make_unique<Components::Toolbar>();
        m_Viewport = std::make_unique<Components::Viewport>();
        m_Inspector = std::make_unique<Components::Inspector>();
        m_Assets = std::make_unique<Components::Assets>();
        m_Console = std::make_unique<Components::Console>();

        // Register panels with default dock regions
        Components::Dockspace::RegisterPanel("Toolbar", m_Toolbar.get(), Components::Dockspace::Region::Top);
        Components::Dockspace::RegisterPanel("Inspector", m_Inspector.get(), Components::Dockspace::Region::Right);
        Components::Dockspace::RegisterPanel("Assets", m_Assets.get(), Components::Dockspace::Region::Bottom);
        Components::Dockspace::RegisterPanel("Console", m_Console.get(), Components::Dockspace::Region::Bottom);
        Components::Dockspace::RegisterPanel("Viewport", m_Viewport.get(), Components::Dockspace::Region::Center); // Load saved layout or use default if none exists
        const auto layoutFile = std::filesystem::current_path() / "layout.ini";

        // Force reset the layout to fix any issues with docking
        Components::Dockspace::Reset();

        // Uncomment this if you want to use saved layouts in the future
        // if (std::filesystem::exists(layoutFile))
        //     Components::Dockspace::LoadLayout(layoutFile.string().c_str());
        // else
        //     Components::Dockspace::Reset();
    }

    void EditorApp::RenderUI()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

#if defined(IMGUI_HAS_DOCK)
        // Docking workspace
        Components::Dockspace::Begin();
        // Draw all registered panels
        m_Toolbar->Draw();
        m_Inspector->Draw();
        m_Assets->Draw();
        m_Console->Draw();
        m_Viewport->Draw();
        Components::Dockspace::End();
#else
        // Fallback: No docking, just show panels as floating windows
        m_Inspector->Draw();
        m_Assets->Draw();
        m_Console->Draw();
        m_Viewport->Draw();
#endif

        ImGui::Render();
        glDisable(GL_DEPTH_TEST);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glEnable(GL_DEPTH_TEST);
    }

    void EditorApp::Shutdown()
    {
        // Automatically save layout on exit
        const char *layoutPath = (std::filesystem::current_path() / "layout.ini").string().c_str();
        Components::Dockspace::SaveLayout(layoutPath);

        // First destroy all components before ImGui cleanup
        // This ensures no component tries to use ImGui after it's destroyed
        m_Viewport.reset();
        m_Inspector.reset();
        m_Assets.reset();
        m_Console.reset();
        m_Toolbar.reset();

        // Then clean up ImGui        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        // Properly shut down the Input system to prevent use-after-free
        Input::Shutdown();
    }

    // Singleton access
    EditorApp *EditorApp::Get() { return s_Instance; }
}