#pragma once
#include <imgui.h>
#include <filesystem>
#include <string>
#include <vector>

/**
 * @file NavigationBar.h
 * @brief Navigation bar component for asset browser
 */

namespace Editor
{
    namespace Assets
    {
        /**
         * @class NavigationBar
         * @brief Provides breadcrumb navigation and path controls for asset browser
         */
        class NavigationBar
        {
        public:
            /**
             * @brief Render the navigation bar
             * @param currentPath Current directory path
             * @param rootPath Root directory path
             * @param isGlobal Whether this is global assets view
             * @return New path if navigation occurred, empty if no change
             */
            static std::filesystem::path Render(const std::filesystem::path &currentPath,
                                                const std::filesystem::path &rootPath,
                                                bool isGlobal);

            /**
             * @brief Render just the breadcrumb path
             * @param currentPath Current directory path
             * @param rootPath Root directory path
             * @return New path if navigation occurred, empty if no change
             */
            static std::filesystem::path RenderBreadcrumbs(const std::filesystem::path &currentPath,
                                                           const std::filesystem::path &rootPath);

            /**
             * @brief Render navigation buttons (home, back, forward)
             * @param currentPath Current directory path
             * @param rootPath Root directory path
             * @return New path if navigation occurred, empty if no change
             */
            static std::filesystem::path RenderNavigationButtons(const std::filesystem::path &currentPath,
                                                                 const std::filesystem::path &rootPath);

            /**
             * @brief Get relative path from root as string
             * @param fullPath Full path
             * @param rootPath Root path
             * @return Relative path string
             */
            static std::string GetRelativePath(const std::filesystem::path &fullPath,
                                               const std::filesystem::path &rootPath);

        private:
            /**
             * @brief Parse path into clickable segments
             * @param relativePath Relative path string
             * @return Vector of path segments
             */
            static std::vector<std::string> ParsePathSegments(const std::string &relativePath);

            /**
             * @brief Build path from root and segments up to index
             * @param rootPath Root path
             * @param segments Path segments
             * @param upToIndex Build path up to this segment index
             * @return Constructed path
             */
            static std::filesystem::path BuildPathFromSegments(const std::filesystem::path &rootPath,
                                                               const std::vector<std::string> &segments,
                                                               size_t upToIndex);
        };
    }
}
