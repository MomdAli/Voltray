#include "LocalAssetProvider.h"
#include "UserDataManager.h"
#include <algorithm>
#include <cctype>

using Voltray::Utils::UserDataManager;

namespace Voltray::Editor::Components::Assets
{
    LocalAssetProvider::LocalAssetProvider()
    {
        // Default initialization - directory will be set via Initialize()
    }

    void LocalAssetProvider::Initialize(const std::string &rootDirectory)
    {
        m_RootDirectory = std::filesystem::path(rootDirectory);

        // Create directory if it doesn't exist
        if (!std::filesystem::exists(m_RootDirectory))
        {
            std::filesystem::create_directories(m_RootDirectory);
        }
    }

    std::vector<AssetItem> LocalAssetProvider::GetDirectoryContents(
        const std::filesystem::path &directory,
        const std::string &searchFilter)
    {
        std::vector<AssetItem> items;

        try
        {
            // Add parent directory navigation (except for root)
            if (directory != m_RootDirectory && directory.has_parent_path())
            {
                AssetItem parentItem;
                parentItem.name = "..";
                parentItem.path = directory.parent_path();
                parentItem.isDirectory = true;
                parentItem.isParentDir = true;
                parentItem.isGlobal = false;
                parentItem.fileSize = 0;
                items.push_back(parentItem);
            }

            // Scan directory contents
            for (const auto &entry : std::filesystem::directory_iterator(directory))
            {
                if (!ShouldShowFile(entry.path(), searchFilter))
                {
                    continue;
                }

                items.push_back(CreateAssetItem(entry.path()));
            }

            // Sort items: directories first, then files, both alphabetically
            std::sort(items.begin(), items.end(), [](const AssetItem &a, const AssetItem &b)
                      {
                if (a.isParentDir) return true;
                if (b.isParentDir) return false;
                if (a.isDirectory != b.isDirectory) {
                    return a.isDirectory > b.isDirectory;
                }
                return a.name < b.name; });
        }
        catch (const std::filesystem::filesystem_error &)
        {
            // Directory access failed, return empty list
        }

        return items;
    }

    void LocalAssetProvider::SetSceneDirectory(const std::filesystem::path &sceneDirectory)
    {
        m_RootDirectory = sceneDirectory;

        // Create directory if it doesn't exist
        if (!std::filesystem::exists(m_RootDirectory))
        {
            std::filesystem::create_directories(m_RootDirectory);
        }
    }

    bool LocalAssetProvider::ShouldShowFile(const std::filesystem::path &path,
                                            const std::string &searchFilter) const
    {
        const std::string filename = path.filename().string();

        // Skip hidden files and system files
        if (filename.empty() || filename[0] == '.')
        {
            return false;
        }

        // Apply search filter if provided
        if (!searchFilter.empty())
        {
            std::string lowerFilename = filename;
            std::string lowerFilter = searchFilter;

            std::transform(lowerFilename.begin(), lowerFilename.end(),
                           lowerFilename.begin(), ::tolower);
            std::transform(lowerFilter.begin(), lowerFilter.end(),
                           lowerFilter.begin(), ::tolower);

            if (lowerFilename.find(lowerFilter) == std::string::npos)
            {
                return false;
            }
        }

        return true;
    }

    AssetItem LocalAssetProvider::CreateAssetItem(const std::filesystem::path &path) const
    {
        AssetItem item;
        item.name = path.filename().string();
        item.path = path;
        item.isDirectory = std::filesystem::is_directory(path);
        item.isParentDir = false;
        item.isGlobal = false; // Always false for local assets

        try
        {
            if (!item.isDirectory)
            {
                item.fileSize = std::filesystem::file_size(path);
                item.lastModified = std::filesystem::last_write_time(path);
            }
            else
            {
                item.fileSize = 0;
            }
        }
        catch (const std::filesystem::filesystem_error &)
        {
            item.fileSize = 0;
        }

        return item;
    }
}
