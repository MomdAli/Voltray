#include "FileOperations.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <regex>
#include <chrono>
#include <ctime>
#include <unordered_map>

#ifdef _WIN32
#include <windows.h>
#include <shellapi.h>
#include <shlobj.h>
#else
#include <unistd.h>
#include <sys/stat.h>
#endif

namespace Voltray::Editor::Components::Assets
{
    FileInfo FileOperations::GetFileInfo(const std::filesystem::path &path)
    {
        FileInfo info;
        info.path = path;
        info.name = path.filename().string();
        info.extension = path.extension().string();

        std::error_code ec;

        // Get basic file attributes
        if (std::filesystem::exists(path, ec) && !ec)
        {
            info.isDirectory = std::filesystem::is_directory(path, ec);

            if (!info.isDirectory && !ec)
            {
                info.size = std::filesystem::file_size(path, ec);
                if (ec)
                    info.size = 0;
            }
            else
            {
                info.size = GetDirectorySize(path);
            }
            info.lastModified = std::filesystem::last_write_time(path, ec);
            if (ec)
                info.lastModified = std::filesystem::file_time_type{};

#ifdef _WIN32
            // Check for hidden attribute on Windows
            DWORD attributes = GetFileAttributesA(path.string().c_str());
            info.isHidden = (attributes != INVALID_FILE_ATTRIBUTES) && (attributes & FILE_ATTRIBUTE_HIDDEN);
#else
            // On Unix-like systems, files starting with '.' are hidden
            info.isHidden = info.name.length() > 0 && info.name[0] == '.';
#endif
        }
        else
        {
            // File doesn't exist or error occurred
            info.isDirectory = false;
            info.size = 0;
            info.lastModified = std::filesystem::file_time_type{};
            info.isHidden = false;
        }

        info.mimeType = GetMimeType(info.extension);
        info.fileCategory = GetFileCategory(info.extension);

        return info;
    }

    CopyResult FileOperations::CopyFiles(const std::vector<std::filesystem::path> &sources,
                                         const std::filesystem::path &destination,
                                         bool overwrite)
    {
        CopyResult result;
        result.success = true;

        // Ensure destination directory exists
        if (!CreateDirectoryRecursive(destination))
        {
            result.success = false;
            result.errorMessage = "Failed to create destination directory: " + destination.string();
            return result;
        }

        for (const auto &source : sources)
        {
            std::error_code ec;

            if (std::filesystem::is_directory(source, ec))
            {
                // Copy directory recursively
                auto dirResult = CopyDirectoryRecursive(source, destination / source.filename(), overwrite);
                if (!dirResult.success)
                {
                    result.success = false;
                    result.errorMessage += dirResult.errorMessage + "; ";
                }
                result.copiedFiles.insert(result.copiedFiles.end(), dirResult.copiedFiles.begin(), dirResult.copiedFiles.end());
                result.failedFiles.insert(result.failedFiles.end(), dirResult.failedFiles.begin(), dirResult.failedFiles.end());
            }
            else
            {
                // Copy single file
                auto destFile = destination / source.filename();
                if (CopySingleFile(source, destFile, overwrite))
                {
                    result.copiedFiles.push_back(destFile);
                }
                else
                {
                    result.success = false;
                    result.failedFiles.push_back(source);
                    result.errorMessage += "Failed to copy " + source.string() + "; ";
                }
            }
        }

        return result;
    }

    CopyResult FileOperations::MoveFiles(const std::vector<std::filesystem::path> &sources,
                                         const std::filesystem::path &destination,
                                         bool overwrite)
    {
        // First copy files
        auto copyResult = CopyFiles(sources, destination, overwrite);

        if (copyResult.success)
        {
            // Delete original files after successful copy
            for (const auto &source : sources)
            {
                std::error_code ec;
                std::filesystem::remove_all(source, ec);
                if (ec)
                {
                    copyResult.success = false;
                    copyResult.errorMessage += "Failed to remove original file: " + source.string() + "; ";
                }
            }
        }

        return copyResult;
    }

    bool FileOperations::CreateDirectoryRecursive(const std::filesystem::path &path)
    {
        std::error_code ec;
        return std::filesystem::create_directories(path, ec) || std::filesystem::exists(path, ec);
    }

    size_t FileOperations::DeleteFiles(const std::vector<std::filesystem::path> &paths, bool moveToTrash)
    {
        size_t deletedCount = 0;

        for (const auto &path : paths)
        {
            bool success = false;

            if (moveToTrash)
            {
                success = MoveToRecycleBin(path);
            }
            else
            {
                std::error_code ec;
                success = std::filesystem::remove_all(path, ec) > 0;
            }

            if (success)
            {
                deletedCount++;
            }
        }

        return deletedCount;
    }

    std::filesystem::path FileOperations::GenerateUniqueFilename(const std::filesystem::path &basePath)
    {
        if (!std::filesystem::exists(basePath))
        {
            return basePath;
        }

        auto parent = basePath.parent_path();
        auto stem = basePath.stem().string();
        auto extension = basePath.extension().string();

        int counter = 1;
        std::filesystem::path uniquePath;

        do
        {
            std::ostringstream oss;
            oss << stem << " (" << counter << ")" << extension;
            uniquePath = parent / oss.str();
            counter++;
        } while (std::filesystem::exists(uniquePath) && counter < 1000);

        return uniquePath;
    }

    std::vector<std::filesystem::path> FileOperations::FindFiles(const std::filesystem::path &directory,
                                                                 const std::string &pattern,
                                                                 bool recursive)
    {
        std::vector<std::filesystem::path> results;
        std::error_code ec;
        if (!std::filesystem::exists(directory, ec) || !std::filesystem::is_directory(directory, ec))
        {
            return results;
        }

        if (recursive)
        {
            std::filesystem::recursive_directory_iterator iterator(directory, ec);
            if (ec)
                return results;

            for (const auto &entry : iterator)
            {
                if (entry.is_regular_file(ec) && !ec)
                {
                    std::string filename = entry.path().filename().string();
                    if (MatchPattern(filename, pattern))
                    {
                        results.push_back(entry.path());
                    }
                }
            }
        }
        else
        {
            std::filesystem::directory_iterator iterator(directory, ec);
            if (ec)
                return results;

            for (const auto &entry : iterator)
            {
                if (entry.is_regular_file(ec) && !ec)
                {
                    std::string filename = entry.path().filename().string();
                    if (MatchPattern(filename, pattern))
                    {
                        results.push_back(entry.path());
                    }
                }
            }
        }

        return results;
    }

    std::string FileOperations::FormatFileSize(std::uintmax_t sizeBytes)
    {
        const char *units[] = {"B", "KB", "MB", "GB", "TB"};
        const int numUnits = sizeof(units) / sizeof(units[0]);

        double size = static_cast<double>(sizeBytes);
        int unitIndex = 0;

        while (size >= 1024.0 && unitIndex < numUnits - 1)
        {
            size /= 1024.0;
            unitIndex++;
        }

        std::ostringstream oss;
        if (unitIndex == 0)
        {
            oss << static_cast<int>(size) << " " << units[unitIndex];
        }
        else
        {
            oss << std::fixed << std::setprecision(1) << size << " " << units[unitIndex];
        }

        return oss.str();
    }

    std::string FileOperations::FormatFileTime(const std::filesystem::file_time_type &fileTime)
    {
        auto systemTime = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
            fileTime - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now());

        auto time_t = std::chrono::system_clock::to_time_t(systemTime);

        std::ostringstream oss;
        oss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

    bool FileOperations::HasPermission(const std::filesystem::path &path, std::filesystem::perms permission)
    {
        std::error_code ec;
        auto perms = std::filesystem::status(path, ec).permissions();
        return !ec && (perms & permission) != std::filesystem::perms::none;
    }

    bool FileOperations::SetPermissions(const std::filesystem::path &path, std::filesystem::perms permissions)
    {
        std::error_code ec;
        std::filesystem::permissions(path, permissions, ec);
        return !ec;
    }

    bool FileOperations::IsValidFilename(const std::string &filename)
    {
        if (filename.empty() || filename.length() > 255)
            return false;

#ifdef _WIN32
        // Windows forbidden characters
        const std::string forbidden = "<>:\"/\\|?*";
        if (filename.find_first_of(forbidden) != std::string::npos)
            return false;

        // Windows reserved names
        const std::vector<std::string> reserved = {
            "CON", "PRN", "AUX", "NUL",
            "COM1", "COM2", "COM3", "COM4", "COM5", "COM6", "COM7", "COM8", "COM9",
            "LPT1", "LPT2", "LPT3", "LPT4", "LPT5", "LPT6", "LPT7", "LPT8", "LPT9"};

        std::string upper = filename;
        std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);

        for (const auto &name : reserved)
        {
            if (upper == name || upper.substr(0, name.length() + 1) == name + ".")
                return false;
        }
#endif

        return true;
    }

    std::string FileOperations::SanitizeFilename(const std::string &filename)
    {
        std::string result = filename;

#ifdef _WIN32
        const std::string forbidden = "<>:\"/\\|?*";
        for (char &c : result)
        {
            if (forbidden.find(c) != std::string::npos)
                c = '_';
        }
#endif

        // Remove leading/trailing spaces and dots
        result.erase(0, result.find_first_not_of(" ."));
        result.erase(result.find_last_not_of(" .") + 1);

        if (result.empty())
            result = "unnamed";

        return result;
    }

    std::string FileOperations::GetMimeType(const std::string &extension)
    {
        std::string ext = extension;
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

        // Common MIME types
        static const std::unordered_map<std::string, std::string> mimeTypes = {
            // Images
            {".jpg", "image/jpeg"},
            {".jpeg", "image/jpeg"},
            {".png", "image/png"},
            {".gif", "image/gif"},
            {".bmp", "image/bmp"},
            {".tiff", "image/tiff"},
            {".webp", "image/webp"},
            {".svg", "image/svg+xml"},

            // Audio
            {".mp3", "audio/mpeg"},
            {".wav", "audio/wav"},
            {".ogg", "audio/ogg"},
            {".flac", "audio/flac"},
            {".aac", "audio/aac"},

            // Video
            {".mp4", "video/mp4"},
            {".avi", "video/x-msvideo"},
            {".mkv", "video/x-matroska"},
            {".mov", "video/quicktime"},
            {".wmv", "video/x-ms-wmv"},

            // Documents
            {".pdf", "application/pdf"},
            {".doc", "application/msword"},
            {".docx", "application/vnd.openxmlformats-officedocument.wordprocessingml.document"},
            {".txt", "text/plain"},
            {".rtf", "application/rtf"},

            // Code
            {".cpp", "text/x-c++"},
            {".h", "text/x-c"},
            {".js", "application/javascript"},
            {".html", "text/html"},
            {".css", "text/css"},
            {".json", "application/json"},
            {".xml", "application/xml"},
            {".py", "text/x-python"},

            // 3D Models
            {".obj", "model/obj"},
            {".fbx", "model/fbx"},
            {".dae", "model/vnd.collada+xml"},
            {".gltf", "model/gltf+json"},
            {".glb", "model/gltf-binary"},

            // Archives
            {".zip", "application/zip"},
            {".rar", "application/vnd.rar"},
            {".7z", "application/x-7z-compressed"}};

        auto it = mimeTypes.find(ext);
        return it != mimeTypes.end() ? it->second : "application/octet-stream";
    }

    std::string FileOperations::GetFileCategory(const std::string &extension)
    {
        std::string ext = extension;
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

        // Image formats
        if (ext == ".jpg" || ext == ".jpeg" || ext == ".png" || ext == ".gif" ||
            ext == ".bmp" || ext == ".tiff" || ext == ".webp" || ext == ".svg")
            return "Image";

        // Audio formats
        if (ext == ".mp3" || ext == ".wav" || ext == ".ogg" || ext == ".flac" || ext == ".aac")
            return "Audio";

        // Video formats
        if (ext == ".mp4" || ext == ".avi" || ext == ".mkv" || ext == ".mov" || ext == ".wmv")
            return "Video";

        // 3D model formats
        if (ext == ".obj" || ext == ".fbx" || ext == ".dae" || ext == ".gltf" ||
            ext == ".glb" || ext == ".3ds" || ext == ".blend" || ext == ".max")
            return "3D Model";

        // Document formats
        if (ext == ".pdf" || ext == ".doc" || ext == ".docx" || ext == ".txt" ||
            ext == ".rtf" || ext == ".odt")
            return "Document";

        // Code formats
        if (ext == ".cpp" || ext == ".h" || ext == ".hpp" || ext == ".c" || ext == ".cc" ||
            ext == ".js" || ext == ".ts" || ext == ".html" || ext == ".css" || ext == ".json" ||
            ext == ".xml" || ext == ".py" || ext == ".java" || ext == ".cs" || ext == ".lua")
            return "Code";

        // Archive formats
        if (ext == ".zip" || ext == ".rar" || ext == ".7z" || ext == ".tar" || ext == ".gz")
            return "Archive";

        // Texture formats (subset of images for specific use)
        if (ext == ".dds" || ext == ".tga" || ext == ".exr" || ext == ".hdr")
            return "Texture";

        return "Unknown";
    }

    std::uintmax_t FileOperations::GetDirectorySize(const std::filesystem::path &directory)
    {
        std::uintmax_t totalSize = 0;
        std::error_code ec;

        for (const auto &entry : std::filesystem::recursive_directory_iterator(directory, ec))
        {
            if (entry.is_regular_file(ec) && !ec)
            {
                totalSize += std::filesystem::file_size(entry.path(), ec);
            }
        }

        return totalSize;
    }

    bool FileOperations::OpenWithDefaultApplication(const std::filesystem::path &path)
    {
#ifdef _WIN32
        std::string pathStr = path.string();
        HINSTANCE result = ShellExecuteA(nullptr, "open", pathStr.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
        return reinterpret_cast<uintptr_t>(result) > 32;
#else
        std::string command = "xdg-open \"" + path.string() + "\"";
        return system(command.c_str()) == 0;
#endif
    }

    bool FileOperations::ShowInFileExplorer(const std::filesystem::path &path)
    {
#ifdef _WIN32
        std::string pathStr = path.string();
        std::string command = "explorer /select,\"" + pathStr + "\"";
        return system(command.c_str()) == 0;
#else
        std::string command = "xdg-open \"" + path.parent_path().string() + "\"";
        return system(command.c_str()) == 0;
#endif
    }

    std::uintmax_t FileOperations::GetAvailableSpace(const std::filesystem::path &path)
    {
        std::error_code ec;
        auto spaceInfo = std::filesystem::space(path, ec);
        return ec ? 0 : spaceInfo.available;
    }
    bool FileOperations::IsNetworkPath(const std::filesystem::path &path)
    {
#ifdef _WIN32
        std::string pathStr = path.string();
        return pathStr.length() >= 2 && pathStr.substr(0, 2) == "\\\\";
#else
        (void)path; // Suppress unused parameter warning
        // On Unix systems, network paths are usually mounted, so we can't easily detect them
        return false;
#endif
    }

    std::filesystem::path FileOperations::CreateBackup(const std::filesystem::path &filePath)
    {
        if (!std::filesystem::exists(filePath))
            return {};

        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);

        std::ostringstream oss;
        oss << std::put_time(std::localtime(&time_t), "%Y%m%d_%H%M%S");

        auto backupPath = filePath.parent_path() / (filePath.stem().string() + "_backup_" + oss.str() + filePath.extension().string());

        std::error_code ec;
        if (std::filesystem::copy_file(filePath, backupPath, ec))
        {
            return backupPath;
        }

        return {};
    }

    bool FileOperations::CompareFiles(const std::filesystem::path &file1, const std::filesystem::path &file2)
    {
        std::error_code ec;

        // Check if both files exist and have the same size
        if (!std::filesystem::exists(file1, ec) || !std::filesystem::exists(file2, ec))
            return false;

        if (std::filesystem::file_size(file1, ec) != std::filesystem::file_size(file2, ec))
            return false;

        // Compare content
        std::ifstream f1(file1, std::ios::binary);
        std::ifstream f2(file2, std::ios::binary);

        if (!f1.is_open() || !f2.is_open())
            return false;

        return std::equal(std::istreambuf_iterator<char>(f1.rdbuf()),
                          std::istreambuf_iterator<char>(),
                          std::istreambuf_iterator<char>(f2.rdbuf()));
    }

    // Private helper methods

    bool FileOperations::CopySingleFile(const std::filesystem::path &source,
                                        const std::filesystem::path &destination,
                                        bool overwrite)
    {
        std::error_code ec;
        auto options = overwrite ? std::filesystem::copy_options::overwrite_existing : std::filesystem::copy_options::skip_existing;

        return std::filesystem::copy_file(source, destination, options, ec) && !ec;
    }

    CopyResult FileOperations::CopyDirectoryRecursive(const std::filesystem::path &source,
                                                      const std::filesystem::path &destination,
                                                      bool overwrite)
    {
        CopyResult result;
        result.success = true;

        std::error_code ec;
        if (!CreateDirectoryRecursive(destination))
        {
            result.success = false;
            result.errorMessage = "Failed to create directory: " + destination.string();
            return result;
        }

        for (const auto &entry : std::filesystem::recursive_directory_iterator(source, ec))
        {
            if (ec)
                break;

            auto relativePath = std::filesystem::relative(entry.path(), source, ec);
            if (ec)
                continue;

            auto destPath = destination / relativePath;

            if (entry.is_directory(ec))
            {
                if (!CreateDirectoryRecursive(destPath))
                {
                    result.success = false;
                    result.failedFiles.push_back(entry.path());
                }
            }
            else if (entry.is_regular_file(ec))
            {
                if (CopySingleFile(entry.path(), destPath, overwrite))
                {
                    result.copiedFiles.push_back(destPath);
                }
                else
                {
                    result.success = false;
                    result.failedFiles.push_back(entry.path());
                }
            }
        }

        return result;
    }

    bool FileOperations::MoveToRecycleBin(const std::filesystem::path &path)
    {
#ifdef _WIN32
        std::string pathStr = path.string();
        pathStr.push_back('\0'); // Double null terminated

        SHFILEOPSTRUCTA fileOp = {};
        fileOp.wFunc = FO_DELETE;
        fileOp.pFrom = pathStr.c_str();
        fileOp.fFlags = FOF_ALLOWUNDO | FOF_NOCONFIRMATION | FOF_SILENT;

        return SHFileOperationA(&fileOp) == 0;
#else
        // On Unix systems, move to .Trash if available
        std::string homeDir = getenv("HOME");
        if (homeDir.empty())
            return false;

        std::filesystem::path trashDir = std::filesystem::path(homeDir) / ".local" / "share" / "Trash" / "files";

        std::error_code ec;
        if (!std::filesystem::exists(trashDir, ec))
        {
            if (!CreateDirectoryRecursive(trashDir))
                return false;
        }
        auto trashPath = trashDir / path.filename();
        trashPath = GenerateUniqueFilename(trashPath);

        std::filesystem::rename(path, trashPath, ec);
        return !ec;
#endif
    }

    bool FileOperations::MatchPattern(const std::string &filename, const std::string &pattern)
    {
        // Convert wildcard pattern to regex
        std::string regexPattern = pattern;

        // Escape special regex characters except * and ?
        std::string specialChars = ".^$+{}[]|()\\";
        for (char c : specialChars)
        {
            size_t pos = 0;
            std::string replacement = std::string("\\") + c;
            while ((pos = regexPattern.find(c, pos)) != std::string::npos)
            {
                regexPattern.replace(pos, 1, replacement);
                pos += replacement.length();
            }
        }

        // Convert wildcards to regex
        size_t pos = 0;
        while ((pos = regexPattern.find("\\*", pos)) != std::string::npos)
        {
            regexPattern.replace(pos, 2, ".*");
            pos += 2;
        }

        pos = 0;
        while ((pos = regexPattern.find("\\?", pos)) != std::string::npos)
        {
            regexPattern.replace(pos, 2, ".");
            pos += 1;
        }

        try
        {
            std::regex regex(regexPattern, std::regex_constants::icase);
            return std::regex_match(filename, regex);
        }
        catch (const std::regex_error &)
        {
            return false;
        }
    }
}