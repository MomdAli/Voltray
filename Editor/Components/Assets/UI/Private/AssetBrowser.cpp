#include "AssetBrowser.h"
#include <imgui.h>
#include <algorithm>
#include <cstring>
#include <cmath>

namespace Voltray::Editor::Components::Assets
{
    AssetBrowser::AssetBrowser(AssetManager &assetManager,
                               IconRenderer &iconRenderer,
                               ContextMenu &contextMenu,
                               NavigationBar &navigationBar,
                               AssetDragDrop &dragDrop)
        : m_assetManager(assetManager), m_iconRenderer(iconRenderer), m_contextMenu(contextMenu),
          m_navigationBar(navigationBar), m_dragDrop(dragDrop), m_currentPath(".")
    {
        RefreshCurrentDirectory();
    }

    void AssetBrowser::Draw(const ImVec2 &windowSize)
    {
        // Navigation bar
        auto navigationResult = NavigationBar::Render(m_currentPath, m_currentPath, false);
        if (!navigationResult.empty())
        {
            NavigateToDirectory(navigationResult.string());
        }

        // Toolbar with view mode controls and search
        RenderToolbar(windowSize.x);

        // Refresh directory if needed
        if (m_needsRefresh)
        {
            AssetBrowserState state;
            state.viewMode = m_viewMode;
            state.searchFilter = m_searchFilter;
            m_currentItems = m_assetManager.GetDirectoryContents(m_currentPath, false, state);
            m_needsRefresh = false;
        }

        // Filter items based on search
        std::vector<AssetItem> filteredItems;
        for (const auto &item : m_currentItems)
        {
            if (PassesSearchFilter(item))
            {
                filteredItems.push_back(item);
            }
        }

        // Render assets based on view mode
        ImGui::BeginChild("AssetView", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
        switch (m_viewMode)
        {
        case AssetViewMode::Grid:
            RenderAssetGrid();
            break;
        case AssetViewMode::List:
            RenderAssetList();
            break;
        case AssetViewMode::Details:
            RenderAssetDetails();
            break;
        }

        // Handle context menu
        if (ImGui::BeginPopupContextWindow())
        {
            // Simple context menu for now
            if (ImGui::MenuItem("Refresh"))
            {
                RefreshCurrentDirectory();
            }
            ImGui::EndPopup();
        }

        ImGui::EndChild();

        // Handle drag and drop rendering
        AssetDragDrop::RenderDragPreview();
    }

    void AssetBrowser::SetCurrentPath(const std::string &path)
    {
        if (m_currentPath != path)
        {
            m_currentPath = path;
            m_needsRefresh = true;
        }
    }

    void AssetBrowser::SetIconSize(float size)
    {
        m_iconSize = std::clamp(size, 32.0f, 128.0f);
    }

    void AssetBrowser::SetSearchFilter(const std::string &filter)
    {
        m_searchFilter = filter;
    }

    void AssetBrowser::RefreshCurrentDirectory()
    {
        m_needsRefresh = true;
    }
    void AssetBrowser::RenderToolbar(float windowWidth)
    {
        (void)windowWidth; // Suppress unused parameter warning
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 4));

        // View mode buttons
        if (ImGui::Button("Grid"))
        {
            m_viewMode = AssetViewMode::Grid;
        }
        ImGui::SameLine();
        if (ImGui::Button("List"))
        {
            m_viewMode = AssetViewMode::List;
        }
        ImGui::SameLine();
        if (ImGui::Button("Details"))
        {
            m_viewMode = AssetViewMode::Details;
        }
        ImGui::SameLine();
        ImGui::Text("|");
        ImGui::SameLine();

        // Icon size slider (only for grid view)
        if (m_viewMode == AssetViewMode::Grid)
        {
            ImGui::Text("Size:");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(100);
            ImGui::SliderFloat("##IconSize", &m_iconSize, 32.0f, 128.0f, "%.0f");
            ImGui::SameLine();
        }

        // Search box
        ImGui::Text("Search:");
        ImGui::SameLine();

        char searchBuffer[256];
        strncpy(searchBuffer, m_searchFilter.c_str(), sizeof(searchBuffer) - 1);
        searchBuffer[sizeof(searchBuffer) - 1] = '\0';

        ImGui::SetNextItemWidth(200);
        if (ImGui::InputText("##Search", searchBuffer, sizeof(searchBuffer)))
        {
            m_searchFilter = searchBuffer;
        }

        ImGui::PopStyleVar();
        ImGui::Separator();
    }

    void AssetBrowser::RenderAssetGrid()
    {
        // Calculate grid layout
        ImVec2 itemSize = CalculateGridItemSize();
        (void)itemSize; // Suppress unused variable warning - used for layout calculations
        float columns = CalculateColumnsForGrid(ImGui::GetContentRegionAvail().x);

        int currentColumn = 0;

        for (const auto &item : m_currentItems)
        {
            if (!PassesSearchFilter(item))
                continue;

            if (currentColumn > 0)
            {
                ImGui::SameLine();
            }

            ImGui::BeginGroup();
            RenderAssetItem(item, true);
            ImGui::EndGroup();

            currentColumn++;
            if (currentColumn >= static_cast<int>(columns))
            {
                currentColumn = 0;
            }
        }
    }

    void AssetBrowser::RenderAssetList()
    {
        // Table with filename and type
        if (ImGui::BeginTable("AssetList", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg))
        {
            ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, 100);
            ImGui::TableHeadersRow();

            for (const auto &item : m_currentItems)
            {
                if (!PassesSearchFilter(item))
                    continue;

                ImGui::TableNextRow();
                ImGui::TableNextColumn();

                RenderAssetItem(item, false);
                ImGui::TableNextColumn();
                ImGui::Text("File"); // TODO: Implement GetFileTypeDisplayName
            }

            ImGui::EndTable();
        }
    }

    void AssetBrowser::RenderAssetDetails()
    {
        // Detailed table with name, type, size, and date
        if (ImGui::BeginTable("AssetDetails", 4, ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_Sortable))
        {
            ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, 100);
            ImGui::TableSetupColumn("Size", ImGuiTableColumnFlags_WidthFixed, 80);
            ImGui::TableSetupColumn("Modified", ImGuiTableColumnFlags_WidthFixed, 120);
            ImGui::TableHeadersRow();

            for (const auto &item : m_currentItems)
            {
                if (!PassesSearchFilter(item))
                    continue;

                ImGui::TableNextRow();
                ImGui::TableNextColumn();

                RenderAssetItem(item, false);
                ImGui::TableNextColumn();
                ImGui::Text("File"); // TODO: Implement GetFileTypeDisplayName

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
                ImGui::Text("Modified"); // TODO: Format time properly
            }

            ImGui::EndTable();
        }
    }
    void AssetBrowser::RenderAssetItem(const AssetItem &item, bool isGrid)
    {
        ImGuiID id = ImGui::GetID(item.path.c_str());
        (void)id; // Suppress unused variable warning - may be used for future features

        // Calculate item dimensions
        ImVec2 itemSize = isGrid ? CalculateGridItemSize() : ImVec2(0, 0);

        // Create invisible button for interaction
        bool clicked = false;
        bool doubleClicked = false;

        if (isGrid)
        {
            clicked = ImGui::InvisibleButton(("##item" + item.name).c_str(), itemSize);
            doubleClicked = ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0);
        }
        else
        {
            // For list view, just render the icon and name inline
            ImVec2 iconSize(16, 16); // Render small icon
            ImVec2 cursorPos = ImGui::GetCursorScreenPos();
            IconRenderer::RenderSmallIcon(ImGui::GetWindowDrawList(),
                                          ImVec2(cursorPos.x + iconSize.x / 2, cursorPos.y + iconSize.y / 2),
                                          std::filesystem::path(item.path).extension().string(),
                                          item.isDirectory);

            ImGui::SetCursorScreenPos(ImVec2(cursorPos.x + iconSize.x + 4, cursorPos.y));
            clicked = ImGui::Selectable(item.name.c_str(), false, ImGuiSelectableFlags_SpanAllColumns); // TODO: Implement selection state
            doubleClicked = ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0);
        }

        // Handle selection
        if (clicked)
        {
            HandleItemSelection(item);
        }

        // Handle double-click
        if (doubleClicked)
        {
            HandleItemDoubleClick(item);
        }

        // Handle drag and drop for non-directories
        if (!item.isDirectory && ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
        {
            AssetDragDrop::BeginDrag(item.path,
                                     std::filesystem::path(item.path).extension().string(),
                                     false);
            ImGui::EndDragDropSource();
        }

        // Context menu
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Open"))
            {
                // TODO: Implement file opening
            }
            if (ImGui::MenuItem("Rename"))
            {
                // TODO: Implement rename
            }
            if (ImGui::MenuItem("Delete"))
            {
                // TODO: Implement delete
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Refresh"))
            {
                RefreshCurrentDirectory();
            }
            ImGui::EndPopup();
        }

        // For grid view, render icon and label
        if (isGrid)
        {
            ImVec2 itemMin = ImGui::GetItemRectMin();
            ImVec2 itemMax = ImGui::GetItemRectMax();

            // Calculate icon position (centered horizontally)
            ImVec2 iconSize(m_iconSize * 0.7f, m_iconSize * 0.7f);
            ImVec2 iconPos;
            iconPos.x = itemMin.x + (itemSize.x - iconSize.x) * 0.5f;
            iconPos.y = itemMin.y + 4;

            // Render icon
            bool isHovered = ImGui::IsItemHovered();
            IconRenderer::RenderFileIcon(ImGui::GetWindowDrawList(),
                                         ImVec2(iconPos.x + iconSize.x / 2, iconPos.y + iconSize.y / 2),
                                         iconSize.x,
                                         std::filesystem::path(item.path).extension().string(),
                                         item.isDirectory,
                                         false,
                                         isHovered); // TODO: Implement selection state

            // Render filename below icon
            ImVec2 textSize = ImGui::CalcTextSize(item.name.c_str());
            ImVec2 textPos;
            textPos.x = itemMin.x + (itemSize.x - textSize.x) * 0.5f;
            textPos.y = iconPos.y + iconSize.y + 4;

            // Clamp text position to item bounds
            textPos.x = std::max(textPos.x, itemMin.x + 2);
            textPos.x = std::min(textPos.x, itemMax.x - textSize.x - 2);
            ImGui::GetWindowDrawList()->AddText(
                nullptr,
                0.0f,
                textPos,
                IM_COL32(200, 200, 200, 255),
                item.name.c_str());
        }
    }

    void AssetBrowser::NavigateToDirectory(const std::string &path)
    {
        SetCurrentPath(path);
    }

    void AssetBrowser::OnDirectoryDoubleClick(const std::string &path)
    {
        NavigateToDirectory(path);
    }

    ImVec2 AssetBrowser::CalculateGridItemSize() const
    {
        return ImVec2(m_iconSize + 16, m_iconSize + 32); // Extra space for text
    }

    float AssetBrowser::CalculateColumnsForGrid(float availableWidth) const
    {
        ImVec2 itemSize = CalculateGridItemSize();
        float columns = std::floor(availableWidth / itemSize.x);
        return std::max(1.0f, columns);
    }

    bool AssetBrowser::PassesSearchFilter(const AssetItem &item) const
    {
        if (m_searchFilter.empty())
        {
            return true;
        }

        std::string lowerFilename = item.name;
        std::string lowerFilter = m_searchFilter;

        std::transform(lowerFilename.begin(), lowerFilename.end(), lowerFilename.begin(), ::tolower);
        std::transform(lowerFilter.begin(), lowerFilter.end(), lowerFilter.begin(), ::tolower);

        return lowerFilename.find(lowerFilter) != std::string::npos;
    }
    void AssetBrowser::HandleItemSelection(const AssetItem &item)
    {
        (void)item; // Suppress unused parameter warning
        // TODO: Implement selection state management
        // For now, just store the selected item path
        // This should be moved to a proper selection manager later
    }

    void AssetBrowser::HandleItemDoubleClick(const AssetItem &item)
    {
        if (item.isDirectory)
        {
            OnDirectoryDoubleClick(item.path.string());
        }
        else
        {
            // TODO: Handle file opening based on type            // For now, just print to console or handle appropriately
        }
    }
}
