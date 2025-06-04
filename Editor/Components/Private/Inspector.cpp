#include "Inspector.h"
#include "EditorApp.h"
#include "Viewport.h"
#include "Scene.h"
#include "Transform.h"
#include "Vec3.h"
#include <imgui.h>
#include <string>

namespace Voltray::Editor::Components
{
    void Inspector::Draw()
    {
        // Only set window position and size when not docked
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 4.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.13f, 0.14f, 0.18f, 1.0f)); // Subtle dark

        // Using a visible boolean to make sure the window is shown
        bool visible = true;
        ImGui::Begin("Inspector", &visible, flags);
        ImGui::PopStyleColor();
        ImGui::PopStyleVar(2); // Header
        ImGui::TextWrapped("Inspector");
        ImGui::Separator();
        ImGui::Spacing();

        // Get the selected object from the scene
        auto *editorApp = EditorApp::Get();
        if (!editorApp || !editorApp->GetViewport())
        {
            ImGui::TextWrapped("No viewport available");
            ImGui::End();
            return;
        }

        // Access the scene through the viewport's scene component
        auto &viewportScene = editorApp->GetViewport()->GetScene();
        auto selectedObject = viewportScene.GetScene().GetSelectedObject();
        if (!selectedObject)
        {
            // No selection state
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.6f, 0.6f, 1.0f));
            ImGui::TextWrapped("No object selected");
            ImGui::TextWrapped("Select an object in the viewport to edit its properties");
            ImGui::PopStyleColor();
        }
        else
        {
            DrawObjectProperties(selectedObject);
        }

        ImGui::End();
    }

    void Inspector::DrawObjectProperties(std::shared_ptr<::SceneObject> object)
    { // Object name and basic info
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.9f, 0.9f, 1.0f));
        ImGui::TextWrapped("Object: %s", object->GetName().c_str());
        ImGui::PopStyleColor();

        ImGui::Separator();
        ImGui::Spacing();

        // Transform component
        DrawTransformProperties(object);

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // Material/Shader properties
        DrawMaterialProperties(object);

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // Object properties
        DrawObjectVisibility(object);
    }

    void Inspector::DrawTransformProperties(std::shared_ptr<::SceneObject> object)
    {
        // Transform header
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            auto &transform = object->GetTransform();

            // Position
            Vec3 position = transform.GetPosition();
            float pos[3] = {position.x, position.y, position.z};
            if (ImGui::DragFloat3("Position", pos, 0.1f, -FLT_MAX, FLT_MAX, "%.2f"))
            {
                transform.SetPosition(Vec3(pos[0], pos[1], pos[2]));
            }

            // Rotation
            Vec3 rotation = transform.GetRotation();
            float rot[3] = {rotation.x, rotation.y, rotation.z};
            if (ImGui::DragFloat3("Rotation", rot, 1.0f, -FLT_MAX, FLT_MAX, "%.1f°"))
            {
                transform.SetRotation(Vec3(rot[0], rot[1], rot[2]));
            } // Scale
            Vec3 scale = transform.GetScale();
            float scl[3] = {scale.x, scale.y, scale.z};
            if (ImGui::DragFloat3("Scale", scl, 0.01f, -FLT_MAX, FLT_MAX, "%.3f"))
            {
                transform.SetScale(Vec3(scl[0], scl[1], scl[2]));
            }

            ImGui::Spacing();

            // Pivot (relative to mesh center)
            Vec3 relativePivot = object->GetRelativePivot();
            float pivot[3] = {relativePivot.x, relativePivot.y, relativePivot.z};
            if (ImGui::DragFloat3("Pivot", pivot, 0.01f, -10.0f, 10.0f, "%.3f"))
            {
                object->SetRelativePivot(Vec3(pivot[0], pivot[1], pivot[2]));
            }

            // Help text for pivot
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7f, 0.7f, 0.7f, 1.0f));
            ImGui::TextWrapped("Pivot is relative to the mesh center.\n"
                               "Setting it to (0,0,0) will center the object.\n"
                               "Adjusting pivot allows for easier rotation and scaling around a specific point.");
            ImGui::PopStyleColor();

            ImGui::Spacing();

            // Reset button
            if (ImGui::Button("Reset Transform"))
            {
                transform.Reset();
                object->SetRelativePivot(Vec3(0.0f, 0.0f, 0.0f)); // Reset pivot to center
            }
        }
    }
    void Inspector::DrawMaterialProperties(std::shared_ptr<::SceneObject> object)
    {
        if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::TextWrapped("Shader Properties");

            // Get the material color from the object
            Vec3 currentColor = object->GetMaterialColor();
            float objectColor[3] = {currentColor.x, currentColor.y, currentColor.z};

            if (ImGui::ColorEdit3("Color", objectColor, ImGuiColorEditFlags_NoAlpha))
            {
                // Set the color on the object's material
                object->SetMaterialColor(Vec3(objectColor[0], objectColor[1], objectColor[2]));
            }

            // Material properties placeholders
            static float roughness = 0.5f;
            ImGui::SliderFloat("Roughness", &roughness, 0.0f, 1.0f);

            static float metallic = 0.0f;
            ImGui::SliderFloat("Metallic", &metallic, 0.0f, 1.0f);

            ImGui::TextWrapped("Material color is applied to shader during rendering");
        }
    }

    void Inspector::DrawObjectVisibility(std::shared_ptr<::SceneObject> object)
    {
        if (ImGui::CollapsingHeader("Object Properties", ImGuiTreeNodeFlags_DefaultOpen))
        {
            // Visibility toggle
            bool visible = object->IsVisible();
            if (ImGui::Checkbox("Visible", &visible))
            {
                object->SetVisible(visible);
            } // Selection state (read-only display)
            bool selected = object->IsSelected();
            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
            ImGui::BeginDisabled();
            ImGui::Checkbox("Selected", &selected);
            ImGui::EndDisabled();
            ImGui::PopStyleColor(); // Object info
            ImGui::Spacing();
            ImGui::TextWrapped("Mesh: %s", object->GetMesh() ? "Loaded" : "None");

            if (object->GetMesh())
            {
                ImGui::SameLine();
                ImGui::TextColored(ImVec4(0.5f, 0.8f, 0.5f, 1.0f), "(Ready)");
            }
        }
    }
}
