#pragma once
#include "AssetItem.h"
#include <vector>
#include <functional>

/**
 * @file AssetManager.h
 * @brief Manages asset operations and data
 */

namespace Voltray::Editor::Components::Assets
{

    /**
     * @class AssetManager
     * @brief Handles asset data management, file operations, and state
     */
    class AssetManager
    {
    public:
        /**
         * @brief Initialize the asset manager
         */
        void Initialize();

        /**
         * @brief Get directory contents as asset items
         * @param directory Directory to scan
         * @param isGlobal Whether these are global assets
         * @param state Browser state for filtering/sorting
         * @return Vector of asset items
         */
        std::vector<AssetItem> GetDirectoryContents(
            const std::filesystem::path &directory,
            bool isGlobal,
            const AssetBrowserState &state);

        /**
         * @brief Check if a file should be displayed based on filters
         * @param path File path to check
         * @param state Browser state with filters
         * @return True if file should be shown
         */
        bool ShouldShowFile(const std::filesystem::path &path, const AssetBrowserState &state);

        /**
         * @brief Sort asset items based on criteria
         * @param items Items to sort
         * @param sortBy Sort criteria
         * @param ascending Sort direction
         */
        void SortAssets(std::vector<AssetItem> &items, const std::string &sortBy, bool ascending);

        /**
         * @brief Get MIME type for a file
         * @param extension File extension
         * @return MIME type string
         */
        std::string GetMimeType(const std::string &extension);

        /**
         * @brief Get file category for UI purposes
         * @param extension File extension
         * @return Category string (3D, Image, Audio, etc.)
         */
        std::string GetFileCategory(const std::string &extension);

        /**
         * @brief Create a new folder at the specified location
         * @param parentPath Parent directory
         * @param folderName Name of the new folder
         * @return True if successful
         */
        bool CreateFolder(const std::filesystem::path &parentPath, const std::string &folderName);

        /**
         * @brief Create a new script file
         * @param parentPath Parent directory
         * @param scriptName Name of the script
         * @param scriptType Type of script (C#, JavaScript, etc.)
         * @return True if successful
         */
        bool CreateScript(const std::filesystem::path &parentPath, const std::string &scriptName, const std::string &scriptType);

        /**
         * @brief Delete an asset
         * @param assetPath Path to the asset
         * @return True if successful
         */
        bool DeleteAsset(const std::filesystem::path &assetPath);

        /**
         * @brief Rename an asset
         * @param oldPath Current path
         * @param newName New name
         * @return True if successful
         */
        bool RenameAsset(const std::filesystem::path &oldPath, const std::string &newName);

        /**
         * @brief Check if an asset can be dragged
         * @param item Asset item to check
         * @return True if draggable
         */
        bool CanDragAsset(const AssetItem &item);

        /**
         * @brief Get drag data for an asset
         * @param item Asset item
         * @return Drag data string
         */
        std::string GetDragData(const AssetItem &item);

    private:
        /**
         * @brief Apply search filter to asset name
         * @param name Asset name
         * @param filter Search filter
         * @return True if matches filter
         */
        bool MatchesFilter(const std::string &name, const std::string &filter);
    };
}
