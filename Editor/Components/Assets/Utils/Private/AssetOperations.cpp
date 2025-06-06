#include "AssetOperations.h"
#include <imgui.h>
#include <cstring>
#include <fstream>

namespace Voltray::Editor::Components::Assets
{
    bool AssetOperations::ShowContextMenu(const AssetItem &item, std::shared_ptr<AssetProvider> provider)
    {
        bool actionPerformed = false;

        if (ImGui::BeginPopupContextItem())
        {
            const bool canModify = provider->CanModifyAssets();

            if (item.isDirectory)
            {
                // Directory context menu
                if (canModify && ImGui::MenuItem("Create Folder"))
                {
                    std::string folderName;
                    if (ShowInputDialog("Create Folder", "New Folder", folderName))
                    {
                        CreateFolder(item.path, folderName, provider);
                        actionPerformed = true;
                    }
                }

                if (canModify && ImGui::MenuItem("Import Asset..."))
                {
                    // TODO: Show file dialog for import
                    actionPerformed = true;
                }

                ImGui::Separator();
            }

            if (!item.isParentDir)
            {
                if (canModify && ImGui::MenuItem("Rename"))
                {
                    std::string newName;
                    if (ShowInputDialog("Rename", item.name, newName))
                    {
                        RenameAsset(item.path, newName, provider);
                        actionPerformed = true;
                    }
                }

                if (canModify && ImGui::MenuItem("Delete"))
                {
                    const std::string message = "Are you sure you want to delete '" + item.name + "'?";
                    if (ShowConfirmationDialog(message))
                    {
                        DeleteAsset(item.path, provider);
                        actionPerformed = true;
                    }
                }

                if (!item.isDirectory)
                {
                    ImGui::Separator();

                    if (ImGui::MenuItem("Open in External Editor"))
                    {
                        // TODO: Open file in external editor
                        actionPerformed = true;
                    }

                    if (ImGui::MenuItem("Show in Explorer"))
                    {
                        // TODO: Show file in system explorer
                        actionPerformed = true;
                    }
                }
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Refresh"))
            {
                actionPerformed = true; // Signal refresh needed
            }

            ImGui::EndPopup();
        }

        return actionPerformed;
    }

    OperationResult AssetOperations::CreateFolder(const std::filesystem::path &parentDirectory,
                                                  const std::string &folderName,
                                                  std::shared_ptr<AssetProvider> provider)
    {
        if (!provider->CanModifyAssets())
        {
            return OperationResult::NotSupported;
        }

        try
        {
            const std::filesystem::path newFolderPath = parentDirectory / folderName;

            if (std::filesystem::exists(newFolderPath))
            {
                return OperationResult::Failed; // Folder already exists
            }

            std::filesystem::create_directory(newFolderPath);
            return OperationResult::Success;
        }
        catch (const std::filesystem::filesystem_error &)
        {
            return OperationResult::Failed;
        }
    }

    OperationResult AssetOperations::DeleteAsset(const std::filesystem::path &path,
                                                 std::shared_ptr<AssetProvider> provider)
    {
        if (!provider->CanModifyAssets())
        {
            return OperationResult::NotSupported;
        }

        try
        {
            if (std::filesystem::is_directory(path))
            {
                std::filesystem::remove_all(path);
            }
            else
            {
                std::filesystem::remove(path);
            }
            return OperationResult::Success;
        }
        catch (const std::filesystem::filesystem_error &)
        {
            return OperationResult::Failed;
        }
    }

    OperationResult AssetOperations::RenameAsset(const std::filesystem::path &oldPath,
                                                 const std::string &newName,
                                                 std::shared_ptr<AssetProvider> provider)
    {
        if (!provider->CanModifyAssets())
        {
            return OperationResult::NotSupported;
        }

        try
        {
            const std::filesystem::path newPath = oldPath.parent_path() / newName;

            if (std::filesystem::exists(newPath))
            {
                return OperationResult::Failed; // Name already exists
            }

            std::filesystem::rename(oldPath, newPath);
            return OperationResult::Success;
        }
        catch (const std::filesystem::filesystem_error &)
        {
            return OperationResult::Failed;
        }
    }

    OperationResult AssetOperations::ImportAsset(const std::filesystem::path &sourcePath,
                                                 const std::filesystem::path &targetDirectory,
                                                 std::shared_ptr<AssetProvider> provider)
    {
        if (!provider->CanModifyAssets())
        {
            return OperationResult::NotSupported;
        }

        try
        {
            const std::filesystem::path targetPath = targetDirectory / sourcePath.filename();

            if (std::filesystem::exists(targetPath))
            {
                return OperationResult::Failed; // File already exists
            }

            std::filesystem::copy_file(sourcePath, targetPath);
            return OperationResult::Success;
        }
        catch (const std::filesystem::filesystem_error &)
        {
            return OperationResult::Failed;
        }
    }

    bool AssetOperations::ShowConfirmationDialog(const std::string &message)
    {
        // Simple confirmation using ImGui modal
        // In a real implementation, you might want a more sophisticated dialog system
        static bool showDialog = false;
        static bool result = false;
        static std::string dialogMessage;

        if (!showDialog)
        {
            showDialog = true;
            dialogMessage = message;
            ImGui::OpenPopup("Confirm Action");
        }

        bool confirmed = false;
        if (ImGui::BeginPopupModal("Confirm Action", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("%s", dialogMessage.c_str());
            ImGui::Separator();

            if (ImGui::Button("Yes", ImVec2(120, 0)))
            {
                confirmed = true;
                result = true;
                showDialog = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("No", ImVec2(120, 0)))
            {
                result = false;
                showDialog = false;
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        return confirmed && result;
    }

    bool AssetOperations::ShowInputDialog(const std::string &title,
                                          const std::string &defaultValue,
                                          std::string &result)
    {
        // Simple input dialog using ImGui modal
        static bool showDialog = false;
        static bool confirmed = false;
        static char inputBuffer[256];
        static std::string dialogTitle;

        if (!showDialog)
        {
            showDialog = true;
            confirmed = false;
            dialogTitle = title;
            strncpy(inputBuffer, defaultValue.c_str(), sizeof(inputBuffer) - 1);
            inputBuffer[sizeof(inputBuffer) - 1] = '\0';
            ImGui::OpenPopup("Input Dialog");
        }

        bool hasResult = false;
        if (ImGui::BeginPopupModal("Input Dialog", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("%s", dialogTitle.c_str());
            ImGui::Separator();

            ImGui::InputText("##input", inputBuffer, sizeof(inputBuffer));

            if (ImGui::Button("OK", ImVec2(120, 0)))
            {
                result = inputBuffer;
                confirmed = true;
                hasResult = true;
                showDialog = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0)))
            {
                confirmed = false;
                showDialog = false;
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        return hasResult && confirmed;
    }
}
