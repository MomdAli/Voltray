#pragma once

#include <string>
#include <filesystem>

namespace Voltray::Utils
{

    /**
     * @brief Manages user data directories across different operating systems
     */
    class UserDataManager
    {
    public:
        /**
         * @brief Initialize the user data directory system
         * @return true if successful, false otherwise
         */
        static bool Initialize();

        /**
         * @brief Get the main application data directory
         * @return Path to the application data directory
         */
        static std::filesystem::path GetAppDataDirectory();

        /**
         * @brief Get the workspaces directory
         * @return Path to the workspaces directory
         */
        static std::filesystem::path GetWorkspacesDirectory();

        /**
         * @brief Get the settings directory
         * @return Path to the settings directory
         */
        static std::filesystem::path GetSettingsDirectory();

        /**
         * @brief Get the cache directory
         * @return Path to the cache directory
         */
        static std::filesystem::path GetCacheDirectory();

        /**
         * @brief Get the global assets directory (shared across all workspaces)
         * @return Path to the global assets directory
         */
        static std::filesystem::path GetGlobalAssetsDirectory();

        /**
         * @brief Initialize default global assets (primitives like cube, sphere, etc.)
         * @param forceUpdate Force update even if assets are already initialized
         * @return true if successful
         */
        static bool InitializeDefaultGlobalAssets(bool forceUpdate = false);

        /**
         * @brief Copy Resources from build directory to user global assets
         * @param resourcesPath Path to the Resources directory in build output
         * @return true if successful
         */
        static bool CopyResourcesFromBuild(const std::filesystem::path &resourcesPath); /**
                                                                                         * @brief Check if we're running in development environment (Resources folder exists)
                                                                                         * @return true if development environment detected
                                                                                         */
        static bool IsDevEnvironment();

        /**
         * @brief Check if the user data system is initialized
         * @return true if initialized, false otherwise
         */
        static bool IsInitialized();

    private:
        static std::filesystem::path GetPlatformAppDataPath();
        static bool CreateDirectoryStructure();
        static bool CreateDirectoryStructureFromResources(const std::filesystem::path &globalAssetsPath);

        static std::filesystem::path s_AppDataPath;
        static bool s_Initialized;
    };

}