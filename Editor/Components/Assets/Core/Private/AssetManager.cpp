#include "AssetManager.h"
#include "UserDataManager.h"
#include <algorithm>
#include <cctype>
#include <fstream>

namespace Voltray::Editor::Components::Assets
{
    void AssetManager::Initialize()
    {
        // Any initialization logic if needed
    }

    std::vector<AssetItem> AssetManager::GetDirectoryContents(
        const std::filesystem::path &directory,
        bool isGlobal,
        const AssetBrowserState &state)
    {
        std::vector<AssetItem> items;

        try
        { // Add parent directory entry if not at root
            auto globalRoot = Voltray::Utils::UserDataManager::GetGlobalAssetsDirectory();
            std::filesystem::path workspaceRoot;

            if (!isGlobal)
            {
                // Get workspace root - this will need to be passed in or retrieved from EditorApp
                // For now, we'll handle this in the calling code
            }

            bool isAtRoot = (isGlobal && directory == globalRoot) ||
                            (!isGlobal && workspaceRoot != std::filesystem::path{} && directory == workspaceRoot);

            if (!isAtRoot)
            {
                AssetItem parentItem;
                parentItem.name = "..";
                parentItem.path = directory.parent_path();
                parentItem.isDirectory = true;
                parentItem.isGlobal = isGlobal;
                parentItem.isParentDir = true;
                items.push_back(parentItem);
            }

            // Get directory contents
            for (const auto &entry : std::filesystem::directory_iterator(directory))
            {
                if (!ShouldShowFile(entry.path(), state))
                    continue;

                AssetItem item;
                item.name = entry.path().filename().string();
                item.path = entry.path();
                item.isDirectory = entry.is_directory();
                item.isGlobal = isGlobal;
                item.isParentDir = false;

                if (!item.isDirectory)
                {
                    try
                    {
                        item.fileSize = entry.file_size();
                        item.lastModified = entry.last_write_time();
                    }
                    catch (...)
                    {
                        item.fileSize = 0;
                    }

                    // Get file extension
                    item.extension = entry.path().extension().string();
                    if (!item.extension.empty() && item.extension[0] == '.')
                        item.extension = item.extension.substr(1);

                    item.mimeType = GetMimeType(item.extension);
                }

                // Check if file is hidden or read-only
                item.isHidden = (item.name[0] == '.');

                try
                {
                    auto permissions = std::filesystem::status(entry.path()).permissions();
                    item.isReadOnly = (permissions & std::filesystem::perms::owner_write) == std::filesystem::perms::none;
                }
                catch (...)
                {
                    item.isReadOnly = false;
                }

                items.push_back(item);
            }

            // Sort items
            SortAssets(items, state.sortBy, state.sortAscending);
        }
        catch (const std::exception &e)
        {
            // Handle directory access errors
        }

        return items;
    }

    bool AssetManager::ShouldShowFile(const std::filesystem::path &path, const AssetBrowserState &state)
    {
        std::string filename = path.filename().string();
        std::string extension = path.extension().string();

        // Hide system/workspace files
        if (filename == ".voltray" || filename == "workspace.json")
            return false;

        // Hide hidden files unless showing them
        if (!state.showHiddenFiles && filename[0] == '.')
            return false;

        // Hide backup files
        if (extension == ".bak" || extension == ".tmp" || filename.find("~") != std::string::npos)
            return false;

        // Apply search filter
        if (!state.searchFilter.empty() && !MatchesFilter(filename, state.searchFilter))
            return false;

        return true;
    }

    void AssetManager::SortAssets(std::vector<AssetItem> &items, const std::string &sortBy, bool ascending)
    {
        auto compareFn = [&sortBy, ascending](const AssetItem &a, const AssetItem &b) -> bool
        {
            // Parent directory always first
            if (a.isParentDir != b.isParentDir)
                return a.isParentDir > b.isParentDir;

            // Then directories before files
            if (a.isDirectory != b.isDirectory)
                return a.isDirectory > b.isDirectory;

            // Then sort by criteria
            bool result = false;
            if (sortBy == "name")
            {
                result = a.name < b.name;
            }
            else if (sortBy == "size")
            {
                result = a.fileSize < b.fileSize;
            }
            else if (sortBy == "date")
            {
                result = a.lastModified < b.lastModified;
            }
            else if (sortBy == "type")
            {
                result = a.extension < b.extension;
            }
            else
            {
                result = a.name < b.name; // Default to name
            }

            return ascending ? result : !result;
        };

        std::sort(items.begin(), items.end(), compareFn);
    }

    std::string AssetManager::GetMimeType(const std::string &extension)
    {
        // Convert to lowercase for comparison
        std::string ext = extension;
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

        // 3D Models
        if (ext == "obj" || ext == "fbx" || ext == "gltf" || ext == "glb" || ext == "3ds" || ext == "dae")
            return "model/3d";

        // Images
        if (ext == "png" || ext == "jpg" || ext == "jpeg" || ext == "bmp" || ext == "tga" || ext == "gif" || ext == "webp")
            return "image";

        // Audio
        if (ext == "wav" || ext == "mp3" || ext == "ogg" || ext == "flac" || ext == "aac")
            return "audio";

        // Video
        if (ext == "mp4" || ext == "avi" || ext == "mov" || ext == "wmv" || ext == "webm")
            return "video";

        // Text
        if (ext == "txt" || ext == "md" || ext == "rtf")
            return "text/plain";

        // Code
        if (ext == "cpp" || ext == "h" || ext == "cs" || ext == "js" || ext == "py" || ext == "java" || ext == "c")
            return "text/code";

        // Shaders
        if (ext == "glsl" || ext == "hlsl" || ext == "vert" || ext == "frag" || ext == "shader")
            return "text/shader";

        // Engine specific
        if (ext == "scene")
            return "application/voltray-scene";
        if (ext == "mat")
            return "application/voltray-material";
        if (ext == "prefab")
            return "application/voltray-prefab";

        return "application/octet-stream"; // Default
    }

    std::string AssetManager::GetFileCategory(const std::string &extension)
    {
        std::string mimeType = GetMimeType(extension);

        if (mimeType.substr(0, 6) == "model/")
            return "3D Model";
        else if (mimeType.substr(0, 6) == "image/")
            return "Image";
        else if (mimeType.substr(0, 6) == "audio/")
            return "Audio";
        else if (mimeType.substr(0, 6) == "video/")
            return "Video";
        else if (mimeType.substr(0, 10) == "text/code")
            return "Script";
        else if (mimeType.substr(0, 12) == "text/shader")
            return "Shader";
        else if (mimeType.substr(0, 5) == "text/")
            return "Text";
        else if (mimeType.substr(0, 20) == "application/voltray-")
            return "Engine Asset";
        else
            return "File";
    }

    bool AssetManager::CreateFolder(const std::filesystem::path &parentPath, const std::string &folderName)
    {
        try
        {
            auto newFolderPath = parentPath / folderName;
            return std::filesystem::create_directory(newFolderPath);
        }
        catch (...)
        {
            return false;
        }
    }

    bool AssetManager::CreateScript(const std::filesystem::path &parentPath, const std::string &scriptName, const std::string &scriptType)
    {
        try
        {
            std::string extension;
            std::string templateContent;

            if (scriptType == "C#")
            {
                extension = ".cs";
                templateContent = R"(using System;

public class )" + scriptName + R"(
{
    public void Start()
    {
        // Initialization code here
    }

    public void Update()
    {
        // Update code here
    }
}
)";
            }
            else if (scriptType == "JavaScript")
            {
                extension = ".js";
                templateContent = R"(// )" + scriptName + R"( Script

function start() {
    // Initialization code here
}

function update() {
    // Update code here
}
)";
            }
            else
            {
                return false; // Unsupported script type
            }

            auto scriptPath = parentPath / (scriptName + extension);
            std::ofstream file(scriptPath);
            if (file.is_open())
            {
                file << templateContent;
                file.close();
                return true;
            }
        }
        catch (...)
        {
            return false;
        }
        return false;
    }

    bool AssetManager::DeleteAsset(const std::filesystem::path &assetPath)
    {
        try
        {
            if (std::filesystem::is_directory(assetPath))
            {
                return std::filesystem::remove_all(assetPath) > 0;
            }
            else
            {
                return std::filesystem::remove(assetPath);
            }
        }
        catch (...)
        {
            return false;
        }
    }

    bool AssetManager::RenameAsset(const std::filesystem::path &oldPath, const std::string &newName)
    {
        try
        {
            auto newPath = oldPath.parent_path() / newName;
            std::filesystem::rename(oldPath, newPath);
            return true;
        }
        catch (...)
        {
            return false;
        }
    }

    bool AssetManager::CanDragAsset(const AssetItem &item)
    {
        // Don't allow dragging parent directory entries
        if (item.isParentDir)
            return false;

        // For now, allow dragging all files and folders except system files
        return !(item.name.size() > 0 && item.name[0] == '.');
    }

    std::string AssetManager::GetDragData(const AssetItem &item)
    {
        // Return the file path as drag data
        // This will be used by the drag-drop system
        return item.path.string();
    }

    bool AssetManager::MatchesFilter(const std::string &name, const std::string &filter)
    {
        // Simple case-insensitive substring search
        std::string lowerName = name;
        std::string lowerFilter = filter;

        std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
        std::transform(lowerFilter.begin(), lowerFilter.end(), lowerFilter.begin(), ::tolower);

        return lowerName.find(lowerFilter) != std::string::npos;
    }
}
