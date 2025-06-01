#include "UserDataManager.h"
#include <iostream>
#include <fstream>
#include <vector>

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

bool UserDataManager::InitializeDefaultGlobalAssets()
{
    try
    {
        auto globalAssetsPath = GetGlobalAssetsDirectory();

        // Create asset category directories
        std::filesystem::create_directories(globalAssetsPath / "Primitives");
        std::filesystem::create_directories(globalAssetsPath / "Materials");
        std::filesystem::create_directories(globalAssetsPath / "Textures");
        std::filesystem::create_directories(globalAssetsPath / "Scripts");

        // Create a marker file to indicate assets have been initialized
        auto markerFile = globalAssetsPath / ".assets_initialized";
        if (!std::filesystem::exists(markerFile))
        {
            std::ofstream marker(markerFile);
            if (marker.is_open())
            {
                marker << "Global assets initialized\n";
                marker.close();
            }

            // TODO: Here you can copy/generate default primitive models, textures, etc.
            // For now, we'll just create placeholder files to show the structure

            // Create placeholder primitive files
            auto primitivesPath = globalAssetsPath / "Primitives";
            std::vector<std::string> primitives = {"cube.obj", "sphere.obj", "plane.obj", "cylinder.obj", "cone.obj"};

            for (const auto &primitive : primitives)
            {
                auto filePath = primitivesPath / primitive;
                if (!std::filesystem::exists(filePath))
                {
                    std::ofstream file(filePath);
                    if (file.is_open())
                    {
                        file << "# " << primitive << " - Placeholder\n";
                        file << "# This would contain the actual mesh data\n";
                        file.close();
                    }
                }
            }

            std::cout << "Initialized default global assets" << std::endl;
        }

        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error initializing global assets: " << e.what() << std::endl;
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