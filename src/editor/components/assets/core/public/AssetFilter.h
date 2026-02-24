/**
 * @file AssetFilter.h
 * @brief Utilities for filtering assets based on file extensions and types
 * @author Voltray Engine Team
 * @date Created: 2025-01-24
 *
 * Provides comprehensive filtering functionality for asset providers,
 * including extension-based filtering and configurable file type management.
 */

#pragma once

#include <filesystem>
#include <string>
#include <unordered_set>
#include <vector>

namespace Voltray::Editor::Components::Assets
{
    /**
     * @enum AssetFilterCategory
     * @brief Categories of file types for filtering
     */
    enum class AssetFilterCategory
    {
        System,         // System files (temporary, cache, etc.)
        Project,        // Project and build files
        IDE,            // IDE-specific files
        VersionControl, // Version control files
        Archive,        // Archive and compressed files
        Executable,     // Executable files
        Media,          // Media files (potentially large)
        Document,       // Document files
        All             // All categories (for bulk operations)
    };

    /**
     * @class AssetFilter
     * @brief Handles file filtering based on extensions and categories
     *
     * Provides a centralized way to filter files in asset providers,
     * with support for different file categories and configurable filtering.
     */
    class AssetFilter
    {
    public:
        AssetFilter();

        /**
         * @brief Check if a file should be shown based on all filtering rules
         * @param path File path to check
         * @param searchFilter Optional search filter string
         * @return True if file should be displayed
         */
        bool ShouldShowFile(const std::filesystem::path &path, const std::string &searchFilter = "") const;

        /**
         * @brief Check if a file extension should be filtered out
         * @param extension File extension (with or without dot)
         * @return True if file should be hidden
         */
        bool IsFilteredExtension(const std::string &extension) const;

        /**
         * @brief Enable/disable filtering for a specific category
         * @param category Category to modify
         * @param enabled True to filter (hide) files in this category
         */
        void SetCategoryFiltering(AssetFilterCategory category, bool enabled);

        /**
         * @brief Check if filtering is enabled for a category
         * @param category Category to check
         * @return True if category is being filtered (hidden)
         */
        bool IsCategoryFiltered(AssetFilterCategory category) const;

        /**
         * @brief Add a custom extension to filter
         * @param extension Extension to filter (with or without dot)
         */
        void AddFilteredExtension(const std::string &extension);

        /**
         * @brief Remove a custom extension from filtering
         * @param extension Extension to stop filtering
         */
        void RemoveFilteredExtension(const std::string &extension);

        /**
         * @brief Get all extensions in a category
         * @param category Category to query
         * @return Vector of extensions in the category
         */
        std::vector<std::string> GetExtensionsInCategory(AssetFilterCategory category) const;

        /**
         * @brief Get display name for a category
         * @param category Category to get name for
         * @return Human-readable category name
         */
        static std::string GetCategoryDisplayName(AssetFilterCategory category);

        /**
         * @brief Reset filtering to default settings
         */
        void ResetToDefaults();

    private:
        // Category filtering enabled flags
        std::unordered_set<AssetFilterCategory> m_FilteredCategories;

        // Custom filtered extensions
        std::unordered_set<std::string> m_CustomFilteredExtensions;

        /**
         * @brief Initialize default filtered file extensions by category
         */
        void InitializeDefaultExtensions();

        /**
         * @brief Get the category for a file extension
         * @param extension File extension (normalized)
         * @return Category of the extension, or null if not categorized
         */
        AssetFilterCategory GetExtensionCategory(const std::string &extension) const;

        /**
         * @brief Normalize extension string (lowercase, with leading dot)
         * @param extension Extension to normalize
         * @return Normalized extension
         */
        std::string NormalizeExtension(const std::string &extension) const;

        /**
         * @brief Check if filename matches search filter
         * @param filename Filename to check
         * @param searchFilter Search filter string
         * @return True if filename matches filter
         */
        bool MatchesSearchFilter(const std::string &filename, const std::string &searchFilter) const;

        // Static extension mappings by category
        static const std::unordered_set<std::string> s_SystemExtensions;
        static const std::unordered_set<std::string> s_ProjectExtensions;
        static const std::unordered_set<std::string> s_IDEExtensions;
        static const std::unordered_set<std::string> s_VersionControlExtensions;
        static const std::unordered_set<std::string> s_ArchiveExtensions;
        static const std::unordered_set<std::string> s_ExecutableExtensions;
        static const std::unordered_set<std::string> s_MediaExtensions;
        static const std::unordered_set<std::string> s_DocumentExtensions;
    };
}
