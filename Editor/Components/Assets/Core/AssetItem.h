#pragma once
#include <filesystem>
#include <string>

/**
 * @file AssetItem.h
 * @brief Core data structures for asset items
 */

namespace Editor
{
    namespace Assets
    {
        /**
         * @enum AssetViewMode
         * @brief Different ways to display assets in the browser
         */
        enum class AssetViewMode
        {
            Grid,   // Large icons in a grid
            List,   // List view with small icons
            Details // Detailed list with file information
        };

        /**
         * @struct AssetItem
         * @brief Represents a single asset item in the file browser
         */
        struct AssetItem
        {
            std::string name;
            std::filesystem::path path;
            bool isDirectory = false;
            bool isGlobal = false; // True if it's a global asset, false if workspace-specific
            size_t fileSize = 0;
            std::string extension;
            bool isParentDir = false; // For ".." navigation

            // Additional metadata
            std::string mimeType;
            bool isHidden = false;
            bool isReadOnly = false;
            std::filesystem::file_time_type lastModified;
        };

        /**
         * @struct AssetBrowserState
         * @brief State information for the asset browser
         */
        struct AssetBrowserState
        {
            AssetViewMode viewMode = AssetViewMode::Grid;
            float iconSize = 70.0f;
            int selectedItem = -1;
            bool showHiddenFiles = false;
            std::string searchFilter;
            std::string sortBy = "name"; // "name", "size", "date", "type"
            bool sortAscending = true;
        };
    }
}
