#include "ResourceManager.h"
#include "UserDataManager.h"
#include <iostream>
#include <vector>

namespace Voltray::Utils
{

    // Static member initialization
    std::string ResourceManager::s_executablePath = "";
    std::string ResourceManager::s_applicationDirectory = "";
    std::string ResourceManager::s_projectRoot = "";

    void ResourceManager::Initialize(const std::string &executablePath)
    {
        s_executablePath = executablePath;

        // Get the directory containing the executable
        std::filesystem::path exePath(executablePath);
        s_applicationDirectory = exePath.parent_path().string();

        // Find the project root by looking for key files
        s_projectRoot = FindProjectRoot(s_applicationDirectory);
    }

    std::string ResourceManager::GetResourcePath(const std::string &relativePath, const std::string &workspacePath)
    {
        if (workspacePath.empty())
        {
            return GetGlobalResourcePath(relativePath);
        }
        else
        {
            // Try workspace first, then fallback to global
            std::string workspaceResource = GetWorkspaceResourcePath(relativePath, workspacePath);
            if (!workspaceResource.empty())
            {
                return workspaceResource;
            }
            return GetGlobalResourcePath(relativePath);
        }
    }

    std::string ResourceManager::GetGlobalResourcePath(const std::string &relativePath)
    {
        try
        {
            auto globalAssetsPath = UserDataManager::GetGlobalAssetsDirectory();
            std::filesystem::path fullPath = globalAssetsPath / relativePath;

            if (std::filesystem::exists(fullPath))
            {
                return fullPath.string();
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error accessing global resource: " << e.what() << std::endl;
        }

        return "";
    }

    std::string ResourceManager::GetWorkspaceResourcePath(const std::string &relativePath, const std::string &workspacePath)
    {
        try
        {
            std::filesystem::path fullPath = std::filesystem::path(workspacePath) / relativePath;

            if (std::filesystem::exists(fullPath))
            {
                return fullPath.string();
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error accessing workspace resource: " << e.what() << std::endl;
        }

        return "";
    }

    std::string ResourceManager::GetApplicationDirectory()
    {
        return s_applicationDirectory;
    }

    bool ResourceManager::ResourceExists(const std::string &relativePath, const std::string &workspacePath)
    {
        return !GetResourcePath(relativePath, workspacePath).empty();
    }

    std::string ResourceManager::FindProjectRoot(const std::string &startPath)
    {
        std::filesystem::path currentPath(startPath);

        // Look for key files that indicate the project root
        std::vector<std::string> keyFiles = {
            "CMakeLists.txt",
            "README.md",
            "Main.cpp",
            "LICENSE"};

        // Search up the directory tree
        for (int i = 0; i < 10; ++i) // Limit search depth to prevent infinite loops
        {
            bool foundAllKeys = true;
            for (const auto &keyFile : keyFiles)
            {
                if (!std::filesystem::exists(currentPath / keyFile))
                {
                    foundAllKeys = false;
                    break;
                }
            }

            if (foundAllKeys)
            {
                return currentPath.string();
            }

            // Go up one directory
            std::filesystem::path parent = currentPath.parent_path();
            if (parent == currentPath) // Reached root
            {
                break;
            }
            currentPath = parent;
        } // Fallback: return the start path
        return startPath;
    }

}
