#pragma once

#include <string>
#include <filesystem>

namespace Voltray::Utils
{

    /**
     * @class ResourceManager
     * @brief Handles resource path resolution and asset management
     *
     * This class provides utilities to find resources relative to the executable
     * or in standard system locations, ensuring resources can be found regardless
     * of where the executable is run from.
     */
    class ResourceManager
    {
    public:
        /**
         * @brief Initialize the resource manager with the executable path
         * @param executablePath Path to the current executable
         */
        static void Initialize(const std::string &executablePath);

        /**
         * @brief Get the full path to a resource file
         * @param relativePath Relative path from the project root (e.g., "Editor/Resources/Fonts/Sora.ttf")
         * @return Full path to the resource, or empty string if not found
         */
        static std::string GetResourcePath(const std::string &relativePath);

        /**
         * @brief Get the application directory (where the executable is located)
         * @return Path to the application directory
         */
        static std::string GetApplicationDirectory();

        /**
         * @brief Check if a resource file exists
         * @param relativePath Relative path to the resource
         * @return True if the resource exists, false otherwise
         */
        static bool ResourceExists(const std::string &relativePath);

    private:
        static std::string s_executablePath;
        static std::string s_applicationDirectory;
        static std::string s_projectRoot;

        /**
         * @brief Find the project root directory by looking for marker files
         * @param startPath Starting path to search from
         * @return Path to project root, or empty if not found
         */
        static std::string FindProjectRoot(const std::string &startPath);
    };

}
