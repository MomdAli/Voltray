#pragma once
#include <filesystem>
#include <string>
#include <vector>

/**
 * @file FileOperations.h
 * @brief Advanced file operation utilities for the Assets system
 */

namespace Editor
{
    namespace Assets
    {
        /**
         * @struct FileInfo
         * @brief Extended file information
         */
        struct FileInfo
        {
            std::filesystem::path path;
            std::string name;
            std::string extension;
            std::uintmax_t size;
            std::filesystem::file_time_type lastModified;
            bool isDirectory;
            bool isReadOnly;
            bool isHidden;
            std::string mimeType;
            std::string fileCategory;
        };

        /**
         * @struct CopyResult
         * @brief Result of a copy operation
         */
        struct CopyResult
        {
            bool success;
            std::string errorMessage;
            std::vector<std::filesystem::path> copiedFiles;
            std::vector<std::filesystem::path> failedFiles;
        };

        /**
         * @class FileOperations
         * @brief Advanced file operations for asset management
         */
        class FileOperations
        {
        public:
            /**
             * @brief Get detailed file information
             * @param path Path to the file or directory
             * @return FileInfo structure with detailed information
             */
            static FileInfo GetFileInfo(const std::filesystem::path &path);

            /**
             * @brief Copy files with progress tracking
             * @param sources List of source files/directories
             * @param destination Destination directory
             * @param overwrite Whether to overwrite existing files
             * @return CopyResult with operation details
             */
            static CopyResult CopyFiles(const std::vector<std::filesystem::path> &sources,
                                        const std::filesystem::path &destination,
                                        bool overwrite = false);

            /**
             * @brief Move files with progress tracking
             * @param sources List of source files/directories
             * @param destination Destination directory
             * @param overwrite Whether to overwrite existing files
             * @return CopyResult with operation details
             */
            static CopyResult MoveFiles(const std::vector<std::filesystem::path> &sources,
                                        const std::filesystem::path &destination,
                                        bool overwrite = false);

            /**
             * @brief Create directory with intermediate directories
             * @param path Directory path to create
             * @return True if successful
             */
            static bool CreateDirectoryRecursive(const std::filesystem::path &path);

            /**
             * @brief Delete files and directories safely
             * @param paths List of paths to delete
             * @param moveToTrash Whether to move to recycle bin instead of permanent deletion
             * @return Number of successfully deleted items
             */
            static size_t DeleteFiles(const std::vector<std::filesystem::path> &paths, bool moveToTrash = true);

            /**
             * @brief Generate unique filename if file already exists
             * @param basePath Base file path
             * @return Unique file path
             */
            static std::filesystem::path GenerateUniqueFilename(const std::filesystem::path &basePath);

            /**
             * @brief Search for files matching pattern
             * @param directory Directory to search in
             * @param pattern Search pattern (supports wildcards)
             * @param recursive Whether to search subdirectories
             * @return List of matching files
             */
            static std::vector<std::filesystem::path> FindFiles(const std::filesystem::path &directory,
                                                                const std::string &pattern,
                                                                bool recursive = false);

            /**
             * @brief Get file size in human-readable format
             * @param sizeBytes Size in bytes
             * @return Formatted size string (e.g., "1.5 MB")
             */
            static std::string FormatFileSize(std::uintmax_t sizeBytes);

            /**
             * @brief Get file modification time as string
             * @param fileTime File time
             * @return Formatted time string
             */
            static std::string FormatFileTime(const std::filesystem::file_time_type &fileTime);

            /**
             * @brief Check if file has specific permissions
             * @param path File path
             * @param permission Permission to check
             * @return True if file has the permission
             */
            static bool HasPermission(const std::filesystem::path &path, std::filesystem::perms permission);

            /**
             * @brief Set file permissions
             * @param path File path
             * @param permissions New permissions
             * @return True if successful
             */
            static bool SetPermissions(const std::filesystem::path &path, std::filesystem::perms permissions);

            /**
             * @brief Validate filename for current OS
             * @param filename Filename to validate
             * @return True if filename is valid
             */
            static bool IsValidFilename(const std::string &filename);

            /**
             * @brief Sanitize filename for current OS
             * @param filename Filename to sanitize
             * @return Sanitized filename
             */
            static std::string SanitizeFilename(const std::string &filename);

            /**
             * @brief Get MIME type for file extension
             * @param extension File extension
             * @return MIME type string
             */
            static std::string GetMimeType(const std::string &extension);

            /**
             * @brief Get file category for extension
             * @param extension File extension
             * @return Category string (e.g., "Image", "Audio", "Document")
             */
            static std::string GetFileCategory(const std::string &extension);

            /**
             * @brief Calculate directory size recursively
             * @param directory Directory path
             * @return Total size in bytes
             */
            static std::uintmax_t GetDirectorySize(const std::filesystem::path &directory);

            /**
             * @brief Open file with system default application
             * @param path File path
             * @return True if successful
             */
            static bool OpenWithDefaultApplication(const std::filesystem::path &path);

            /**
             * @brief Show file in system file explorer
             * @param path File path
             * @return True if successful
             */
            static bool ShowInFileExplorer(const std::filesystem::path &path);

            /**
             * @brief Get available disk space for path
             * @param path Directory path
             * @return Available space in bytes
             */
            static std::uintmax_t GetAvailableSpace(const std::filesystem::path &path);

            /**
             * @brief Check if path is on a network drive
             * @param path Path to check
             * @return True if on network drive
             */
            static bool IsNetworkPath(const std::filesystem::path &path);

            /**
             * @brief Backup file with timestamp
             * @param filePath File to backup
             * @return Path to backup file, empty if failed
             */
            static std::filesystem::path CreateBackup(const std::filesystem::path &filePath);

            /**
             * @brief Compare two files for equality
             * @param file1 First file path
             * @param file2 Second file path
             * @return True if files are identical
             */
            static bool CompareFiles(const std::filesystem::path &file1, const std::filesystem::path &file2);

        private:
            /**
             * @brief Copy single file with error handling
             * @param source Source file path
             * @param destination Destination file path
             * @param overwrite Whether to overwrite existing file
             * @return True if successful
             */
            static bool CopySingleFile(const std::filesystem::path &source,
                                       const std::filesystem::path &destination,
                                       bool overwrite);

            /**
             * @brief Copy directory recursively
             * @param source Source directory path
             * @param destination Destination directory path
             * @param overwrite Whether to overwrite existing files
             * @return CopyResult with operation details
             */
            static CopyResult CopyDirectoryRecursive(const std::filesystem::path &source,
                                                     const std::filesystem::path &destination,
                                                     bool overwrite);

            /**
             * @brief Move to recycle bin (Windows implementation)
             * @param path File path to move to recycle bin
             * @return True if successful
             */
            static bool MoveToRecycleBin(const std::filesystem::path &path);

            /**
             * @brief Match pattern against filename (supports wildcards)
             * @param filename Filename to match
             * @param pattern Pattern with wildcards (* and ?)
             * @return True if filename matches pattern
             */
            static bool MatchPattern(const std::string &filename, const std::string &pattern);
        };
    }
}
