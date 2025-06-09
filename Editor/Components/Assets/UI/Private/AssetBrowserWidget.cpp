#include "AssetBrowserWidget.h"
#include "GlobalAssetProvider.h"
#include "LocalAssetProvider.h"
#include "AssetFilter.h"
#include "UserDataManager.h"
#include "FileOperations.h"
#include "Workspace.h"
#include "EditorApp.h"
#include "Console.h"
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
          m_ViewMode(AssetViewMode::Grid), m_IconSize(80.0f), m_NeedsRefresh(true),
          m_CurrentAssetView(AssetView::Global), m_RecursiveSearch(false)
    {
        // Initialize asset providers
        m_GlobalProvider = std::make_shared<GlobalAssetProvider>();
        m_LocalProvider = std::make_shared<LocalAssetProvider>();

        m_CurrentDirectory = UserDataManager::GetGlobalAssetsDirectory();

        // Set initial provider based on current view
        SetAssetView(m_CurrentAssetView);

        if (m_Provider)
        {
            Refresh();
        }
    }

    AssetBrowserWidget::AssetBrowserWidget(std::shared_ptr<AssetProvider> globalProvider,
                                           std::shared_ptr<AssetProvider> localProvider,
                                           AssetRenderer &renderer,
                                           AssetOperations &operations,
                                           AssetDragDrop &dragDrop)
        : m_Provider(globalProvider), m_Renderer(renderer), m_Operations(operations), m_DragDrop(dragDrop),
          m_ViewMode(AssetViewMode::Grid), m_IconSize(80.0f), m_NeedsRefresh(true),
          m_CurrentAssetView(AssetView::Global), m_RecursiveSearch(false)
    {
        // Use the provided asset providers
        m_GlobalProvider = globalProvider;
        m_LocalProvider = localProvider;

        m_CurrentDirectory = UserDataManager::GetGlobalAssetsDirectory();

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
                Console::PrintError("Error during recursive search: " + std::string(e.what()));
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
            {
                m_Provider = m_LocalProvider;
                // Set to current workspace assets directory
                auto workspace = Voltray::Editor::EditorApp::Get()->GetCurrentWorkspace();
                if (workspace && workspace->IsPathValid())
                {
                    SetCurrentDirectory(workspace->path);
                }
                else
                {
                    // If no valid workspace, set to user data directory
                    SetCurrentDirectory(".");
                }
            }
            break;
            }

            m_NeedsRefresh = true;
        }
    }

    AssetView AssetBrowserWidget::GetAssetView() const
    {
        return m_CurrentAssetView;
    }

    void AssetBrowserWidget::UpdateLocalProvider(const std::filesystem::path &path)
    {
        if (m_LocalProvider)
        {
            // Reinitialize the local provider with the new path
            if (auto localProvider = std::dynamic_pointer_cast<LocalAssetProvider>(m_LocalProvider))
            {
                localProvider->Initialize(path.string());

                // If we're currently viewing local assets, refresh the view
                if (m_CurrentAssetView == AssetView::Local)
                {
                    SetCurrentDirectory(path / "Assets");
                    m_NeedsRefresh = true;
                }
            }
        }
    }

    void AssetBrowserWidget::RenderToolbar()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));

        // Asset view selector
        const char *viewNames[] = {"Global", "Local"};
        int currentView = static_cast<int>(m_CurrentAssetView);
        ImGui::Text("Assets:");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(100.0f);
        if (ImGui::Combo("##AssetView", &currentView, viewNames, 2))
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
        strncpy(searchBuffer, m_SearchFilter.c_str(), sizeof(searchBuffer) - 1);
        searchBuffer[sizeof(searchBuffer) - 1] = '\0';

        if (ImGui::InputText("##Search", searchBuffer, sizeof(searchBuffer)))
        {
            m_SearchFilter = std::string(searchBuffer);
            m_NeedsRefresh = true;
        } // Recursive search checkbox
        ImGui::SameLine();
        if (ImGui::Checkbox("Recursive", &m_RecursiveSearch))
        {
            m_NeedsRefresh = true;
        }

        // Filter controls
        ImGui::SameLine();
        ImGui::Separator();
        ImGui::SameLine();

        // Filter button that opens popup
        if (ImGui::Button("Filters"))
        {
            ImGui::OpenPopup("FilterOptions");
        }

        // Filter popup
        RenderFilterPopup();

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
                    // Open file with default system application (like in file explorer)
                    if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                    {
                        try
                        {
                            std::filesystem::path filePath = item.path;
                            if (std::filesystem::exists(filePath))
                            {
#ifdef _WIN32
                                // Windows - use ShellExecute
                                std::string command = "start \"\" \"" + filePath.string() + "\"";
                                system(command.c_str());
#elif __APPLE__
                                // macOS - use open command
                                std::string command = "open \"" + filePath.string() + "\"";
                                system(command.c_str());
#else
                                // Linux - use xdg-open
                                std::string command = "xdg-open \"" + filePath.string() + "\"";
                                system(command.c_str());
#endif
                            }
                        }
                        catch (const std::exception &e)
                        {
                            // Handle error silently or log if logging system available
                            Console::PrintError("Failed to open file: " + std::string(e.what()));
                        }
                    }
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

    void AssetBrowserWidget::RenderFilterPopup()
    {
        if (ImGui::BeginPopup("FilterOptions"))
        {
            ImGui::Text("File Filter Options");
            ImGui::Separator();

            if (!m_Provider)
            {
                ImGui::Text("No provider available");
                ImGui::EndPopup();
                return;
            }

            AssetFilter &filter = m_Provider->GetAssetFilter();
            bool changed = false;

            // Category filtering options
            ImGui::Text("Filter Categories:");
            ImGui::Indent();

            // System files
            bool systemFiltered = filter.IsCategoryFiltered(AssetFilterCategory::System);
            if (ImGui::Checkbox("System Files (tmp, cache, logs)", &systemFiltered))
            {
                filter.SetCategoryFiltering(AssetFilterCategory::System, systemFiltered);
                changed = true;
            }

            // Project files
            bool projectFiltered = filter.IsCategoryFiltered(AssetFilterCategory::Project);
            if (ImGui::Checkbox("Project Files (vcxproj, cmake, etc.)", &projectFiltered))
            {
                filter.SetCategoryFiltering(AssetFilterCategory::Project, projectFiltered);
                changed = true;
            }

            // IDE files
            bool ideFiltered = filter.IsCategoryFiltered(AssetFilterCategory::IDE);
            if (ImGui::Checkbox("IDE Files (.vscode, .vs, etc.)", &ideFiltered))
            {
                filter.SetCategoryFiltering(AssetFilterCategory::IDE, ideFiltered);
                changed = true;
            }

            // Version control files
            bool vcFiltered = filter.IsCategoryFiltered(AssetFilterCategory::VersionControl);
            if (ImGui::Checkbox("Version Control (.git, .svn, etc.)", &vcFiltered))
            {
                filter.SetCategoryFiltering(AssetFilterCategory::VersionControl, vcFiltered);
                changed = true;
            }

            // Archive files
            bool archiveFiltered = filter.IsCategoryFiltered(AssetFilterCategory::Archive);
            if (ImGui::Checkbox("Archive Files (zip, rar, etc.)", &archiveFiltered))
            {
                filter.SetCategoryFiltering(AssetFilterCategory::Archive, archiveFiltered);
                changed = true;
            }

            // Executable files
            bool execFiltered = filter.IsCategoryFiltered(AssetFilterCategory::Executable);
            if (ImGui::Checkbox("Executable Files (exe, dll, etc.)", &execFiltered))
            {
                filter.SetCategoryFiltering(AssetFilterCategory::Executable, execFiltered);
                changed = true;
            }

            // Media files
            bool mediaFiltered = filter.IsCategoryFiltered(AssetFilterCategory::Media);
            if (ImGui::Checkbox("Large Media Files (video, audio, 3D)", &mediaFiltered))
            {
                filter.SetCategoryFiltering(AssetFilterCategory::Media, mediaFiltered);
                changed = true;
            }

            // Document files
            bool docFiltered = filter.IsCategoryFiltered(AssetFilterCategory::Document);
            if (ImGui::Checkbox("Document Files (pdf, doc, etc.)", &docFiltered))
            {
                filter.SetCategoryFiltering(AssetFilterCategory::Document, docFiltered);
                changed = true;
            }

            ImGui::Unindent();
            ImGui::Separator();

            // Reset to defaults button
            if (ImGui::Button("Reset to Defaults"))
            {
                filter.ResetToDefaults();
                changed = true;
            }

            ImGui::SameLine();

            // Custom extension input
            static char customExtBuffer[64] = "";
            ImGui::Text("Add Custom Filter:");
            ImGui::SetNextItemWidth(120.0f);
            if (ImGui::InputText("##CustomExt", customExtBuffer, sizeof(customExtBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
            {
                std::string ext = customExtBuffer;
                if (!ext.empty())
                {
                    filter.AddFilteredExtension(ext);
                    customExtBuffer[0] = '\0'; // Clear input
                    changed = true;
                }
            }

            if (changed)
            {
                m_NeedsRefresh = true;
            }

            ImGui::EndPopup();
        }
    }
}
