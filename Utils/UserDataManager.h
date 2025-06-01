#pragma once

#include <string>
#include <filesystem>

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
    static std::filesystem::path GetSettingsDirectory(); /**
                                                          * @brief Get the cache directory
                                                          * @return Path to the cache directory
                                                          */
    static std::filesystem::path GetCacheDirectory();    /**
                                                          * @brief Get the global assets directory (shared across all workspaces)
                                                          * @return Path to the global assets directory
                                                          */
    static std::filesystem::path GetGlobalAssetsDirectory();

    /**
     * @brief Initialize default global assets (primitives like cube, sphere, etc.)
     * @return true if successful
     */
    static bool InitializeDefaultGlobalAssets();

    /**
     * @brief Check if the user data system is initialized
     * @return true if initialized, false otherwise
     */
    static bool IsInitialized();

private:
    static std::filesystem::path GetPlatformAppDataPath();
    static bool CreateDirectoryStructure();

    static std::filesystem::path s_AppDataPath;
    static bool s_Initialized;
};