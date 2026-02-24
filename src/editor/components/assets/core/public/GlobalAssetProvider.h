/**
 * @file GlobalAssetProvider.h
 * @brief Provider for global engine assets
 * @author Voltray Engine Team
 * @date Created: 2025-06-06
 *
 * Handles read-only access to global engine assets that are shared
 * across all projects and cannot be modified by users.
 */

#pragma once

#include "AssetProvider.h"

namespace Voltray::Editor::Components::Assets
{
    /**
     * @class GlobalAssetProvider
     * @brief Provides read-only access to global engine assets
     *
     * Global assets are shared engine resources that users can view and use
     * but cannot modify, delete, or add to. These include built-in materials,
     * meshes, textures, and other engine-provided content.
     */
    class GlobalAssetProvider : public AssetProvider
    {
    public:
        GlobalAssetProvider();

        // AssetProvider interface
        void Initialize(const std::string &rootDirectory) override;

        std::vector<AssetItem> GetDirectoryContents(
            const std::filesystem::path &directory,
            const std::string &searchFilter = "") override;

        bool CanModifyAssets() const override { return false; }

        std::filesystem::path GetRootDirectory() const override { return m_RootDirectory; }

        std::string GetDisplayName() const override { return "Global Assets"; }

        AssetFilter &GetAssetFilter() override { return m_AssetFilter; }
        const AssetFilter &GetAssetFilter() const override { return m_AssetFilter; }

    private:
        std::filesystem::path m_RootDirectory;
        AssetFilter m_AssetFilter;

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
         * @return Asset item with global flag set
         */
        AssetItem CreateAssetItem(const std::filesystem::path &path) const;
    };
}
