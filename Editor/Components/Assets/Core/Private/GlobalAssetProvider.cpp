#include "GlobalAssetProvider.h"
#include "UserDataManager.h"
#include <algorithm>
#include <cctype>

using Voltray::Utils::UserDataManager;

namespace Voltray::Editor::Components::Assets
{
    GlobalAssetProvider::GlobalAssetProvider()
    {
        // Default initialization - directory will be set via Initialize()
    }

    void GlobalAssetProvider::Initialize(const std::string &rootDirectory)
    {
        m_RootDirectory = std::filesystem::path(rootDirectory);

        // Create directory if it doesn't exist
        if (!std::filesystem::exists(m_RootDirectory))
        {
            std::filesystem::create_directories(m_RootDirectory);
        }
    }

    std::vector<AssetItem> GlobalAssetProvider::GetDirectoryContents(
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
                parentItem.isGlobal = true;
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
    bool GlobalAssetProvider::ShouldShowFile(const std::filesystem::path &path,
                                             const std::string &searchFilter) const
    {
        return m_AssetFilter.ShouldShowFile(path, searchFilter);
    }

    AssetItem GlobalAssetProvider::CreateAssetItem(const std::filesystem::path &path) const
    {
        AssetItem item;
        item.name = path.filename().string();
        item.path = path;
        item.isDirectory = std::filesystem::is_directory(path);
        item.isParentDir = false;
        item.isGlobal = true; // Always true for global assets

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
