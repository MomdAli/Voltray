#pragma once

#include <string>
#include <filesystem>
#include <vector>
#include <chrono>

/**
 * @brief Represents a workspace with its metadata and settings
 *
 * A workspace is a project directory that contains user-specific content like scenes,
 * custom materials, scripts, and models. Global assets (primitives like cube, sphere)
 * are stored separately and shared across all workspaces.
 *
 * Workspace structure:
 * - Scenes/     - Scene files (.scene)
 * - Scripts/    - User scripts
 * - Materials/  - Custom materials
 * - Textures/   - Custom textures
 * - Models/     - Custom 3D models
 * - Audio/      - Custom audio files
 * - Prefabs/    - Reusable objects
 * - .voltray/   - Internal engine data
 */
struct Workspace
{
    std::string name;                                 // Display name of the workspace
    std::string description;                          // Optional description
    std::filesystem::path path;                       // Full path to workspace directory
    std::chrono::system_clock::time_point lastOpened; // Last access time
    std::chrono::system_clock::time_point created;    // Creation time
    bool isValid = true;                              // Whether the workspace path still exists

    /**
     * @brief Serialize workspace to JSON string
     * @return JSON representation of the workspace
     */
    std::string ToJson() const;

    /**
     * @brief Deserialize workspace from JSON string
     * @param json JSON string to parse
     * @return Workspace object
     */
    static Workspace FromJson(const std::string &json);

    /**
     * @brief Check if the workspace path is valid and exists
     * @return true if path exists and is a directory
     */
    bool IsPathValid() const;
};

/**
 * @brief Manages all workspace operations including creation, loading, and persistence
 *
 * This class handles:
 * - Creating new workspaces with proper directory structure (NO global assets)
 * - Loading and saving workspace lists to/from JSON config files
 * - Managing the currently active workspace
 * - Cleaning up invalid workspaces (deleted directories)
 *
 * Note: Global assets (cube, sphere, etc.) are managed by UserDataManager,
 * not individual workspaces. Each workspace contains only user-specific content.
 */
class WorkspaceManager
{
public:
    /**
     * @brief Initialize the workspace manager and load existing workspaces
     * @return true if successful
     */
    static bool Initialize();

    /**
     * @brief Get all available workspaces
     * @return Vector of all workspaces (including invalid ones)
     */
    static std::vector<Workspace> GetAllWorkspaces();

    /**
     * @brief Create a new workspace with standard directory structure
     * @param name Display name for the workspace
     * @param path Directory path where workspace will be created
     * @param description Optional description
     * @return true if created successfully
     */
    static bool CreateWorkspace(const std::string &name, const std::filesystem::path &path, const std::string &description = "");

    /**
     * @brief Remove a workspace from the managed list (doesn't delete files)
     * @param workspacePath Path to the workspace to remove
     * @return true if removed successfully
     */
    static bool RemoveWorkspace(const std::filesystem::path &workspacePath);

    /**
     * @brief Update last opened time for a workspace
     * @param workspacePath Path to the workspace
     * @return true if updated successfully
     */
    static bool UpdateLastOpened(const std::filesystem::path &workspacePath);

    /**
     * @brief Get the currently active workspace
     * @return Pointer to current workspace, nullptr if none active
     */
    static const Workspace *GetCurrentWorkspace();

    /**
     * @brief Set the current active workspace
     * @param workspacePath Path to the workspace to set as current
     * @return true if set successfully
     */
    static bool SetCurrentWorkspace(const std::filesystem::path &workspacePath);

    /**
     * @brief Save workspace list to persistent storage
     * @return true if saved successfully
     */
    static bool SaveWorkspaces();

    /**
     * @brief Load workspace list from persistent storage
     * @return true if loaded successfully
     */
    static bool LoadWorkspaces();

    /**
     * @brief Check if a directory is a valid Voltray workspace
     * @param path Directory path to check
     * @return true if contains workspace marker file
     */
    static bool IsValidWorkspaceDirectory(const std::filesystem::path &path);

    /**
     * @brief Remove invalid workspaces from the list (paths that no longer exist)
     * @return Number of workspaces removed
     */
    static int CleanupInvalidWorkspaces();

private:
    static std::vector<Workspace> s_Workspaces; // All known workspaces
    static Workspace *s_CurrentWorkspace;       // Currently active workspace

    /**
     * @brief Get path to the workspaces configuration file
     * @return Path to workspaces.json in settings directory
     */
    static std::filesystem::path GetWorkspacesConfigFile();

    /**
     * @brief Create standard workspace directory structure
     * @param path Workspace root directory
     * @return true if structure created successfully
     */
    static bool CreateWorkspaceStructure(const std::filesystem::path &path);

    // Marker file that identifies a directory as a Voltray workspace
    static constexpr const char *WORKSPACE_FILE_NAME = ".voltray_workspace";
};
