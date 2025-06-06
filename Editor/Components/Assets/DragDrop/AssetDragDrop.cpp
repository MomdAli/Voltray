#include "AssetDragDrop.h"
#include "../../EditorApp.h"
#include "../../Components/Console.h"
#include "../../../Engine/Scene/Scene.h"
#include "../../../Engine/Scene/SceneObject.h"
#include "../../../Engine/Graphics/Camera/BaseCamera.h"
#include "../../../Math/Ray.h"
#include "../../../Math/Vec3.h"
#include <algorithm>

using namespace Editor::Components;

namespace Editor
{
    namespace Assets
    {
        // Static member definitions
        DragDropPayload AssetDragDrop::s_currentPayload;
        bool AssetDragDrop::s_isDragging = false;

        bool AssetDragDrop::BeginDrag(const std::filesystem::path &assetPath, const std::string &assetType, bool isGlobal)
        {
            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
            {
                s_currentPayload = DragDropPayload(assetPath, assetType, isGlobal);
                s_isDragging = true;

                // Set payload data
                ImGui::SetDragDropPayload(DRAG_DROP_ID, &s_currentPayload, sizeof(DragDropPayload));

                // Render drag preview
                ImGui::Text("Dragging: %s", s_currentPayload.fileName.c_str());
                ImGui::Text("Type: %s", s_currentPayload.assetType.c_str());

                ImGui::EndDragDropSource();
                return true;
            }

            return false;
        }

        const DragDropPayload *AssetDragDrop::AcceptDrop()
        {
            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload(DRAG_DROP_ID))
                {
                    const DragDropPayload *assetPayload = static_cast<const DragDropPayload *>(payload->Data);
                    s_isDragging = false;
                    ImGui::EndDragDropTarget();
                    return assetPayload;
                }
                ImGui::EndDragDropTarget();
            }
            return nullptr;
        }

        bool AssetDragDrop::CanAcceptDrop(const std::string &targetType)
        {
            if (!s_isDragging)
                return false;

            // Define compatibility rules
            if (targetType == "viewport")
            {
                return s_currentPayload.assetType == "model" ||
                       s_currentPayload.assetType == "3d" ||
                       s_currentPayload.assetType == "mesh" ||
                       s_currentPayload.assetType == "texture" ||
                       s_currentPayload.assetType == "script";
            }
            else if (targetType == "scene")
            {
                return s_currentPayload.assetType == "model" ||
                       s_currentPayload.assetType == "3d" ||
                       s_currentPayload.assetType == "mesh" ||
                       s_currentPayload.assetType == "script";
            }
            else if (targetType == "inspector")
            {
                return s_currentPayload.assetType == "texture" ||
                       s_currentPayload.assetType == "material" ||
                       s_currentPayload.assetType == "script";
            }

            return false;
        }

        const DragDropPayload *AssetDragDrop::GetDragPayload()
        {
            return s_isDragging ? &s_currentPayload : nullptr;
        }

        bool AssetDragDrop::HandleViewportDrop(const DragDropPayload &payload, const ImVec2 &dropPosition)
        {
            auto *editorApp = EditorApp::Get();
            if (!editorApp || !editorApp->GetViewport())
            {
                Console::PrintError("Cannot handle viewport drop: Editor or viewport not available");
                return false;
            }

            // Get viewport scene
            auto &viewportScene = editorApp->GetViewport()->GetScene();
            auto &scene = viewportScene.GetScene();

            try
            {
                if (payload.assetType == "model" || payload.assetType == "3d" || payload.assetType == "mesh")
                {
                    return LoadModelAsset(payload.assetPath, dropPosition);
                }
                else if (payload.assetType == "texture")
                {
                    return ApplyTextureAsset(payload.assetPath);
                }
                else if (payload.assetType == "script")
                {
                    return AttachScriptAsset(payload.assetPath);
                }

                Console::PrintWarning("Unsupported asset type for viewport drop: " + payload.assetType);
                return false;
            }
            catch (const std::exception &e)
            {
                Console::PrintError("Error handling viewport drop: " + std::string(e.what()));
                return false;
            }
        }

        bool AssetDragDrop::HandleSceneDrop(const DragDropPayload &payload)
        {
            // For scene drops, we'll place objects at the origin
            return HandleViewportDrop(payload, ImVec2(0, 0));
        }

        void AssetDragDrop::RenderDragPreview()
        {
            if (!s_isDragging)
                return;

            // Render a preview overlay during drag operation
            ImDrawList *drawList = ImGui::GetForegroundDrawList();
            ImVec2 mousePos = ImGui::GetMousePos();

            // Draw a small preview icon
            ImVec2 iconSize(32, 32);
            ImVec2 iconMin = ImVec2(mousePos.x + 10, mousePos.y + 10);
            ImVec2 iconMax = ImVec2(iconMin.x + iconSize.x, iconMin.y + iconSize.y);

            // Background
            drawList->AddRectFilled(iconMin, iconMax, IM_COL32(50, 50, 50, 200), 4.0f);
            drawList->AddRect(iconMin, iconMax, IM_COL32(100, 100, 100, 255), 4.0f, 0, 2.0f);

            // Simple icon based on asset type
            ImVec2 center = ImVec2(iconMin.x + iconSize.x * 0.5f, iconMin.y + iconSize.y * 0.5f);

            if (s_currentPayload.assetType == "model" || s_currentPayload.assetType == "3d")
            {
                // Draw a simple 3D cube icon
                drawList->AddRect(ImVec2(center.x - 8, center.y - 8), ImVec2(center.x + 8, center.y + 8),
                                  IM_COL32(100, 150, 255, 255), 0.0f, 0, 2.0f);
            }
            else if (s_currentPayload.assetType == "texture")
            {
                // Draw a simple image icon
                drawList->AddRectFilled(ImVec2(center.x - 8, center.y - 8), ImVec2(center.x + 8, center.y + 8),
                                        IM_COL32(255, 200, 100, 255));
            }
            else if (s_currentPayload.assetType == "script")
            {
                // Draw a simple document icon
                drawList->AddRect(ImVec2(center.x - 6, center.y - 8), ImVec2(center.x + 6, center.y + 8),
                                  IM_COL32(150, 255, 150, 255), 0.0f, 0, 2.0f);
            }

            // File name tooltip
            drawList->AddText(ImVec2(iconMax.x + 5, iconMin.y), IM_COL32(255, 255, 255, 255),
                              s_currentPayload.fileName.c_str());
        }

        bool AssetDragDrop::CreateSceneObjectFromAsset(const DragDropPayload &payload, const ImVec2 &position)
        {
            auto *editorApp = EditorApp::Get();
            if (!editorApp || !editorApp->GetViewport())
            {
                return false;
            }

            auto &viewportScene = editorApp->GetViewport()->GetScene();
            auto &scene = viewportScene.GetScene();
            auto &camera = viewportScene.GetCamera();

            // Convert screen position to world position using camera ray casting
            Ray ray = camera.ScreenToWorldRay(position.x, position.y);

            // Place object at a default distance along the ray
            float defaultDistance = 5.0f;
            Vec3 worldPosition = ray.origin + ray.direction * defaultDistance;

            // Create scene object at the calculated position
            // This will depend on your scene object creation system
            // For now, we'll just log the operation
            Console::Print("Creating scene object from asset: " + payload.fileName +
                           " at position (" + std::to_string(worldPosition.x) + ", " +
                           std::to_string(worldPosition.y) + ", " + std::to_string(worldPosition.z) + ")");

            return true;
        }

        bool AssetDragDrop::LoadModelAsset(const std::filesystem::path &assetPath, const ImVec2 &position)
        {
            // Check file extension to determine model type
            std::string extension = assetPath.extension().string();
            std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

            if (extension == ".obj" || extension == ".fbx" || extension == ".gltf" ||
                extension == ".glb" || extension == ".dae" || extension == ".3ds")
            {
                Console::Print("Loading 3D model: " + assetPath.string());

                // TODO: Implement actual model loading
                // This would involve:
                // 1. Loading the model using Assimp or similar
                // 2. Creating mesh and material data
                // 3. Creating a scene object
                // 4. Positioning it in the world

                return CreateSceneObjectFromAsset(DragDropPayload(assetPath, "model", false), position);
            }

            Console::PrintWarning("Unsupported model format: " + extension);
            return false;
        }

        bool AssetDragDrop::ApplyTextureAsset(const std::filesystem::path &assetPath)
        {
            auto *editorApp = EditorApp::Get();
            if (!editorApp || !editorApp->GetViewport())
            {
                return false;
            }

            auto &viewportScene = editorApp->GetViewport()->GetScene();
            auto &scene = viewportScene.GetScene();
            auto selectedObject = scene.GetSelectedObject();

            if (!selectedObject)
            {
                Console::PrintWarning("No object selected to apply texture to");
                return false;
            }

            // Check file extension
            std::string extension = assetPath.extension().string();
            std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

            if (extension == ".png" || extension == ".jpg" || extension == ".jpeg" ||
                extension == ".bmp" || extension == ".tga" || extension == ".dds")
            {
                Console::Print("Applying texture to selected object: " + assetPath.string());

                // TODO: Implement actual texture application
                // This would involve:
                // 1. Loading the texture
                // 2. Applying it to the selected object's material
                // 3. Updating the rendering

                return true;
            }

            Console::PrintWarning("Unsupported texture format: " + extension);
            return false;
        }

        bool AssetDragDrop::AttachScriptAsset(const std::filesystem::path &assetPath)
        {
            auto *editorApp = EditorApp::Get();
            if (!editorApp || !editorApp->GetViewport())
            {
                return false;
            }

            auto &viewportScene = editorApp->GetViewport()->GetScene();
            auto &scene = viewportScene.GetScene();
            auto selectedObject = scene.GetSelectedObject();

            if (!selectedObject)
            {
                Console::PrintWarning("No object selected to attach script to");
                return false;
            }

            std::string extension = assetPath.extension().string();
            std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

            if (extension == ".lua" || extension == ".js" || extension == ".py" || extension == ".cs")
            {
                Console::Print("Attaching script to selected object: " + assetPath.string());

                // TODO: Implement script attachment
                // This would involve:
                // 1. Loading the script
                // 2. Attaching it to the selected object
                // 3. Setting up script execution context

                return true;
            }

            Console::PrintWarning("Unsupported script format: " + extension);
            return false;
        }
    }
}
