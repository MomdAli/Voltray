#pragma once
#include "../Core/AssetItem.h"
#include "../Core/AssetManager.h"
#include <functional>

/**
 * @file ContextMenu.h
 * @brief Handles right-click context menus for assets
 */

namespace Editor
{
    namespace Assets
    {
        /**
         * @struct ContextMenuResult
         * @brief Result of a context menu action
         */
        struct ContextMenuResult
        {
            enum Type
            {
                None,
                CreateFolder,
                CreateScript,
                Rename,
                Delete,
                Copy,
                Cut,
                Paste,
                ShowInExplorer,
                Refresh
            };

            Type type = None;
            std::string stringParam; // For folder name, script name, etc.
            AssetItem targetItem;    // The item the action was performed on
        };

        /**
         * @class ContextMenu
         * @brief Manages context menus for the asset browser
         */
        class ContextMenu
        {
        public:
            /**
             * @brief Show context menu for empty space in the browser
             * @param currentPath Current directory path
             * @param isGlobal Whether this is global assets
             * @return Context menu result
             */
            static ContextMenuResult ShowEmptySpaceMenu(const std::filesystem::path &currentPath, bool isGlobal);

            /**
             * @brief Show context menu for a specific asset item
             * @param item The asset item
             * @param isGlobal Whether this is global assets
             * @return Context menu result
             */
            static ContextMenuResult ShowAssetMenu(const AssetItem &item, bool isGlobal);

            /**
             * @brief Show script creation submenu
             * @param scriptName Output parameter for script name
             * @param scriptType Output parameter for script type
             * @return True if script should be created
             */
            static bool ShowScriptCreationDialog(std::string &scriptName, std::string &scriptType);

            /**
             * @brief Show rename dialog
             * @param currentName Current name of the item
             * @param newName Output parameter for new name
             * @return True if item should be renamed
             */
            static bool ShowRenameDialog(const std::string &currentName, std::string &newName);

            /**
             * @brief Show folder creation dialog
             * @param folderName Output parameter for folder name
             * @return True if folder should be created
             */
            static bool ShowFolderCreationDialog(std::string &folderName);

            /**
             * @brief Show delete confirmation dialog
             * @param itemName Name of the item to delete
             * @param isDirectory Whether it's a directory
             * @return True if item should be deleted
             */
            static bool ShowDeleteConfirmation(const std::string &itemName, bool isDirectory);

        private:
            static bool s_showScriptDialog;
            static bool s_showRenameDialog;
            static bool s_showFolderDialog;
            static bool s_showDeleteDialog;

            static std::string s_tempScriptName;
            static std::string s_tempScriptType;
            static std::string s_tempNewName;
            static std::string s_tempFolderName;
            static std::string s_tempDeleteItemName;
            static bool s_tempIsDirectory;
        };
    }
}
