#include "Viewport.h"
#include "Console.h"
#include "Settings.h"
#include "EditorApp.h"
#include "AssetDragDrop.h"
#include <imgui.h>
#include <stdexcept>

namespace Voltray::Editor::Components
{
    Viewport::Viewport()
    {
        try
        {
            initialize();
        }
        catch (const std::exception &e)
        {
            Console::PrintError("Viewport initialization failed: " + std::string(e.what()));
            throw;
        }
        catch (...)
        {
            Console::PrintError("Viewport initialization failed with unknown error");
            throw;
        }
    }

    void Viewport::Draw()
    {
        if (!isInitialized())
        {
            ImGui::TextWrapped("Viewport initialization failed");
            return;
        }

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4{0, 0, 0, 0});
        ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

        ImVec2 size = ImGui::GetContentRegionAvail();
        int width = static_cast<int>(size.x);
        int height = static_cast<int>(size.y);

        // Handle framebuffer resizing
        if (width != m_Framebuffer.GetWidth() || height != m_Framebuffer.GetHeight())
        {
            m_Framebuffer.Resize(width, height);
            m_Scene.UpdateCameraAspect(width, height);
        }

        // Render the scene to framebuffer
        m_Framebuffer.Bind();
        m_Framebuffer.Clear();

        m_Renderer.RenderScene(m_Scene.GetScene(), m_Scene.GetCamera(), m_Scene.GetRenderer(), width, height);

        m_Framebuffer.Unbind(); // Display the rendered image in ImGui
        ImGui::Image((ImTextureID)(intptr_t)m_Framebuffer.GetColorTexture(),
                     size, ImVec2{0, 1}, ImVec2{1, 0});

        // Update camera viewport bounds for input handling
        ImVec2 imagePos = ImGui::GetItemRectMin();
        ImVec2 imageSize = ImGui::GetItemRectSize();
        m_Scene.SetCameraViewportBounds(imagePos.x, imagePos.y, imageSize.x, imageSize.y);

        // Handle drag and drop operations
        if (const auto *payload = Voltray::Editor::Components::Assets::AssetDragDrop::AcceptDrop())
        {
            // Get mouse position relative to the viewport image
            ImVec2 mousePos = ImGui::GetMousePos();
            ImVec2 dropPosition = ImVec2(mousePos.x - imagePos.x, mousePos.y - imagePos.y);

            // Handle the drop
            Voltray::Editor::Components::Assets::AssetDragDrop::HandleViewportDrop(*payload, dropPosition);
        }

        // Handle input
        m_Input.ProcessInput(m_Scene.GetScene(), m_Scene.GetCamera(), imagePos, imageSize);

        ImGui::End();
        ImGui::PopStyleColor();
        ImGui::PopStyleVar();
    }
    void Viewport::initialize()
    {
        Console::Print("Initializing viewport components...");

        try
        { // Initialize framebuffer
            Console::Print("Initializing framebuffer...");
            m_Framebuffer.Initialize();
            if (!m_Framebuffer.IsCreated())
            {
                throw std::runtime_error("Framebuffer initialization failed");
            }
            Console::Print("Framebuffer initialized successfully");

            // Initialize renderer
            Console::Print("Initializing renderer...");
            m_Renderer.Initialize();
            if (!m_Renderer.IsInitialized())
            {
                throw std::runtime_error("Renderer initialization failed");
            }
            Console::Print("Renderer initialized successfully");

            // Initialize scene
            Console::Print("Initializing scene...");
            m_Scene.Initialize();
            if (!m_Scene.IsInitialized())
            {
                throw std::runtime_error("Scene initialization failed");
            }
            Console::Print("Scene initialized successfully");

            m_Initialized = true;
            Console::Print("All viewport components initialized successfully");
        }
        catch (const std::exception &e)
        {
            Console::PrintError("Viewport component initialization failed: " + std::string(e.what()));
            throw;
        }
    }
    bool Viewport::isInitialized() const
    {
        return m_Framebuffer.IsCreated() &&
               m_Renderer.IsInitialized() &&
               m_Scene.IsInitialized();
    }
}
