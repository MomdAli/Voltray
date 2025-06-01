#include "ViewportInput.h"
#include "../../Engine/Graphics/Camera/BaseCamera.h"
#include "../../Engine/Scene/Scene.h"
#include "../../Engine/Scene/SceneObject.h"
#include "../../Engine/Input/Input.h"
#include "../../Math/Ray.h"
#include "../../Math/Mat4.h"
#include <GLFW/glfw3.h>
#include <limits>

namespace Editor::Components
{
    void ViewportInput::ProcessInput(::Scene &scene, ::BaseCamera &camera, const ImVec2 &viewportPos, const ImVec2 &viewportSize)
    {
        // Update global input
        ::Input::Update();

        // Handle camera controls (middle mouse button is handled globally in Camera class)
        handleCameraControls();

        // Handle F key for focus on selected object (smooth animation)
        if (ImGui::IsItemHovered() && ::Input::IsKeyPressed(GLFW_KEY_F))
        {
            auto selectedObject = scene.GetSelectedObject();
            if (selectedObject)
            {
                Vec3 objectPosition = selectedObject->GetTransform().GetPosition();
                camera.FocusOnObjectSmooth(objectPosition, 0.5f);
            }
        }

        // Handle object selection on left mouse click
        if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
            handleObjectSelection(scene, camera, viewportPos, viewportSize);
        }
    }
    void ViewportInput::handleObjectSelection(::Scene &scene, ::BaseCamera &camera, const ImVec2 &viewportPos, const ImVec2 &viewportSize)
    {
        ImVec2 mousePos = ImGui::GetMousePos();

        // Check if mouse is within viewport bounds
        ImVec2 relativePos = getRelativeMousePosition(mousePos, viewportPos);
        if (relativePos.x < 0 || relativePos.x >= viewportSize.x ||
            relativePos.y < 0 || relativePos.y >= viewportSize.y)
        {
            return;
        }

        // Create ray from camera through mouse position (using viewport-relative coordinates)
        Ray ray = camera.ScreenToWorldRay(relativePos.x, relativePos.y);

        std::shared_ptr<SceneObject> closestObject = nullptr;
        float closestDistance = std::numeric_limits<float>::max();
        const auto &objects = scene.GetObjects();
        for (const auto &obj : objects)
        {
            if (obj)
            {

                // First, perform AABB intersection test for early rejection
                Vec3 minBounds, maxBounds;
                obj->GetWorldBounds(minBounds, maxBounds);

                float aabbDistance;
                if (!ray.IntersectAABB(minBounds, maxBounds, aabbDistance))
                {
                    // Ray doesn't intersect the AABB, skip expensive mesh intersection
                    continue;
                }

                // AABB test passed, now do detailed mesh intersection
                float intersectionDistance = 0.0f;
                auto mesh = obj->GetMesh();
                if (mesh)
                {
                    // Get the object's model matrix (local to world transformation)
                    Mat4 modelMatrix = obj->GetModelMatrix();
                    if (ray.IntersectMesh(mesh->GetVertices(), mesh->GetIndices(), modelMatrix, intersectionDistance))
                    {
                        // Check if this is the closest object
                        if (intersectionDistance < closestDistance)
                        {
                            closestDistance = intersectionDistance;
                            closestObject = obj;
                        }
                    }
                }
            }
        }

        // Select the closest object or clear selection
        if (closestObject)
        {
            scene.SelectObject(closestObject);
        }
        else
        {
            scene.ClearSelection();
        }
    }

    void ViewportInput::handleCameraControls()
    {
        // Camera controls are now handled globally in the Camera class
        // The middle mouse button behavior is no longer viewport-constrained when held
        // No additional viewport-specific camera handling needed here
    }

    bool ViewportInput::isMouseInViewport(const ImVec2 &mousePos, const ImVec2 &viewportPos, const ImVec2 &viewportSize) const
    {
        ImVec2 relativePos = getRelativeMousePosition(mousePos, viewportPos);
        return (relativePos.x >= 0 && relativePos.x < viewportSize.x &&
                relativePos.y >= 0 && relativePos.y < viewportSize.y);
    }

    ImVec2 ViewportInput::getRelativeMousePosition(const ImVec2 &mousePos, const ImVec2 &viewportPos) const
    {
        return ImVec2(mousePos.x - viewportPos.x, mousePos.y - viewportPos.y);
    }
}
