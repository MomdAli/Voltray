#include "ContextMenu.h"
#include <imgui.h>
#include <cstring>

namespace Voltray::Editor::Components::Assets
{
    // Static member definitions
    bool ContextMenu::s_showScriptDialog = false;
    bool ContextMenu::s_showRenameDialog = false;
    bool ContextMenu::s_showFolderDialog = false;
    bool ContextMenu::s_showDeleteDialog = false;

    std::string ContextMenu::s_tempScriptName = "";
    std::string ContextMenu::s_tempScriptType = "C#";
    std::string ContextMenu::s_tempNewName = "";
    std::string ContextMenu::s_tempFolderName = "";
    std::string ContextMenu::s_tempDeleteItemName = "";
    bool ContextMenu::s_tempIsDirectory = false;
    ContextMenuResult ContextMenu::ShowEmptySpaceMenu(const std::filesystem::path &currentPath, bool isGlobal)
    {
        (void)currentPath; // Suppress unused parameter warning
        (void)isGlobal;    // Suppress unused parameter warning
        ContextMenuResult result;

        if (ImGui::BeginPopupContextWindow("AssetBrowserContextMenu"))
        {
            if (ImGui::MenuItem("Create Folder"))
            {
                s_showFolderDialog = true;
                s_tempFolderName = "New Folder";
            }

            if (ImGui::BeginMenu("Create Script"))
            {
                if (ImGui::MenuItem("C# Script"))
                {
                    s_showScriptDialog = true;
                    s_tempScriptName = "NewScript";
                    s_tempScriptType = "C#";
                }

                if (ImGui::MenuItem("JavaScript"))
                {
                    s_showScriptDialog = true;
                    s_tempScriptName = "NewScript";
                    s_tempScriptType = "JavaScript";
                }

                ImGui::EndMenu();
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Paste", nullptr, false, false)) // Disabled for now
            {
                result.type = ContextMenuResult::Paste;
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Refresh"))
            {
                result.type = ContextMenuResult::Refresh;
            }

            if (ImGui::MenuItem("Show in Explorer"))
            {
                result.type = ContextMenuResult::ShowInExplorer;
            }

            ImGui::EndPopup();
        }

        return result;
    }
    ContextMenuResult ContextMenu::ShowAssetMenu(const AssetItem &item, bool isGlobal)
    {
        ContextMenuResult result;
        if (ImGui::BeginPopupContextItem())
        {
            ImGui::Text("%s", item.name.c_str());
            if (isGlobal)
            {
                ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.3f, 1.0f), "(Global Asset)");
            }
            ImGui::Separator();

            // Disable Rename and Delete for global assets
            bool canModify = !isGlobal;

            if (ImGui::MenuItem("Rename", nullptr, false, canModify))
            {
                s_showRenameDialog = true;
                s_tempNewName = item.name;
                result.targetItem = item;
            }

            if (ImGui::MenuItem("Delete", nullptr, false, canModify))
            {
                s_showDeleteDialog = true;
                s_tempDeleteItemName = item.name;
                s_tempIsDirectory = item.isDirectory;
                result.targetItem = item;
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Copy", nullptr, false, false)) // Disabled for now
            {
                result.type = ContextMenuResult::Copy;
                result.targetItem = item;
            }

            if (ImGui::MenuItem("Cut", nullptr, false, false)) // Disabled for now
            {
                result.type = ContextMenuResult::Cut;
                result.targetItem = item;
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Show in Explorer"))
            {
                result.type = ContextMenuResult::ShowInExplorer;
                result.targetItem = item;
            }

            ImGui::EndPopup();
        }

        return result;
    }

    bool ContextMenu::ShowScriptCreationDialog(std::string &scriptName, std::string &scriptType)
    {
        bool shouldCreate = false;

        if (s_showScriptDialog)
        {
            ImGui::OpenPopup("Create Script");
            s_showScriptDialog = false;
        }

        if (ImGui::BeginPopupModal("Create Script", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Create a new script");
            ImGui::Separator(); // Script name input
            char nameBuffer[256];
            strncpy(nameBuffer, s_tempScriptName.c_str(), sizeof(nameBuffer) - 1);
            nameBuffer[sizeof(nameBuffer) - 1] = '\0';

            if (ImGui::InputText("Script Name", nameBuffer, sizeof(nameBuffer)))
            {
                s_tempScriptName = nameBuffer;
            }

            // Script type selection
            const char *scriptTypes[] = {"C#", "JavaScript"};
            int currentType = (s_tempScriptType == "C#") ? 0 : 1;

            if (ImGui::Combo("Script Type", &currentType, scriptTypes, IM_ARRAYSIZE(scriptTypes)))
            {
                s_tempScriptType = scriptTypes[currentType];
            }

            ImGui::Separator();

            if (ImGui::Button("Create"))
            {
                scriptName = s_tempScriptName;
                scriptType = s_tempScriptType;
                shouldCreate = true;
                ImGui::CloseCurrentPopup();
            }

            ImGui::SameLine();

            if (ImGui::Button("Cancel"))
            {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        return shouldCreate;
    }

    bool ContextMenu::ShowRenameDialog(const std::string &currentName, std::string &newName)
    {
        bool shouldRename = false;

        if (s_showRenameDialog)
        {
            ImGui::OpenPopup("Rename Asset");
            s_showRenameDialog = false;
        }

        if (ImGui::BeginPopupModal("Rename Asset", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Rename: %s", currentName.c_str());
            ImGui::Separator();
            char nameBuffer[256];
            strncpy(nameBuffer, s_tempNewName.c_str(), sizeof(nameBuffer) - 1);
            nameBuffer[sizeof(nameBuffer) - 1] = '\0';

            if (ImGui::InputText("New Name", nameBuffer, sizeof(nameBuffer)))
            {
                s_tempNewName = nameBuffer;
            }

            ImGui::Separator();

            bool canRename = !s_tempNewName.empty() && s_tempNewName != currentName;

            if (ImGui::Button("Rename") && canRename)
            {
                newName = s_tempNewName;
                shouldRename = true;
                ImGui::CloseCurrentPopup();
            }

            if (!canRename)
            {
                ImGui::SameLine();
                ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "Invalid name");
            }

            ImGui::SameLine();

            if (ImGui::Button("Cancel"))
            {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        return shouldRename;
    }

    bool ContextMenu::ShowFolderCreationDialog(std::string &folderName)
    {
        bool shouldCreate = false;

        if (s_showFolderDialog)
        {
            ImGui::OpenPopup("Create Folder");
            s_showFolderDialog = false;
        }

        if (ImGui::BeginPopupModal("Create Folder", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Create a new folder");
            ImGui::Separator();
            char nameBuffer[256];
            strncpy(nameBuffer, s_tempFolderName.c_str(), sizeof(nameBuffer) - 1);
            nameBuffer[sizeof(nameBuffer) - 1] = '\0';

            if (ImGui::InputText("Folder Name", nameBuffer, sizeof(nameBuffer)))
            {
                s_tempFolderName = nameBuffer;
            }

            ImGui::Separator();

            bool canCreate = !s_tempFolderName.empty();

            if (ImGui::Button("Create") && canCreate)
            {
                folderName = s_tempFolderName;
                shouldCreate = true;
                ImGui::CloseCurrentPopup();
            }

            if (!canCreate)
            {
                ImGui::SameLine();
                ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "Invalid name");
            }

            ImGui::SameLine();

            if (ImGui::Button("Cancel"))
            {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        return shouldCreate;
    }

    bool ContextMenu::ShowDeleteConfirmation(const std::string &itemName, bool isDirectory)
    {
        bool shouldDelete = false;

        if (s_showDeleteDialog)
        {
            ImGui::OpenPopup("Delete Asset");
            s_showDeleteDialog = false;
        }

        if (ImGui::BeginPopupModal("Delete Asset", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            if (isDirectory)
            {
                ImGui::Text("Are you sure you want to delete the folder:");
                ImGui::Text("'%s'", itemName.c_str());
                ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "This will delete all contents!");
            }
            else
            {
                ImGui::Text("Are you sure you want to delete:");
                ImGui::Text("'%s'", itemName.c_str());
            }

            ImGui::Separator();

            if (ImGui::Button("Delete"))
            {
                shouldDelete = true;
                ImGui::CloseCurrentPopup();
            }

            ImGui::SameLine();

            if (ImGui::Button("Cancel"))
            {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
        return shouldDelete;
    }
}
