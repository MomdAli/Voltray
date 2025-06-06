#include "AssetBrowserWidget.h"
#include "GlobalAssetProvider.h"
#include "LocalAssetProvider.h"
#include "UserDataManager.h"
#include "FileOperations.h"
#include <imgui.h>
#include <algorithm>
#include <filesystem>

using Voltray::Utils::UserDataManager;

namespace Voltray::Editor::Components::Assets
{
    AssetBrowserWidget::AssetBrowserWidget(std::shared_ptr<AssetProvider> provider,
                                           AssetRenderer &renderer,
                                           AssetOperations &operations,
                                           AssetDragDrop &dragDrop)
        : m_Provider(provider), m_Renderer(renderer), m_Operations(operations), m_DragDrop(dragDrop),
          m_CurrentDirectory("."), m_ViewMode(AssetViewMode::Grid), m_IconSize(64.0f), m_NeedsRefresh(true),
          m_CurrentAssetView(AssetView::Global), m_RecursiveSearch(false)
    {
        // Initialize asset providers
        m_GlobalProvider = std::make_shared<GlobalAssetProvider>();
        m_LocalProvider = std::make_shared<LocalAssetProvider>();

        // Set initial provider based on current view
        SetAssetView(m_CurrentAssetView);

        if (m_Provider)
        {
            Refresh();
        }
    }

    void AssetBrowserWidget::Draw(const ImVec2 &availableSize)
    {
        // Draw toolbar with view controls and search
        RenderToolbar();

        // Refresh directory if needed
        if (m_NeedsRefresh)
        {
            Refresh();
            m_NeedsRefresh = false;
        }

        // Draw navigation breadcrumb
        RenderNavigation();

        // Calculate content area
        ImVec2 contentSize = ImVec2(availableSize.x, availableSize.y - 60.0f); // Reserve space for toolbar and breadcrumb

        // Draw main content area
        if (ImGui::BeginChild("AssetContent", contentSize, false, ImGuiWindowFlags_HorizontalScrollbar))
        {
            // Filter items based on search
            auto filteredItems = FilterItems(m_CurrentItems); // Render filtered items based on view mode
            if (m_ViewMode == AssetViewMode::Grid)
            {
                std::string selectedPath = m_Renderer.RenderGridView(filteredItems, m_IconSize, contentSize.x);
                HandleItemInteraction(selectedPath);
            }
            else if (m_ViewMode == AssetViewMode::List)
            {
                std::string selectedPath = m_Renderer.RenderListView(filteredItems);
                HandleItemInteraction(selectedPath);
            }
            else // Details view
            {
                std::string selectedPath = m_Renderer.RenderDetailsView(filteredItems);
                HandleItemInteraction(selectedPath);
            }
        }
        ImGui::EndChild();
    }

    void AssetBrowserWidget::SetCurrentDirectory(const std::filesystem::path &directory)
    {
        if (m_CurrentDirectory != directory)
        {
            m_CurrentDirectory = directory;
            m_NeedsRefresh = true;
        }
    }

    void AssetBrowserWidget::SetIconSize(float size)
    {
        size = std::clamp(size, 32.0f, 128.0f);
        if (m_IconSize != size)
        {
            m_IconSize = size;
        }
    }
    void AssetBrowserWidget::Refresh()
    {
        if (!m_Provider)
            return;

        // Get directory contents using the provider interface
        m_CurrentItems = m_Provider->GetDirectoryContents(m_CurrentDirectory, m_SearchFilter);

        // If recursive search is enabled and we have a search filter, add recursive results
        if (m_RecursiveSearch && !m_SearchFilter.empty())
        {
            try
            {
                // Use FileOperations to find files recursively
                auto recursiveFiles = FileOperations::FindFiles(m_CurrentDirectory, "*" + m_SearchFilter + "*", true);

                // Convert to AssetItems and add to current items
                for (const auto &filePath : recursiveFiles)
                {
                    // Skip if already in current items
                    bool alreadyExists = false;
                    for (const auto &item : m_CurrentItems)
                    {
                        if (item.path == filePath)
                        {
                            alreadyExists = true;
                            break;
                        }
                    }

                    if (!alreadyExists)
                    {
                        AssetItem item;
                        item.path = filePath;
                        item.name = filePath.filename().string();
                        item.isDirectory = std::filesystem::is_directory(filePath);
                        // item.size = item.isDirectory ? 0 : std::filesystem::file_size(filePath);
                        auto relativePath = std::filesystem::relative(filePath, m_CurrentDirectory);
                        // item.relativePath = relativePath.string();
                        m_CurrentItems.push_back(item);
                    }
                }
            }
            catch (const std::exception &e)
            {
                // Handle any filesystem errors gracefully
                // Could log error here if logging system is available
            }
        }
    }

    void AssetBrowserWidget::SetAssetView(AssetView view)
    {
        if (m_CurrentAssetView != view)
        {
            m_CurrentAssetView = view;

            // Switch to appropriate provider
            switch (view)
            {
            case AssetView::Global:
                m_Provider = m_GlobalProvider;
                // Set to global assets directory
                {
                    auto globalAssetsPath = UserDataManager::GetGlobalAssetsDirectory();
                    SetCurrentDirectory(globalAssetsPath);
                }
                break;

            case AssetView::Local:
                m_Provider = m_LocalProvider;
                // Set to local workspace directory
                SetCurrentDirectory(".");
                break;

            case AssetView::Scene:
                m_Provider = m_LocalProvider; // For now, use local provider for scene assets
                // TODO: Implement scene-specific provider or filtering
                SetCurrentDirectory(".");
                break;
            }

            m_NeedsRefresh = true;
        }
    }

    AssetView AssetBrowserWidget::GetAssetView() const
    {
        return m_CurrentAssetView;
    }

    void AssetBrowserWidget::RenderToolbar()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));

        // Asset view selector
        const char *viewNames[] = {"Global", "Local", "Scene"};
        int currentView = static_cast<int>(m_CurrentAssetView);
        ImGui::Text("Assets:");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(100.0f);
        if (ImGui::Combo("##AssetView", &currentView, viewNames, 3))
        {
            SetAssetView(static_cast<AssetView>(currentView));
        }

        ImGui::SameLine();
        ImGui::Separator();
        ImGui::SameLine();

        // View mode buttons
        if (ImGui::Button("Grid"))
        {
            SetViewMode(AssetViewMode::Grid);
        }
        ImGui::SameLine();
        if (ImGui::Button("List"))
        {
            SetViewMode(AssetViewMode::List);
        }
        ImGui::SameLine();
        if (ImGui::Button("Details"))
        {
            SetViewMode(AssetViewMode::Details);
        }

        ImGui::SameLine();
        ImGui::Separator();
        ImGui::SameLine();

        // Icon size slider (only visible in grid mode)
        if (m_ViewMode == AssetViewMode::Grid)
        {
            ImGui::Text("Size:");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(100);
            float iconSize = m_IconSize;
            if (ImGui::SliderFloat("##IconSize", &iconSize, 32.0f, 128.0f, "%.0f"))
            {
                SetIconSize(iconSize);
            }
            ImGui::SameLine();
        } // Search box
        ImGui::Text("Search:");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(150.0f);

        char searchBuffer[256];
        strncpy_s(searchBuffer, m_SearchFilter.c_str(), sizeof(searchBuffer) - 1);
        searchBuffer[sizeof(searchBuffer) - 1] = '\0';

        if (ImGui::InputText("##Search", searchBuffer, sizeof(searchBuffer)))
        {
            m_SearchFilter = std::string(searchBuffer);
            m_NeedsRefresh = true;
        }

        // Recursive search checkbox
        ImGui::SameLine();
        if (ImGui::Checkbox("Recursive", &m_RecursiveSearch))
        {
            m_NeedsRefresh = true;
        }

        // Refresh button
        ImGui::SameLine();
        if (ImGui::Button("Refresh"))
        {
            m_NeedsRefresh = true;
        }

        ImGui::PopStyleVar();
    }

    void AssetBrowserWidget::RenderNavigation()
    {
        // Parse current path into components
        std::vector<std::string> pathComponents;
        std::string currentPath = m_CurrentDirectory.string();

        // Handle root path
        if (currentPath == "." || currentPath.empty())
        {
            pathComponents.push_back("Root");
        }
        else
        {
            // Split path by directory separator
            std::string delimiter = "/";
            if (currentPath.find('\\') != std::string::npos)
                delimiter = "\\";

            size_t pos = 0;
            std::string token;
            while ((pos = currentPath.find(delimiter)) != std::string::npos)
            {
                token = currentPath.substr(0, pos);
                if (!token.empty())
                    pathComponents.push_back(token);
                currentPath.erase(0, pos + delimiter.length());
            }
            if (!currentPath.empty())
                pathComponents.push_back(currentPath);
        }

        // Draw breadcrumb navigation
        for (size_t i = 0; i < pathComponents.size(); ++i)
        {
            if (i > 0)
            {
                ImGui::SameLine();
                ImGui::Text(">");
                ImGui::SameLine();
            }

            if (ImGui::Button(pathComponents[i].c_str()))
            {
                // Navigate to this path level
                std::filesystem::path newPath = ".";
                for (size_t j = 0; j <= i && j < pathComponents.size(); ++j)
                {
                    if (pathComponents[j] != "Root")
                    {
                        if (newPath == ".")
                            newPath = pathComponents[j];
                        else
                            newPath = newPath / pathComponents[j];
                    }
                }
                NavigateTo(newPath);
            }
        }
    }

    void AssetBrowserWidget::NavigateTo(const std::filesystem::path &path)
    {
        SetCurrentDirectory(path);
    }

    std::vector<AssetItem> AssetBrowserWidget::FilterItems(const std::vector<AssetItem> &items) const
    {
        if (m_SearchFilter.empty())
        {
            return items;
        }

        std::vector<AssetItem> filteredItems;
        std::string lowerFilter = m_SearchFilter;
        std::transform(lowerFilter.begin(), lowerFilter.end(), lowerFilter.begin(), ::tolower);

        for (const auto &item : items)
        {
            std::string lowerName = item.name;
            std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);

            if (lowerName.find(lowerFilter) != std::string::npos)
            {
                filteredItems.push_back(item);
            }
        }

        return filteredItems;
    }

    void AssetBrowserWidget::HandleItemInteraction(const std::string &selectedPath)
    {
        if (selectedPath.empty())
            return;

        // Find the selected item
        for (const auto &item : m_CurrentItems)
        {
            if (item.path.string() == selectedPath)
            {
                if (item.isDirectory)
                {
                    // Navigate into directory on double-click
                    if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                    {
                        NavigateTo(item.path);
                    }
                }
                else
                {
                    // Handle file selection/opening
                    // TODO: Implement file opening logic
                }
                break;
            }
        }

        // Handle context menu
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
        {
            // Find item under mouse
            for (const auto &item : m_CurrentItems)
            {
                if (item.path.string() == selectedPath)
                {
                    if (m_Operations.ShowContextMenu(item, m_Provider))
                    {
                        m_NeedsRefresh = true;
                    }
                    break;
                }
            }
        }
    }
}
