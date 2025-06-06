#include "IconRenderer.h"
#include <algorithm>
#include <cctype>

namespace Voltray::Editor::Components::Assets
{
    void IconRenderer::RenderFileIcon(
        ImDrawList *drawList,
        ImVec2 center,
        float size,
        const std::string &extension,
        bool isDirectory,
        bool isParentDir,
        bool isSelected)
    {
        if (isParentDir)
        {
            RenderParentDirIcon(drawList, center, size, isSelected);
        }
        else if (isDirectory)
        {
            RenderFolderIcon(drawList, center, size, isSelected);
        }
        else
        {
            // Convert extension to lowercase for comparison
            std::string ext = extension;
            std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

            if (ext == "obj" || ext == "fbx" || ext == "gltf" || ext == "glb" || ext == "3ds" || ext == "dae")
            {
                Render3DModelIcon(drawList, center, size, isSelected);
            }
            else if (ext == "png" || ext == "jpg" || ext == "jpeg" || ext == "bmp" || ext == "tga" || ext == "gif")
            {
                RenderImageIcon(drawList, center, size, isSelected);
            }
            else if (ext == "wav" || ext == "mp3" || ext == "ogg" || ext == "flac")
            {
                RenderAudioIcon(drawList, center, size, isSelected);
            }
            else if (ext == "txt" || ext == "md" || ext == "rtf")
            {
                RenderTextIcon(drawList, center, size, isSelected);
            }
            else if (ext == "cpp" || ext == "h" || ext == "cs" || ext == "js" || ext == "py" || ext == "java")
            {
                RenderCodeIcon(drawList, center, size, isSelected);
            }
            else if (ext == "scene")
            {
                RenderSceneIcon(drawList, center, size, isSelected);
            }
            else if (ext == "mat")
            {
                RenderMaterialIcon(drawList, center, size, isSelected);
            }
            else
            {
                RenderGenericFileIcon(drawList, center, size, isSelected);
            }
        }
    }

    void IconRenderer::RenderSmallIcon(
        ImDrawList *drawList,
        ImVec2 center,
        const std::string &extension,
        bool isDirectory)
    {
        const float smallSize = 16.0f;
        RenderFileIcon(drawList, center, smallSize, extension, isDirectory, false, false);
    }

    ImU32 IconRenderer::GetFileTypeColor(const std::string &extension, bool isDirectory)
    {
        if (isDirectory)
            return FOLDER_COLOR;

        std::string ext = extension;
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

        if (ext == "obj" || ext == "fbx" || ext == "gltf" || ext == "glb" || ext == "3ds" || ext == "dae")
            return MODEL_COLOR;
        else if (ext == "png" || ext == "jpg" || ext == "jpeg" || ext == "bmp" || ext == "tga" || ext == "gif")
            return IMAGE_COLOR;
        else if (ext == "wav" || ext == "mp3" || ext == "ogg" || ext == "flac")
            return AUDIO_COLOR;
        else if (ext == "txt" || ext == "md" || ext == "rtf")
            return TEXT_COLOR;
        else if (ext == "cpp" || ext == "h" || ext == "cs" || ext == "js" || ext == "py" || ext == "java")
            return CODE_COLOR;
        else if (ext == "scene")
            return SCENE_COLOR;
        else if (ext == "mat")
            return MATERIAL_COLOR;
        else
            return GENERIC_COLOR;
    }

    void IconRenderer::RenderFolderIcon(ImDrawList *drawList, ImVec2 center, float size, bool isSelected)
    {
        const float folderWidth = size * 0.8f;
        const float folderHeight = size * 0.6f;

        ImU32 color = isSelected ? IM_COL32(255, 220, 120, 255) : FOLDER_COLOR;
        ImU32 shadowColor = IM_COL32(255, 180, 80, 255);

        // Folder body
        drawList->AddRectFilled(
            ImVec2(center.x - folderWidth * 0.5f, center.y - folderHeight * 0.3f),
            ImVec2(center.x + folderWidth * 0.5f, center.y + folderHeight * 0.7f),
            color);

        // Folder tab
        drawList->AddRectFilled(
            ImVec2(center.x - folderWidth * 0.3f, center.y - folderHeight * 0.7f),
            ImVec2(center.x + folderWidth * 0.1f, center.y - folderHeight * 0.3f),
            shadowColor);

        // Add outline for selected state
        if (isSelected)
        {
            drawList->AddRect(
                ImVec2(center.x - folderWidth * 0.5f, center.y - folderHeight * 0.3f),
                ImVec2(center.x + folderWidth * 0.5f, center.y + folderHeight * 0.7f),
                IM_COL32(255, 255, 255, 200), 0.0f, 0, 2.0f);
        }
    }

    void IconRenderer::RenderParentDirIcon(ImDrawList *drawList, ImVec2 center, float size, bool isSelected)
    {
        ImU32 color = isSelected ? IM_COL32(120, 170, 255, 255) : PARENT_COLOR;

        // Up arrow for parent directory
        ImVec2 points[3] = {
            ImVec2(center.x, center.y - size * 0.3f),               // Top
            ImVec2(center.x - size * 0.3f, center.y + size * 0.1f), // Bottom left
            ImVec2(center.x + size * 0.3f, center.y + size * 0.1f)  // Bottom right
        };
        drawList->AddTriangleFilled(points[0], points[1], points[2], color);

        // Arrow stem
        drawList->AddRectFilled(
            ImVec2(center.x - size * 0.1f, center.y - size * 0.1f),
            ImVec2(center.x + size * 0.1f, center.y + size * 0.4f),
            color);
    }

    void IconRenderer::Render3DModelIcon(ImDrawList *drawList, ImVec2 center, float size, bool isSelected)
    {
        const float cubeSize = size * 0.6f;
        ImVec2 cubeMin = ImVec2(center.x - cubeSize * 0.5f, center.y - cubeSize * 0.5f);
        ImVec2 cubeMax = ImVec2(center.x + cubeSize * 0.5f, center.y + cubeSize * 0.5f);

        ImU32 color = isSelected ? IM_COL32(170, 170, 255, 255) : MODEL_COLOR;
        ImU32 shadowColor = IM_COL32(100, 100, 200, 255);

        // Front face
        drawList->AddRect(cubeMin, cubeMax, color, 0.0f, 0, 2.0f);

        // Back face (offset)
        ImVec2 offset = ImVec2(cubeSize * 0.2f, -cubeSize * 0.2f);
        drawList->AddRect(
            ImVec2(cubeMin.x + offset.x, cubeMin.y + offset.y),
            ImVec2(cubeMax.x + offset.x, cubeMax.y + offset.y),
            shadowColor, 0.0f, 0, 2.0f);

        // Connect corners
        drawList->AddLine(cubeMin, ImVec2(cubeMin.x + offset.x, cubeMin.y + offset.y), shadowColor, 1.0f);
        drawList->AddLine(ImVec2(cubeMax.x, cubeMin.y), ImVec2(cubeMax.x + offset.x, cubeMin.y + offset.y), shadowColor, 1.0f);
        drawList->AddLine(ImVec2(cubeMin.x, cubeMax.y), ImVec2(cubeMin.x + offset.x, cubeMax.y + offset.y), shadowColor, 1.0f);
        drawList->AddLine(cubeMax, ImVec2(cubeMax.x + offset.x, cubeMax.y + offset.y), shadowColor, 1.0f);
    }

    void IconRenderer::RenderImageIcon(ImDrawList *drawList, ImVec2 center, float size, bool isSelected)
    {
        const float frameSize = size * 0.8f;
        ImVec2 frameMin = ImVec2(center.x - frameSize * 0.5f, center.y - frameSize * 0.5f);
        ImVec2 frameMax = ImVec2(center.x + frameSize * 0.5f, center.y + frameSize * 0.5f);

        ImU32 frameColor = isSelected ? IM_COL32(120, 255, 170, 255) : IMAGE_COLOR;

        // Frame background
        drawList->AddRectFilled(frameMin, frameMax, IM_COL32(80, 80, 80, 255));
        drawList->AddRect(frameMin, frameMax, frameColor, 0.0f, 0, 2.0f);

        // Simple landscape (mountains and sun)
        ImVec2 mountain1[3] = {
            ImVec2(center.x - frameSize * 0.3f, frameMax.y - 4),
            ImVec2(center.x - frameSize * 0.1f, center.y - frameSize * 0.1f),
            ImVec2(center.x + frameSize * 0.1f, frameMax.y - 4)};
        drawList->AddTriangleFilled(mountain1[0], mountain1[1], mountain1[2], IM_COL32(100, 150, 100, 255));

        // Sun
        drawList->AddCircleFilled(
            ImVec2(center.x + frameSize * 0.2f, center.y - frameSize * 0.2f),
            frameSize * 0.08f,
            IM_COL32(255, 255, 100, 255));
    }

    void IconRenderer::RenderAudioIcon(ImDrawList *drawList, ImVec2 center, float size, bool isSelected)
    {
        const float speakerSize = size * 0.4f;
        ImU32 color = isSelected ? IM_COL32(255, 170, 120, 255) : AUDIO_COLOR;

        // Speaker cone
        drawList->AddTriangleFilled(
            ImVec2(center.x - speakerSize * 0.5f, center.y - speakerSize * 0.3f),
            ImVec2(center.x - speakerSize * 0.5f, center.y + speakerSize * 0.3f),
            ImVec2(center.x, center.y),
            color);

        // Speaker base
        drawList->AddRectFilled(
            ImVec2(center.x - speakerSize * 0.7f, center.y - speakerSize * 0.2f),
            ImVec2(center.x - speakerSize * 0.5f, center.y + speakerSize * 0.2f),
            color);

        // Sound waves
        for (int i = 1; i <= 3; i++)
        {
            float radius = speakerSize * 0.3f * i;
            ImU32 waveColor = IM_COL32(255, 150, 100, 200 - i * 50);
            drawList->AddCircle(center, radius, waveColor, 12, 1.5f);
        }
    }

    void IconRenderer::RenderTextIcon(ImDrawList *drawList, ImVec2 center, float size, bool isSelected)
    {
        const float docWidth = size * 0.6f;
        const float docHeight = size * 0.8f;
        ImVec2 docMin = ImVec2(center.x - docWidth * 0.5f, center.y - docHeight * 0.5f);
        ImVec2 docMax = ImVec2(center.x + docWidth * 0.5f, center.y + docHeight * 0.5f);

        ImU32 color = isSelected ? IM_COL32(220, 220, 220, 255) : TEXT_COLOR;

        // Document background
        drawList->AddRectFilled(docMin, docMax, IM_COL32(240, 240, 240, 255));
        drawList->AddRect(docMin, docMax, color, 0.0f, 0, 1.5f);

        // Text lines
        for (int i = 0; i < 4; i++)
        {
            float lineY = docMin.y + docHeight * 0.2f + i * docHeight * 0.15f;
            float lineWidth = (i == 3) ? docWidth * 0.5f : docWidth * 0.8f; // Last line shorter
            drawList->AddLine(
                ImVec2(docMin.x + docWidth * 0.1f, lineY),
                ImVec2(docMin.x + docWidth * 0.1f + lineWidth, lineY),
                IM_COL32(100, 100, 100, 255), 1.0f);
        }
    }

    void IconRenderer::RenderCodeIcon(ImDrawList *drawList, ImVec2 center, float size, bool isSelected)
    {
        const float docWidth = size * 0.6f;
        const float docHeight = size * 0.8f;
        ImVec2 docMin = ImVec2(center.x - docWidth * 0.5f, center.y - docHeight * 0.5f);
        ImVec2 docMax = ImVec2(center.x + docWidth * 0.5f, center.y + docHeight * 0.5f);

        ImU32 bgColor = isSelected ? IM_COL32(60, 60, 80, 255) : IM_COL32(40, 40, 60, 255);
        ImU32 borderColor = isSelected ? IM_COL32(120, 255, 120, 255) : CODE_COLOR;

        // Document background
        drawList->AddRectFilled(docMin, docMax, bgColor);
        drawList->AddRect(docMin, docMax, borderColor, 0.0f, 0, 1.5f);

        // Code brackets and text
        const char *codeSymbols[] = {"{ }", "< >", "[ ]"};
        for (int i = 0; i < 3; i++)
        {
            ImVec2 textPos = ImVec2(
                center.x - ImGui::CalcTextSize(codeSymbols[i]).x * 0.5f,
                docMin.y + docHeight * 0.25f + i * docHeight * 0.2f);
            drawList->AddText(textPos, borderColor, codeSymbols[i]);
        }
    }

    void IconRenderer::RenderSceneIcon(ImDrawList *drawList, ImVec2 center, float size, bool isSelected)
    {
        const float gridSize = size * 0.8f;
        ImU32 color = isSelected ? IM_COL32(255, 120, 255, 255) : SCENE_COLOR;
        ImU32 accentColor = IM_COL32(100, 150, 255, 255);

        // Grid lines
        for (int i = 0; i <= 4; i++)
        {
            float offset = (i - 2) * gridSize * 0.2f;
            // Horizontal lines
            drawList->AddLine(
                ImVec2(center.x - gridSize * 0.4f, center.y + offset),
                ImVec2(center.x + gridSize * 0.4f, center.y + offset),
                color, 1.0f);
            // Vertical lines
            drawList->AddLine(
                ImVec2(center.x + offset, center.y - gridSize * 0.4f),
                ImVec2(center.x + offset, center.y + gridSize * 0.4f),
                color, 1.0f);
        }

        // Center highlight
        drawList->AddCircleFilled(center, size * 0.08f, accentColor);
    }

    void IconRenderer::RenderMaterialIcon(ImDrawList *drawList, ImVec2 center, float size, bool isSelected)
    {
        ImU32 color = isSelected ? IM_COL32(170, 120, 220, 255) : MATERIAL_COLOR;

        // Material sphere with shading
        drawList->AddCircleFilled(center, size * 0.4f, color);

        // Highlight
        drawList->AddCircleFilled(
            ImVec2(center.x - size * 0.15f, center.y - size * 0.15f),
            size * 0.15f,
            IM_COL32(255, 255, 255, 180));

        // Reflection
        drawList->AddCircleFilled(
            ImVec2(center.x + size * 0.1f, center.y + size * 0.1f),
            size * 0.08f,
            IM_COL32(255, 255, 255, 120));
    }

    void IconRenderer::RenderGenericFileIcon(ImDrawList *drawList, ImVec2 center, float size, bool isSelected)
    {
        const float docWidth = size * 0.6f;
        const float docHeight = size * 0.8f;
        ImVec2 docMin = ImVec2(center.x - docWidth * 0.5f, center.y - docHeight * 0.5f);
        ImVec2 docMax = ImVec2(center.x + docWidth * 0.5f, center.y + docHeight * 0.5f);

        ImU32 color = isSelected ? IM_COL32(200, 200, 200, 255) : GENERIC_COLOR;

        // Document
        drawList->AddRectFilled(docMin, docMax, IM_COL32(200, 200, 200, 255));
        drawList->AddRect(docMin, docMax, color, 0.0f, 0, 1.5f);

        // Folded corner
        ImVec2 cornerSize = ImVec2(docWidth * 0.2f, docHeight * 0.2f);
        drawList->AddTriangleFilled(
            ImVec2(docMax.x - cornerSize.x, docMin.y),
            ImVec2(docMax.x, docMin.y + cornerSize.y),
            docMax,
            IM_COL32(180, 180, 180, 255));

        // Corner fold line
        drawList->AddLine(
            ImVec2(docMax.x - cornerSize.x, docMin.y),
            ImVec2(docMax.x - cornerSize.x, docMin.y + cornerSize.y),
            color, 1.0f);
        drawList->AddLine(
            ImVec2(docMax.x - cornerSize.x, docMin.y + cornerSize.y),
            ImVec2(docMax.x, docMin.y + cornerSize.y), color, 1.0f);
    }
}
