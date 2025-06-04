#include "Workspace.h"
#include "UserDataManager.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>

using json = nlohmann::json;

namespace Voltray::Utils
{

    // Static member definitions
    std::vector<Workspace> WorkspaceManager::s_Workspaces;
    Workspace *WorkspaceManager::s_CurrentWorkspace = nullptr;

    // ===== Workspace Implementation =====

    std::string Workspace::ToJson() const
    {
        json j;
        j["name"] = name;
        j["description"] = description;
        j["path"] = path.string();

        // Convert time points to Unix timestamps (seconds since epoch)
        auto lastOpenedTime = std::chrono::duration_cast<std::chrono::seconds>(lastOpened.time_since_epoch()).count();
        auto createdTime = std::chrono::duration_cast<std::chrono::seconds>(created.time_since_epoch()).count();

        j["lastOpened"] = lastOpenedTime;
        j["created"] = createdTime;
        j["isValid"] = isValid;

        return j.dump();
    }

    Workspace Workspace::FromJson(const std::string &jsonStr)
    {
        Workspace workspace;
        try
        {
            json j = json::parse(jsonStr);

            workspace.name = j.value("name", "");
            workspace.description = j.value("description", "");
            workspace.path = j.value("path", "");
            workspace.isValid = j.value("isValid", true);

            // Convert Unix timestamps back to time points
            auto lastOpenedTime = j.value("lastOpened", 0);
            auto createdTime = j.value("created", 0);

            workspace.lastOpened = std::chrono::system_clock::from_time_t(lastOpenedTime);
            workspace.created = std::chrono::system_clock::from_time_t(createdTime);
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error parsing workspace JSON: " << e.what() << std::endl;
            workspace.isValid = false;
        }

        return workspace;
    }

    bool Workspace::IsPathValid() const
    {
        return std::filesystem::exists(path) && std::filesystem::is_directory(path);
    }

    // ===== WorkspaceManager Implementation =====

    bool WorkspaceManager::Initialize()
    {
        // Ensure UserDataManager is initialized first
        if (!UserDataManager::IsInitialized())
        {
            std::cerr << "UserDataManager must be initialized before WorkspaceManager" << std::endl;
            return false;
        }

        // Load existing workspaces from persistent storage
        LoadWorkspaces();

        // Clean up any workspaces whose directories no longer exist
        int removedCount = CleanupInvalidWorkspaces();
        if (removedCount > 0)
        {
            std::cout << "Cleaned up " << removedCount << " invalid workspaces" << std::endl;
        }

        std::cout << "WorkspaceManager initialized with " << s_Workspaces.size() << " workspaces" << std::endl;
        return true;
    }

    std::vector<Workspace> WorkspaceManager::GetAllWorkspaces()
    {
        return s_Workspaces;
    }

    bool WorkspaceManager::CreateWorkspace(const std::string &name, const std::filesystem::path &path, const std::string &description)
    {
        try
        {
            // Check if workspace already exists at this path
            for (const auto &workspace : s_Workspaces)
            {
                if (workspace.path == path)
                {
                    std::cerr << "Workspace already exists at path: " << path << std::endl;
                    return false;
                }
            }

            // Create the directory if it doesn't exist
            std::filesystem::create_directories(path);

            // Create standard workspace directory structure
            if (!CreateWorkspaceStructure(path))
            {
                std::cerr << "Failed to create workspace structure" << std::endl;
                return false;
            }

            // Create workspace metadata object
            Workspace workspace;
            workspace.name = name;
            workspace.description = description;
            workspace.path = path;
            workspace.created = std::chrono::system_clock::now();
            workspace.lastOpened = workspace.created;
            workspace.isValid = true;

            // Add to workspace list
            s_Workspaces.push_back(workspace);

            // Persist to disk
            SaveWorkspaces();

            std::cout << "Created workspace: " << name << " at " << path << std::endl;
            return true;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error creating workspace: " << e.what() << std::endl;
            return false;
        }
    }

    bool WorkspaceManager::CreateWorkspaceStructure(const std::filesystem::path &path)
    {
        try
        {
            // Create workspace-specific directories (Scene Panel content)
            std::filesystem::create_directories(path / "Scenes");    // Scene files (.scene)
            std::filesystem::create_directories(path / "Scripts");   // User scripts
            std::filesystem::create_directories(path / "Materials"); // Custom materials
            std::filesystem::create_directories(path / "Textures");  // Custom textures
            std::filesystem::create_directories(path / "Models");    // Custom 3D models
            std::filesystem::create_directories(path / "Audio");     // Custom audio files
            std::filesystem::create_directories(path / "Prefabs");   // Reusable objects
            std::filesystem::create_directories(path / ".voltray");  // Internal engine data

            // NOTE: No "Assets" folder - that's global and shared across all workspaces

            // Create workspace marker file with metadata
            std::ofstream markerFile(path / WORKSPACE_FILE_NAME);
            if (markerFile.is_open())
            {
                json markerJson;
                markerJson["workspace_version"] = "1.0";
                markerJson["engine_version"] = "Voltray 1.0";
                markerJson["created"] = std::chrono::duration_cast<std::chrono::seconds>(
                                            std::chrono::system_clock::now().time_since_epoch())
                                            .count();

                markerFile << markerJson.dump(4);
                markerFile.close();
            }

            return true;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error creating workspace structure: " << e.what() << std::endl;
            return false;
        }
    }

    bool WorkspaceManager::IsValidWorkspaceDirectory(const std::filesystem::path &path)
    {
        // A valid workspace must have the marker file and be a directory
        return std::filesystem::exists(path / WORKSPACE_FILE_NAME) &&
               std::filesystem::is_directory(path);
    }

    std::filesystem::path WorkspaceManager::GetWorkspacesConfigFile()
    {
        return UserDataManager::GetSettingsDirectory() / "workspaces.json";
    }

    bool WorkspaceManager::SaveWorkspaces()
    {
        try
        {
            json j;
            j["version"] = "1.0";
            j["workspaces"] = json::array();

            // Serialize each workspace to JSON
            for (const auto &workspace : s_Workspaces)
            {
                json workspaceJson = json::parse(workspace.ToJson());
                j["workspaces"].push_back(workspaceJson);
            }

            // Write to file
            std::ofstream file(GetWorkspacesConfigFile());
            if (file.is_open())
            {
                file << j.dump(4); // Pretty print with 4-space indentation
                file.close();
                return true;
            }
            else
            {
                std::cerr << "Failed to open workspaces config file for writing" << std::endl;
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error saving workspaces: " << e.what() << std::endl;
        }

        return false;
    }

    bool WorkspaceManager::LoadWorkspaces()
    {
        try
        {
            auto configFile = GetWorkspacesConfigFile();
            if (!std::filesystem::exists(configFile))
            {
                std::cout << "No workspace config file found, starting with empty workspace list" << std::endl;
                return true; // This is not an error for first run
            }

            std::ifstream file(configFile);
            if (!file.is_open())
            {
                std::cerr << "Failed to open workspace config file for reading" << std::endl;
                return false;
            }

            json j;
            file >> j;
            file.close();

            s_Workspaces.clear();

            // Parse workspaces array
            if (j.contains("workspaces") && j["workspaces"].is_array())
            {
                for (const auto &workspaceJson : j["workspaces"])
                {
                    Workspace workspace = Workspace::FromJson(workspaceJson.dump());
                    if (workspace.isValid)
                    {
                        s_Workspaces.push_back(workspace);
                    }
                }
            }

            std::cout << "Loaded " << s_Workspaces.size() << " workspaces from config" << std::endl;
            return true;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error loading workspaces: " << e.what() << std::endl;
            return false;
        }
    }

    int WorkspaceManager::CleanupInvalidWorkspaces()
    {
        int removedCount = 0;
        auto it = s_Workspaces.begin();

        while (it != s_Workspaces.end())
        {
            // Check if path exists and is still a valid workspace
            if (!it->IsPathValid() || !IsValidWorkspaceDirectory(it->path))
            {
                std::cout << "Removing invalid workspace: " << it->name << " (" << it->path << ")" << std::endl;

                // If this was the current workspace, clear it
                if (s_CurrentWorkspace == &(*it))
                {
                    s_CurrentWorkspace = nullptr;
                }

                it = s_Workspaces.erase(it);
                removedCount++;
            }
            else
            {
                ++it;
            }
        }

        // Save changes if any workspaces were removed
        if (removedCount > 0)
        {
            SaveWorkspaces();
        }

        return removedCount;
    }

    const Workspace *WorkspaceManager::GetCurrentWorkspace()
    {
        return s_CurrentWorkspace;
    }

    bool WorkspaceManager::SetCurrentWorkspace(const std::filesystem::path &workspacePath)
    {
        // Find workspace by path
        for (auto &workspace : s_Workspaces)
        {
            if (workspace.path == workspacePath)
            {
                s_CurrentWorkspace = &workspace;
                UpdateLastOpened(workspacePath);
                return true;
            }
        }

        s_CurrentWorkspace = nullptr;
        return false;
    }

    bool WorkspaceManager::UpdateLastOpened(const std::filesystem::path &workspacePath)
    {
        for (auto &workspace : s_Workspaces)
        {
            if (workspace.path == workspacePath)
            {
                workspace.lastOpened = std::chrono::system_clock::now();
                SaveWorkspaces();
                return true;
            }
        }
        return false;
    }

    bool WorkspaceManager::RemoveWorkspace(const std::filesystem::path &workspacePath)
    {
        auto it = std::find_if(s_Workspaces.begin(), s_Workspaces.end(),
                               [&workspacePath](const Workspace &w)
                               { return w.path == workspacePath; });

        if (it != s_Workspaces.end())
        {
            if (s_CurrentWorkspace == &(*it))
            {
                s_CurrentWorkspace = nullptr;
            }

            s_Workspaces.erase(it);
            SaveWorkspaces();
            return true;
        }

        return false;
    }

}
