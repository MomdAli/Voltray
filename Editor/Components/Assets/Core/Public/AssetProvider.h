/**
 * @file AssetProvider.h
 * @brief Base interface for asset providers
 * @author Voltray Engine Team
 * @date Created: 2025-06-06
 *
 * Defines the interface for providing assets from different sources
 * (global engine assets vs local scene assets).
 */

#pragma once

#include "AssetItem.h"
#include "AssetFilter.h"
#include <vector>
#include <filesystem>
#include <string>

namespace Voltray::Editor::Components::Assets
{
    /**
     * @class AssetProvider
     * @brief Abstract base class for asset providers
     *
     * Provides a unified interface for accessing assets from different sources.
     * Concrete implementations handle global (read-only) and local (modifiable) assets.
     */
    class AssetProvider
    {
    public:
        virtual ~AssetProvider() = default;

        /**
         * @brief Initialize the provider with a root directory
         * @param rootDirectory Root directory path
         */
        virtual void Initialize(const std::string &rootDirectory) = 0;

        /**
         * @brief Get directory contents as asset items
         * @param directory Directory to scan
         * @param searchFilter Optional search filter
         * @return Vector of asset items
         */
        virtual std::vector<AssetItem> GetDirectoryContents(
            const std::filesystem::path &directory,
            const std::string &searchFilter = "") = 0;

        /**
         * @brief Check if this provider allows modifications
         * @return True if assets can be modified, false for read-only
         */
        virtual bool CanModifyAssets() const = 0;

        /**
         * @brief Get the root directory for this provider
         * @return Root directory path
         */
        virtual std::filesystem::path GetRootDirectory() const = 0; /**
                                                                     * @brief Get display name for this provider
                                                                     * @return Display name (e.g., "Global Assets", "Scene Assets")
                                                                     */
        virtual std::string GetDisplayName() const = 0;

        /**
         * @brief Get the asset filter for this provider
         * @return Reference to the asset filter
         */
        virtual AssetFilter &GetAssetFilter() = 0;

        /**
         * @brief Get the asset filter for this provider (const version)
         * @return Const reference to the asset filter
         */
        virtual const AssetFilter &GetAssetFilter() const = 0;
    };
}
