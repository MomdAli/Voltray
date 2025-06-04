#include "ResourceManager.h"
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

    std::string ResourceManager::GetResourcePath(const std::string &relativePath)
    {
        if (s_projectRoot.empty())
        {
            return "";
        }

        // Try project root first
        std::filesystem::path fullPath = std::filesystem::path(s_projectRoot) / relativePath;
        if (std::filesystem::exists(fullPath))
        {
            return fullPath.string();
        }

        // Try relative to executable directory
        fullPath = std::filesystem::path(s_applicationDirectory) / relativePath;
        if (std::filesystem::exists(fullPath))
        {
            return fullPath.string();
        } // Try going up one level from executable (for build subdirectories)
        fullPath = std::filesystem::path(s_applicationDirectory).parent_path() / relativePath;
        if (std::filesystem::exists(fullPath))
        {
            return fullPath.string();
        }

        // Additional check: try two levels up from executable
        fullPath = std::filesystem::path(s_applicationDirectory).parent_path().parent_path() / relativePath;
        if (std::filesystem::exists(fullPath))
        {
            return fullPath.string();
        }

        // Resource not found
        return ""; // Resource not found
    }

    std::string ResourceManager::GetApplicationDirectory()
    {
        return s_applicationDirectory;
    }

    bool ResourceManager::ResourceExists(const std::string &relativePath)
    {
        return !GetResourcePath(relativePath).empty();
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
