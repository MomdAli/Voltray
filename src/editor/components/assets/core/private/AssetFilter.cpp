#include "AssetFilter.h"
#include <algorithm>
#include <cctype>

namespace Voltray::Editor::Components::Assets
{
    // Static extension definitions

    // System files (temporary, cache, logs, etc.)
    const std::unordered_set<std::string> AssetFilter::s_SystemExtensions = {
        ".tmp", ".temp", ".cache", ".log", ".bak", ".backup", ".old", ".orig",
        ".swp", ".swo", ".lock", ".pid", ".dump", ".dmp", ".crash", ".stackdump",
        ".thumbs.db", ".ds_store", ".desktop.ini", ".directory", ".fuse_hidden"};

    // Project and build files
    const std::unordered_set<std::string> AssetFilter::s_ProjectExtensions = {
        ".vcxproj", ".vcxproj.filters", ".vcxproj.user", ".sln", ".suo", ".user",
        ".makefile", ".make", ".cmake", ".cmakecache.txt", ".ninja", ".gyp", ".gypi",
        ".gradle", ".gradlew", ".ant", ".maven", ".pom", ".project", ".classpath",
        ".cproject", ".settings", ".metadata", ".idea", ".iml", ".ipr", ".iws",
        ".xcodeproj", ".xcworkspace", ".pbxproj", ".plist", ".entitlements"};

    // IDE-specific files
    const std::unordered_set<std::string> AssetFilter::s_IDEExtensions = {
        ".vscode", ".vs", ".idea", ".eclipse", ".netbeans", ".qtcreator",
        ".sublime-project", ".sublime-workspace", ".atom", ".brackets",
        ".codelite", ".cbp", ".dev", ".dsw", ".dsp", ".ncb", ".aps", ".clw"};

    // Version control files
    const std::unordered_set<std::string> AssetFilter::s_VersionControlExtensions = {
        ".git", ".gitignore", ".gitattributes", ".gitmodules", ".gitkeep",
        ".svn", ".hg", ".hgignore", ".bzr", ".bzrignore", ".cvs", ".cvsignore",
        ".p4ignore", ".tfignore", ".darcs"};

    // Archive and compressed files
    const std::unordered_set<std::string> AssetFilter::s_ArchiveExtensions = {
        ".zip", ".rar", ".7z", ".tar", ".gz", ".bz2", ".xz", ".lzma", ".lz",
        ".cab", ".iso", ".dmg", ".pkg", ".deb", ".rpm", ".msi", ".exe",
        ".war", ".jar", ".ear", ".aar", ".apk"};

    // Executable files
    const std::unordered_set<std::string> AssetFilter::s_ExecutableExtensions = {
        ".exe", ".dll", ".so", ".dylib", ".lib", ".a", ".o", ".obj", ".pdb",
        ".ilk", ".exp", ".com", ".bat", ".cmd", ".ps1", ".sh", ".bash",
        ".app", ".bundle", ".framework"};

    // Large media files (might want to filter for performance)
    const std::unordered_set<std::string> AssetFilter::s_MediaExtensions = {
        ".avi", ".mov", ".mp4", ".mkv", ".wmv", ".flv", ".webm", ".m4v",
        ".mp3", ".wav", ".flac", ".ogg", ".wma", ".aac", ".m4a", ".opus",
        ".psd", ".ai", ".eps", ".indd", ".sketch", ".fig", ".xd",
        ".blend", ".max", ".maya", ".c4d", ".3ds", ".obj", ".fbx", ".dae"};

    // Document files
    const std::unordered_set<std::string> AssetFilter::s_DocumentExtensions = {
        ".doc", ".docx", ".pdf", ".rtf", ".odt", ".pages", ".tex", ".latex",
        ".xls", ".xlsx", ".ods", ".numbers", ".csv", ".tsv",
        ".ppt", ".pptx", ".odp", ".key", ".prezi"};

    AssetFilter::AssetFilter()
    {
        InitializeDefaultExtensions();
    }

    void AssetFilter::InitializeDefaultExtensions()
    {
        // By default, filter out system and project files
        m_FilteredCategories.insert(AssetFilterCategory::System);
        m_FilteredCategories.insert(AssetFilterCategory::Project);
        m_FilteredCategories.insert(AssetFilterCategory::IDE);
        m_FilteredCategories.insert(AssetFilterCategory::VersionControl);

        // Keep archives, executables, media, and documents visible by default
        // Users can choose to filter these if needed
    }

    bool AssetFilter::ShouldShowFile(const std::filesystem::path &path, const std::string &searchFilter) const
    {
        const std::string filename = path.filename().string();

        // Skip empty filenames
        if (filename.empty())
        {
            return false;
        }

        // Skip hidden files (starting with dot) - this is a universal rule
        if (filename[0] == '.')
        {
            return false;
        }

        // Check extension-based filtering
        if (path.has_extension())
        {
            const std::string extension = NormalizeExtension(path.extension().string());

            // Check if this extension is in a filtered category
            AssetFilterCategory category = GetExtensionCategory(extension);
            if (category != AssetFilterCategory::All && IsCategoryFiltered(category))
            {
                return false;
            }

            // Check custom filtered extensions
            if (m_CustomFilteredExtensions.find(extension) != m_CustomFilteredExtensions.end())
            {
                return false;
            }
        }

        // Apply search filter if provided
        if (!searchFilter.empty())
        {
            if (!MatchesSearchFilter(filename, searchFilter))
            {
                return false;
            }
        }

        return true;
    }

    bool AssetFilter::IsFilteredExtension(const std::string &extension) const
    {
        const std::string normalizedExt = NormalizeExtension(extension);

        // Check custom filtered extensions
        if (m_CustomFilteredExtensions.find(normalizedExt) != m_CustomFilteredExtensions.end())
        {
            return true;
        }

        // Check category-based filtering
        AssetFilterCategory category = GetExtensionCategory(normalizedExt);
        return category != AssetFilterCategory::All && IsCategoryFiltered(category);
    }

    void AssetFilter::SetCategoryFiltering(AssetFilterCategory category, bool enabled)
    {
        if (enabled)
        {
            m_FilteredCategories.insert(category);
        }
        else
        {
            m_FilteredCategories.erase(category);
        }
    }

    bool AssetFilter::IsCategoryFiltered(AssetFilterCategory category) const
    {
        return m_FilteredCategories.find(category) != m_FilteredCategories.end();
    }

    void AssetFilter::AddFilteredExtension(const std::string &extension)
    {
        m_CustomFilteredExtensions.insert(NormalizeExtension(extension));
    }

    void AssetFilter::RemoveFilteredExtension(const std::string &extension)
    {
        m_CustomFilteredExtensions.erase(NormalizeExtension(extension));
    }

    std::vector<std::string> AssetFilter::GetExtensionsInCategory(AssetFilterCategory category) const
    {
        const std::unordered_set<std::string> *extensionSet = nullptr;

        switch (category)
        {
        case AssetFilterCategory::System:
            extensionSet = &s_SystemExtensions;
            break;
        case AssetFilterCategory::Project:
            extensionSet = &s_ProjectExtensions;
            break;
        case AssetFilterCategory::IDE:
            extensionSet = &s_IDEExtensions;
            break;
        case AssetFilterCategory::VersionControl:
            extensionSet = &s_VersionControlExtensions;
            break;
        case AssetFilterCategory::Archive:
            extensionSet = &s_ArchiveExtensions;
            break;
        case AssetFilterCategory::Executable:
            extensionSet = &s_ExecutableExtensions;
            break;
        case AssetFilterCategory::Media:
            extensionSet = &s_MediaExtensions;
            break;
        case AssetFilterCategory::Document:
            extensionSet = &s_DocumentExtensions;
            break;
        default:
            return {};
        }

        return std::vector<std::string>(extensionSet->begin(), extensionSet->end());
    }

    std::string AssetFilter::GetCategoryDisplayName(AssetFilterCategory category)
    {
        switch (category)
        {
        case AssetFilterCategory::System:
            return "System Files";
        case AssetFilterCategory::Project:
            return "Project Files";
        case AssetFilterCategory::IDE:
            return "IDE Files";
        case AssetFilterCategory::VersionControl:
            return "Version Control";
        case AssetFilterCategory::Archive:
            return "Archives";
        case AssetFilterCategory::Executable:
            return "Executables";
        case AssetFilterCategory::Media:
            return "Media Files";
        case AssetFilterCategory::Document:
            return "Documents";
        case AssetFilterCategory::All:
            return "All Categories";
        default:
            return "Unknown";
        }
    }

    void AssetFilter::ResetToDefaults()
    {
        m_FilteredCategories.clear();
        m_CustomFilteredExtensions.clear();
        InitializeDefaultExtensions();
    }

    AssetFilterCategory AssetFilter::GetExtensionCategory(const std::string &extension) const
    {
        if (s_SystemExtensions.find(extension) != s_SystemExtensions.end())
            return AssetFilterCategory::System;
        if (s_ProjectExtensions.find(extension) != s_ProjectExtensions.end())
            return AssetFilterCategory::Project;
        if (s_IDEExtensions.find(extension) != s_IDEExtensions.end())
            return AssetFilterCategory::IDE;
        if (s_VersionControlExtensions.find(extension) != s_VersionControlExtensions.end())
            return AssetFilterCategory::VersionControl;
        if (s_ArchiveExtensions.find(extension) != s_ArchiveExtensions.end())
            return AssetFilterCategory::Archive;
        if (s_ExecutableExtensions.find(extension) != s_ExecutableExtensions.end())
            return AssetFilterCategory::Executable;
        if (s_MediaExtensions.find(extension) != s_MediaExtensions.end())
            return AssetFilterCategory::Media;
        if (s_DocumentExtensions.find(extension) != s_DocumentExtensions.end())
            return AssetFilterCategory::Document;

        return AssetFilterCategory::All; // Not categorized
    }

    std::string AssetFilter::NormalizeExtension(const std::string &extension) const
    {
        std::string normalized = extension;

        // Convert to lowercase
        std::transform(normalized.begin(), normalized.end(), normalized.begin(), ::tolower);

        // Ensure it starts with a dot
        if (!normalized.empty() && normalized[0] != '.')
        {
            normalized = "." + normalized;
        }

        return normalized;
    }

    bool AssetFilter::MatchesSearchFilter(const std::string &filename, const std::string &searchFilter) const
    {
        std::string lowerFilename = filename;
        std::string lowerFilter = searchFilter;

        std::transform(lowerFilename.begin(), lowerFilename.end(),
                       lowerFilename.begin(), ::tolower);
        std::transform(lowerFilter.begin(), lowerFilter.end(),
                       lowerFilter.begin(), ::tolower);

        return lowerFilename.find(lowerFilter) != std::string::npos;
    }
}
