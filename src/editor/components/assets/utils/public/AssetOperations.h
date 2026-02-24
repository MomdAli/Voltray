/**
 * @file AssetOperations.h
 * @brief Handles asset operations and context menu actions
 * @author Voltray Engine Team
 * @date Created: 2025-06-06
 *
 * Manages file operations like create, delete, rename and handles
 * context menu interactions for asset management.
 */

#pragma once

#include "AssetItem.h"
#include "AssetProvider.h"
#include <filesystem>
#include <string>
#include <memory>

namespace Voltray::Editor::Components::Assets
{
    /**
     * @enum OperationResult
     * @brief Result of an asset operation
     */
    enum class OperationResult
    {
        Success,
        Failed,
        Cancelled,
        NotSupported
    };

    /**
     * @class AssetOperations
     * @brief Handles asset file operations and context menu actions
     *
     * This class manages all asset operations including file creation,
     * deletion, renaming, and folder operations. It respects provider
     * capabilities (read-only vs modifiable).
     */
    class AssetOperations
    {
    public:
        /**
         * @brief Show context menu for an asset item
         * @param item Asset item to show menu for
         * @param provider Asset provider (for capability checking)
         * @return True if any action was performed
         */
        bool ShowContextMenu(const AssetItem &item, std::shared_ptr<AssetProvider> provider);

        /**
         * @brief Create a new folder
         * @param parentDirectory Parent directory path
         * @param folderName Name of new folder
         * @param provider Asset provider
         * @return Operation result
         */
        OperationResult CreateFolder(const std::filesystem::path &parentDirectory,
                                     const std::string &folderName,
                                     std::shared_ptr<AssetProvider> provider);

        /**
         * @brief Delete an asset or folder
         * @param path Path to delete
         * @param provider Asset provider
         * @return Operation result
         */
        OperationResult DeleteAsset(const std::filesystem::path &path,
                                    std::shared_ptr<AssetProvider> provider);

        /**
         * @brief Rename an asset or folder
         * @param oldPath Current path
         * @param newName New name
         * @param provider Asset provider
         * @return Operation result
         */
        OperationResult RenameAsset(const std::filesystem::path &oldPath,
                                    const std::string &newName,
                                    std::shared_ptr<AssetProvider> provider);

        /**
         * @brief Import an asset file
         * @param sourcePath Source file path
         * @param targetDirectory Target directory
         * @param provider Asset provider
         * @return Operation result
         */
        OperationResult ImportAsset(const std::filesystem::path &sourcePath,
                                    const std::filesystem::path &targetDirectory,
                                    std::shared_ptr<AssetProvider> provider);

    private:
        /**
         * @brief Show confirmation dialog for destructive operations
         * @param message Confirmation message
         * @return True if user confirmed
         */
        bool ShowConfirmationDialog(const std::string &message);

        /**
         * @brief Show input dialog for text input
         * @param title Dialog title
         * @param defaultValue Default input value
         * @param result Output string
         * @return True if user entered text
         */
        bool ShowInputDialog(const std::string &title,
                             const std::string &defaultValue,
                             std::string &result);
    };
}
