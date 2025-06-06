#include "UserDataManager.h"
#include "ResourceManager.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

#ifdef _WIN32
#include <Windows.h>
#include <shlobj.h>
#elif defined(__APPLE__)
#include <pwd.h>
#include <unistd.h>
#else
#include <pwd.h>
#include <unistd.h>
#endif

namespace Voltray::Utils
{

    std::filesystem::path UserDataManager::s_AppDataPath;
    bool UserDataManager::s_Initialized = false;

    bool UserDataManager::Initialize()
    {
        try
        {
            s_AppDataPath = GetPlatformAppDataPath();

            if (s_AppDataPath.empty())
            {
                std::cerr << "Failed to determine platform app data path" << std::endl;
                return false;
            }

            // Create the directory structure
            if (!CreateDirectoryStructure())
            {
                std::cerr << "Failed to create directory structure" << std::endl;
                return false;
            }

            s_Initialized = true;
            std::cout << "UserDataManager initialized successfully at: " << s_AppDataPath << std::endl;
            return true;
        }
        catch (const std::exception &e)
        {
            std::cerr << "UserDataManager initialization failed: " << e.what() << std::endl;
            return false;
        }
    }

    std::filesystem::path UserDataManager::GetAppDataDirectory()
    {
        return s_AppDataPath;
    }

    std::filesystem::path UserDataManager::GetWorkspacesDirectory()
    {
        return s_AppDataPath / "Workspaces";
    }

    std::filesystem::path UserDataManager::GetSettingsDirectory()
    {
        return s_AppDataPath / "Settings";
    }

    std::filesystem::path UserDataManager::GetCacheDirectory()
    {
        return s_AppDataPath / "Cache";
    }

    std::filesystem::path UserDataManager::GetGlobalAssetsDirectory()
    {
        return s_AppDataPath / "GlobalAssets";
    }
    bool UserDataManager::InitializeDefaultGlobalAssets(bool forceUpdate)
    {
        try
        {
            auto globalAssetsPath = GetGlobalAssetsDirectory();

            // Create a marker file to indicate assets have been initialized
            auto markerFile = globalAssetsPath / ".assets_initialized";

            // In development environment, always update resources if they exist locally
            bool isDev = IsDevEnvironment();
            bool markerExists = std::filesystem::exists(markerFile);
            bool shouldUpdate = forceUpdate || !markerExists || isDev;
            if (shouldUpdate)
            {
                // Try to find Resources in application directory first
                auto resourcesPath = std::filesystem::path(ResourceManager::GetApplicationDirectory()) / "Resources";

                if (std::filesystem::exists(resourcesPath))
                {
                    std::cout << (isDev ? "Development environment detected - updating resources from build" : "Initializing global assets from build resources") << std::endl;

                    if (CopyResourcesFromBuild(resourcesPath))
                    {
                        std::ofstream marker(markerFile);
                        if (marker.is_open())
                        {
                            marker << "Global assets initialized from build resources\n";
                            marker << "Environment: " << (isDev ? "Development" : "Release") << "\n";
                            marker << "Last updated: " << std::chrono::system_clock::now().time_since_epoch().count() << "\n";
                            marker.close();
                        }
                        std::cout << "Successfully updated global assets from build resources" << std::endl;
                        return true;
                    }
                }

                // Fallback: create directory structure based on Resources folder if no local Resources found
                if (!isDev && !std::filesystem::exists(markerFile))
                {
                    CreateDirectoryStructureFromResources(globalAssetsPath);

                    std::ofstream marker(markerFile);
                    if (marker.is_open())
                    {
                        marker << "Global assets initialized with dynamic structure\n";
                        marker << "Environment: Release\n";
                        marker.close();
                    }
                    std::cout << "Initialized global assets with dynamic directory structure" << std::endl;
                }
            }
            else
            {
                std::cout << "Global assets already initialized, skipping update" << std::endl;
            }

            return true;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error initializing global assets: " << e.what() << std::endl;
            return false;
        }
    }

    bool UserDataManager::CopyResourcesFromBuild(const std::filesystem::path &resourcesPath)
    {
        try
        {
            auto globalAssetsPath = GetGlobalAssetsDirectory();

            if (!std::filesystem::exists(resourcesPath))
            {
                std::cerr << "Resources path does not exist: " << resourcesPath << std::endl;
                return false;
            } // Copy everything from Resources to GlobalAssets
            for (const auto &entry : std::filesystem::recursive_directory_iterator(resourcesPath))
            {
                if (entry.is_regular_file())
                {
                    auto relativePath = std::filesystem::relative(entry.path(), resourcesPath);
                    auto destPath = globalAssetsPath / relativePath; // Create destination directory if it doesn't exist
                    std::filesystem::create_directories(destPath.parent_path());

                    // Copy file
                    std::filesystem::copy_file(entry.path(), destPath,
                                               std::filesystem::copy_options::overwrite_existing);
                }
            }

            std::cout << "Successfully copied resources from: " << resourcesPath << std::endl;
            return true;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error copying resources: " << e.what() << std::endl;
            return false;
        }
    }
    bool UserDataManager::IsInitialized()
    {
        return s_Initialized;
    }

    std::filesystem::path UserDataManager::GetPlatformAppDataPath()
    {
        std::filesystem::path appDataPath;

#ifdef _WIN32
        // Windows: Use %APPDATA%
        wchar_t *path = nullptr;
        if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &path)))
        {
            appDataPath = std::filesystem::path(path) / "Voltray";
            CoTaskMemFree(path);
        }
#elif defined(__APPLE__)
        // macOS: Use ~/Library/Application Support
        struct passwd *pw = getpwuid(getuid());
        if (pw && pw->pw_dir)
        {
            appDataPath = std::filesystem::path(pw->pw_dir) / "Library" / "Application Support" / "Voltray";
        }
#else
        // Linux: Use ~/.local/share
        const char *xdgDataHome = std::getenv("XDG_DATA_HOME");
        if (xdgDataHome)
        {
            appDataPath = std::filesystem::path(xdgDataHome) / "Voltray";
        }
        else
        {
            struct passwd *pw = getpwuid(getuid());
            if (pw && pw->pw_dir)
            {
                appDataPath = std::filesystem::path(pw->pw_dir) / ".local" / "share" / "Voltray";
            }
        }
#endif

        return appDataPath;
    }

    bool UserDataManager::CreateDirectoryStructure()
    {
        try
        {
            // Create main app data directory
            std::filesystem::create_directories(s_AppDataPath); // Create subdirectories
            std::filesystem::create_directories(GetWorkspacesDirectory());
            std::filesystem::create_directories(GetSettingsDirectory());
            std::filesystem::create_directories(GetCacheDirectory());
            std::filesystem::create_directories(GetGlobalAssetsDirectory());

            // Initialize default global assets on first run
            InitializeDefaultGlobalAssets();

            return true;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Failed to create directory structure: " << e.what() << std::endl;
            return false;
        }
    }

    bool UserDataManager::CreateDirectoryStructureFromResources(const std::filesystem::path &globalAssetsPath)
    {
        try
        {
            // Try to get the Resources path relative to the application
            auto resourcesPath = std::filesystem::path(ResourceManager::GetApplicationDirectory()) / "Resources";

            if (std::filesystem::exists(resourcesPath))
            {
                // If Resources directory exists, create directories based on its structure
                for (const auto &entry : std::filesystem::directory_iterator(resourcesPath))
                {
                    if (entry.is_directory())
                    {
                        auto folderName = entry.path().filename().string();
                        std::filesystem::create_directories(globalAssetsPath / folderName);
                        std::cout << "Created global assets folder: " << folderName << std::endl;
                    }
                }
                return true;
            }
            else
            {
                // Fallback: create hardcoded common asset directories
                std::vector<std::string> defaultDirectories = {"Models", "Fonts", "Shaders", "Textures", "Materials", "Scripts"};

                for (const auto &dir : defaultDirectories)
                {
                    std::filesystem::create_directories(globalAssetsPath / dir);
                    std::cout << "Created default global assets folder: " << dir << std::endl;
                }
                return true;
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error creating directory structure from resources: " << e.what() << std::endl;

            // Final fallback: create basic structure
            try
            {
                std::filesystem::create_directories(globalAssetsPath / "Models");
                std::filesystem::create_directories(globalAssetsPath / "Fonts");
                std::cout << "Created basic fallback structure" << std::endl;
                return true;
            }
            catch (...)
            {
                return false;
            }
        }
    }
    bool UserDataManager::IsDevEnvironment()
    {
        try
        {
            // Check if Resources folder exists in the application directory
            auto resourcesPath = std::filesystem::path(ResourceManager::GetApplicationDirectory()) / "Resources";
            bool hasResources = std::filesystem::exists(resourcesPath);

            // Check if we're in a typical development build structure
            auto appDir = std::filesystem::path(ResourceManager::GetApplicationDirectory());
            bool inBuildDir = appDir.filename() == "Debug" || appDir.filename() == "Release" ||
                              appDir.parent_path().filename() == "build";

            // Development environment if we have local Resources OR we're in a build directory
            return hasResources || inBuildDir;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error detecting environment: " << e.what() << std::endl;
            return false; // Default to release behavior on error
        }
    }

}