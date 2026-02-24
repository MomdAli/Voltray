#include "NavigationBar.h"
#include <imgui.h>
#include <algorithm>

namespace Voltray::Editor::Components::Assets
{
    std::filesystem::path NavigationBar::Render(const std::filesystem::path &currentPath,
                                                const std::filesystem::path &rootPath,
                                                bool isGlobal)
    {
        (void)isGlobal; // Suppress unused parameter warning
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 6));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));

        std::filesystem::path newPath;

        // Navigation buttons
        auto navPath = RenderNavigationButtons(currentPath, rootPath);
        if (!navPath.empty())
        {
            newPath = navPath;
        }

        if (!newPath.empty())
        {
            ImGui::SameLine();
            ImGui::Text(">");
            ImGui::SameLine();
        }

        // Breadcrumbs
        auto breadcrumbPath = RenderBreadcrumbs(currentPath, rootPath);
        if (!breadcrumbPath.empty())
        {
            newPath = breadcrumbPath;
        }

        ImGui::PopStyleColor(2);
        ImGui::PopStyleVar();

        return newPath;
    }

    std::filesystem::path NavigationBar::RenderBreadcrumbs(const std::filesystem::path &currentPath,
                                                           const std::filesystem::path &rootPath)
    {
        std::filesystem::path newPath;

        // Build breadcrumb path
        std::string relativePath = GetRelativePath(currentPath, rootPath);

        if (relativePath.empty() || relativePath == ".")
        {
            ImGui::Text("Root");
        }
        else
        {
            // Split path by separators and create clickable breadcrumbs
            auto pathSegments = ParsePathSegments(relativePath);

            for (size_t i = 0; i < pathSegments.size(); ++i)
            {
                if (ImGui::Button(pathSegments[i].c_str()))
                {
                    newPath = BuildPathFromSegments(rootPath, pathSegments, i + 1);
                }

                if (i < pathSegments.size() - 1)
                {
                    ImGui::SameLine();
                    ImGui::Text(">");
                    ImGui::SameLine();
                }
            }
        }

        return newPath;
    }

    std::filesystem::path NavigationBar::RenderNavigationButtons(const std::filesystem::path &currentPath,
                                                                 const std::filesystem::path &rootPath)
    {
        std::filesystem::path newPath;

        // Home button
        if (ImGui::Button("Home"))
        {
            newPath = rootPath;
        }
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("Go to root");

        ImGui::SameLine();

        // Up directory button (if not at root)
        if (currentPath != rootPath && currentPath.has_parent_path())
        {
            if (ImGui::Button("Up"))
            {
                auto parentPath = currentPath.parent_path();
                // Don't go above root
                if (parentPath.string().length() >= rootPath.string().length())
                {
                    newPath = parentPath;
                }
                else
                {
                    newPath = rootPath;
                }
            }
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Go up one directory");

            ImGui::SameLine();
        }

        // Refresh button
        if (ImGui::Button("Refresh"))
        {
            newPath = currentPath; // Return current path to trigger refresh
        }
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("Refresh directory contents");

        return newPath;
    }

    std::string NavigationBar::GetRelativePath(const std::filesystem::path &fullPath,
                                               const std::filesystem::path &rootPath)
    {
        try
        {
            auto relativePath = std::filesystem::relative(fullPath, rootPath);
            return relativePath.string();
        }
        catch (const std::filesystem::filesystem_error &)
        {
            // If relative path calculation fails, return empty string
            return "";
        }
    }

    std::vector<std::string> NavigationBar::ParsePathSegments(const std::string &relativePath)
    {
        std::vector<std::string> segments;
        std::string segment;

        for (char c : relativePath)
        {
            if (c == '\\' || c == '/')
            {
                if (!segment.empty())
                {
                    segments.push_back(segment);
                    segment.clear();
                }
            }
            else
            {
                segment += c;
            }
        }

        if (!segment.empty())
        {
            segments.push_back(segment);
        }

        return segments;
    }

    std::filesystem::path NavigationBar::BuildPathFromSegments(const std::filesystem::path &rootPath,
                                                               const std::vector<std::string> &segments,
                                                               size_t upToIndex)
    {
        std::filesystem::path result = rootPath;

        for (size_t i = 0; i < upToIndex && i < segments.size(); ++i)
        {
            result /= segments[i];
        }
        return result;
    }
}
