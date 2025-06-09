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
        static void Initialize(const std::string &executablePath); /**
                                                                    * @brief Get the full path to a resource file in workspace or global assets
                                                                    * @param relativePath Relative path to the resource e.g. "Models/cube.obj"
                                                                    * @param workspacePath Optional path to current workspace, if empty uses global assets
                                                                    * @return Full path to the resource, or empty string if not found
                                                                    */
        static std::string GetResourcePath(const std::string &relativePath, const std::string &workspacePath = "");

        /**
         * @brief Get the full path to a resource file in global assets only
         * @param relativePath Relative path to the resource
         * @return Full path to the resource, or empty string if not found
         */
        static std::string GetGlobalResourcePath(const std::string &relativePath);

        /**
         * @brief Get the full path to a resource file in workspace only
         * @param relativePath Relative path to the resource
         * @param workspacePath Path to the workspace
         * @return Full path to the resource, or empty string if not found
         */
        static std::string GetWorkspaceResourcePath(const std::string &relativePath, const std::string &workspacePath);

        /**
         * @brief Get the application directory (where the executable is located)
         * @return Path to the application directory
         */
        static std::string GetApplicationDirectory();

        /**
         * @brief Check if a resource file exists in workspace or global assets
         * @param relativePath Relative path to the resource
         * @param workspacePath Optional path to current workspace, if empty uses global assets
         * @return True if the resource exists, false otherwise
         */
        static bool ResourceExists(const std::string &relativePath, const std::string &workspacePath = "");

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
