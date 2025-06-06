/**
 * @file LocalAssetProvider.h
 * @brief Provider for local scene assets
 * @author Voltray Engine Team
 * @date Created: 2025-06-06
 *
 * Handles full access to local scene assets that can be modified,
 * created, deleted, and organized by users.
 */

#pragma once

#include "AssetProvider.h"

namespace Voltray::Editor::Components::Assets
{
    /**
     * @class LocalAssetProvider
     * @brief Provides full access to local scene assets
     *
     * Local assets are scene-specific resources that users have full control over.
     * They can create, modify, delete, and organize these assets as needed.
     */
    class LocalAssetProvider : public AssetProvider
    {
    public:
        LocalAssetProvider();

        // AssetProvider interface
        void Initialize(const std::string &rootDirectory) override;

        std::vector<AssetItem> GetDirectoryContents(
            const std::filesystem::path &directory,
            const std::string &searchFilter = "") override;

        bool CanModifyAssets() const override { return true; }

        std::filesystem::path GetRootDirectory() const override { return m_RootDirectory; }

        std::string GetDisplayName() const override { return "Scene Assets"; }

        /**
         * @brief Set the current scene directory
         * @param sceneDirectory Path to scene assets directory
         */
        void SetSceneDirectory(const std::filesystem::path &sceneDirectory);

    private:
        std::filesystem::path m_RootDirectory;

        /**
         * @brief Check if a file should be displayed
         * @param path File path to check
         * @param searchFilter Search filter string
         * @return True if file should be shown
         */
        bool ShouldShowFile(const std::filesystem::path &path, const std::string &searchFilter) const;

        /**
         * @brief Create asset item from file path
         * @param path File path
         * @return Asset item with local flag set
         */
        AssetItem CreateAssetItem(const std::filesystem::path &path) const;
    };
}
