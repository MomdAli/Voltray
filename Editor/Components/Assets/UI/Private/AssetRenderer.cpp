#include "AssetRenderer.h"
#include "AssetDragDrop.h"
#include <algorithm>
#include <cmath>
#include <filesystem>

namespace Voltray::Editor::Components::Assets
{
    AssetRenderer::AssetRenderer(IconRenderer &iconRenderer)
        : m_IconRenderer(iconRenderer)
    {
    }

    std::string AssetRenderer::RenderGridView(const std::vector<AssetItem> &items,
                                              float iconSize,
                                              float availableWidth)
    {
        std::string selectedPath;

        const int columns = CalculateGridColumns(iconSize, availableWidth);
        const float itemWidth = iconSize + 16.0f;
        const float itemHeight = iconSize + 32.0f;

        int currentColumn = 0;

        for (const auto &item : items)
        {
            if (currentColumn > 0)
            {
                ImGui::SameLine();
            }

            ImGui::BeginGroup();

            // Create invisible button for interaction
            const std::string buttonId = "##item_" + item.name;
            bool clicked = ImGui::InvisibleButton(buttonId.c_str(), ImVec2(itemWidth, itemHeight));
            bool doubleClicked = ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0);

            if (clicked || doubleClicked)
            {
                selectedPath = item.path.string();
            }

            // Get item bounds for rendering
            ImVec2 itemMin = ImGui::GetItemRectMin();
            ImVec2 itemMax = ImGui::GetItemRectMax();

            // Calculate icon position (centered)
            ImVec2 iconPos;
            iconPos.x = itemMin.x + (itemWidth - iconSize) * 0.5f;
            iconPos.y = itemMin.y + 4.0f; // Render icon
            bool isHovered = ImGui::IsItemHovered();
            m_IconRenderer.RenderFileIcon(
                ImGui::GetWindowDrawList(),
                ImVec2(iconPos.x + iconSize * 0.5f, iconPos.y + iconSize * 0.5f),
                iconSize,
                item.path.extension().string(),
                item.isDirectory,
                selectedPath == item.path.string(), // Add selection state
                isHovered);

            // Render filename for both files and folders, but keep it clean
            const ImVec2 textSize = ImGui::CalcTextSize(item.name.c_str());
            ImVec2 textPos;
            textPos.x = itemMin.x + (itemWidth - textSize.x) * 0.5f;
            textPos.y = iconPos.y + iconSize + 4.0f;

            // Clamp text to item bounds
            textPos.x = std::max(textPos.x, itemMin.x + 2.0f);
            textPos.x = std::min(textPos.x, itemMax.x - textSize.x - 2.0f);

            const ImU32 textColor = GetAssetTextColor(item);
            ImGui::GetWindowDrawList()->AddText(textPos, textColor, item.name.c_str());

            // Remove global indicator overlay to keep clean icons

            // Handle drag and drop for files
            if (!item.isDirectory && !item.isParentDir)
            {
                AssetDragDrop::BeginDrag(
                    item.path,
                    item.path.extension().string(),
                    item.isGlobal);
            }

            ImGui::EndGroup();

            currentColumn++;
            if (currentColumn >= columns)
            {
                currentColumn = 0;
            }
        }

        return selectedPath;
    }

    std::string AssetRenderer::RenderListView(const std::vector<AssetItem> &items)
    {
        std::string selectedPath;

        if (ImGui::BeginTable("AssetList", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg))
        {
            ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, 100.0f);
            ImGui::TableHeadersRow();

            for (const auto &item : items)
            {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();

                // Render small icon + name
                ImVec2 iconSize(16.0f, 16.0f);
                ImVec2 cursorPos = ImGui::GetCursorScreenPos();

                m_IconRenderer.RenderSmallIcon(
                    ImGui::GetWindowDrawList(),
                    ImVec2(cursorPos.x + iconSize.x * 0.5f, cursorPos.y + iconSize.y * 0.5f),
                    item.path.extension().string(),
                    item.isDirectory);

                ImGui::SetCursorScreenPos(ImVec2(cursorPos.x + iconSize.x + 4.0f, cursorPos.y));

                bool clicked = ImGui::Selectable(item.name.c_str(), false, ImGuiSelectableFlags_SpanAllColumns);
                bool doubleClicked = ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0);

                if (clicked || doubleClicked)
                {
                    selectedPath = item.path.string();
                }

                // Handle drag and drop for files
                if (!item.isDirectory && !item.isParentDir)
                {
                    AssetDragDrop::BeginDrag(
                        item.path,
                        item.path.extension().string(),
                        item.isGlobal);
                }

                ImGui::TableNextColumn();
                if (item.isDirectory)
                {
                    ImGui::Text("Folder");
                }
                else
                {
                    // TODO: Implement proper file type detection
                    ImGui::Text("File");
                }
            }

            ImGui::EndTable();
        }

        return selectedPath;
    }

    std::string AssetRenderer::RenderDetailsView(const std::vector<AssetItem> &items)
    {
        std::string selectedPath;

        if (ImGui::BeginTable("AssetDetails", 4,
                              ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_Sortable))
        {
            ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, 100.0f);
            ImGui::TableSetupColumn("Size", ImGuiTableColumnFlags_WidthFixed, 80.0f);
            ImGui::TableSetupColumn("Modified", ImGuiTableColumnFlags_WidthFixed, 120.0f);
            ImGui::TableHeadersRow();

            for (const auto &item : items)
            {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();

                // Render small icon + name
                ImVec2 iconSize(16.0f, 16.0f);
                ImVec2 cursorPos = ImGui::GetCursorScreenPos();

                m_IconRenderer.RenderSmallIcon(
                    ImGui::GetWindowDrawList(),
                    ImVec2(cursorPos.x + iconSize.x * 0.5f, cursorPos.y + iconSize.y * 0.5f),
                    item.path.extension().string(),
                    item.isDirectory);

                ImGui::SetCursorScreenPos(ImVec2(cursorPos.x + iconSize.x + 4.0f, cursorPos.y));

                bool clicked = ImGui::Selectable(item.name.c_str(), false, ImGuiSelectableFlags_SpanAllColumns);
                bool doubleClicked = ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0);

                if (clicked || doubleClicked)
                {
                    selectedPath = item.path.string();
                }

                // Handle drag and drop for files
                if (!item.isDirectory && !item.isParentDir)
                {
                    AssetDragDrop::BeginDrag(
                        item.path,
                        item.path.extension().string(),
                        item.isGlobal);
                }

                ImGui::TableNextColumn();
                if (item.isDirectory)
                {
                    ImGui::Text("Folder");
                }
                else
                {
                    // TODO: Implement proper file type detection
                    ImGui::Text("File");
                }

                ImGui::TableNextColumn();
                if (item.isDirectory)
                {
                    ImGui::Text("--");
                }
                else
                {
                    ImGui::Text("%lu KB", static_cast<unsigned long>(item.fileSize / 1024));
                }

                ImGui::TableNextColumn();
                // TODO: Format timestamp properly
                ImGui::Text("Modified");
            }

            ImGui::EndTable();
        }

        return selectedPath;
    }

    int AssetRenderer::CalculateGridColumns(float iconSize, float availableWidth) const
    {
        const float itemWidth = iconSize + 16.0f;
        const int columns = static_cast<int>(availableWidth / itemWidth);
        return std::max(1, columns);
    }

    ImU32 AssetRenderer::GetAssetTextColor(const AssetItem &item) const
    {
        if (item.isGlobal)
        {
            return IM_COL32(180, 180, 120, 255); // Yellowish for global
        }
        return IM_COL32(200, 200, 200, 255); // Light gray for local
    }
}
