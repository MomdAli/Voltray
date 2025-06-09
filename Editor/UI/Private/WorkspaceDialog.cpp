#include "WorkspaceDialog.h"
#include "UserDataManager.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <iostream>
#include <cstring>
#include <filesystem>

#ifdef _WIN32
#include <windows.h>
#include <commdlg.h>
#include <shlobj.h>
#endif

namespace Voltray::Editor::UI
{ // Static member definitions
    bool WorkspaceDialog::s_IsOpen = false;
    WorkspaceDialog::WorkspaceCallback WorkspaceDialog::s_Callback = nullptr;
    std::vector<Voltray::Utils::Workspace> WorkspaceDialog::s_Workspaces;
    int WorkspaceDialog::s_SelectedWorkspace = -1;
    bool WorkspaceDialog::s_ShowCreateDialog = false;
    char WorkspaceDialog::s_NewWorkspaceName[256] = "";
    char WorkspaceDialog::s_NewWorkspaceDescription[512] = "";
    char WorkspaceDialog::s_NewWorkspacePath[1024] = "";
    bool WorkspaceDialog::s_CreateInProgress = false;
    static int s_PendingSelection = -1;
    void WorkspaceDialog::Show(WorkspaceCallback callback)
    {
        s_IsOpen = true;
        s_Callback = callback;
        s_ShowCreateDialog = false;
        s_SelectedWorkspace = -1;
        s_CreateInProgress = false;
        s_PendingSelection = -1;

        // Clear create dialog fields
        memset(s_NewWorkspaceName, 0, sizeof(s_NewWorkspaceName));
        memset(s_NewWorkspaceDescription, 0, sizeof(s_NewWorkspaceDescription));
        memset(s_NewWorkspacePath, 0, sizeof(s_NewWorkspacePath));

        RefreshWorkspaceList();
    }
    bool WorkspaceDialog::Render()
    {
        if (!s_IsOpen)
            return false;

        // Open the popup modal if it's not already open
        if (!ImGui::IsPopupOpen("Workspace Manager"))
        {
            ImGui::OpenPopup("Workspace Manager");
        }

        // Center the dialog on screen
        ImGuiIO &io = ImGui::GetIO();
        ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);

        // Make dialog modal
        bool modalOpen = true;
        if (ImGui::BeginPopupModal("Workspace Manager", &modalOpen, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
        {
            ImGui::Text("Welcome to Voltray Editor!");
            ImGui::Text("Please select a workspace to continue:");
            ImGui::Separator();

            if (s_ShowCreateDialog)
            {
                RenderCreateDialog();
            }
            else
            {
                RenderWorkspaceList();
            }

            ImGui::EndPopup();
        }

        // Handle pending selection (deferred from double-click)
        if (s_PendingSelection >= 0 && s_PendingSelection < static_cast<int>(s_Workspaces.size()))
        {
            OnWorkspaceSelected(&s_Workspaces[s_PendingSelection]);
            s_PendingSelection = -1;
            return s_IsOpen;
        }

        // Handle dialog close only when user explicitly closes it
        if (!modalOpen)
        {
            s_IsOpen = false;
            if (s_Callback)
            {
                OnWorkspaceSelected(nullptr); // User cancelled
            }
        }

        return s_IsOpen;
    }

    void WorkspaceDialog::RenderWorkspaceList()
    {
        // Workspace list
        ImGui::BeginChild("WorkspaceList", ImVec2(0, -50), true);

        if (s_Workspaces.empty())
        {
            ImGui::TextWrapped("No workspaces found. Create your first workspace to get started!");
        }
        else
        {
            ImGui::Text("Recent Workspaces:");
            ImGui::Separator();
            for (int i = 0; i < static_cast<int>(s_Workspaces.size()); ++i)
            {
                const auto &workspace = s_Workspaces[i];

                ImGui::PushID(i);

                // Workspace item
                bool isSelected = (s_SelectedWorkspace == i);
                if (ImGui::Selectable("##workspace", isSelected, ImGuiSelectableFlags_AllowDoubleClick))
                {
                    s_SelectedWorkspace = i;
                    // Double-click to select
                    if (ImGui::IsMouseDoubleClicked(0))
                    {
                        s_PendingSelection = i;
                        ImGui::PopID();    // Clean up ID before early return
                        ImGui::EndChild(); // End the child window before early return
                        return;
                    }
                }

                // Show workspace info
                ImGui::SameLine();
                ImGui::BeginGroup();

                // Name and description
                ImGui::Text("%s", workspace.name.c_str());
                if (!workspace.description.empty())
                {
                    ImGui::SameLine();
                    ImGui::TextDisabled("- %s", workspace.description.c_str());
                }

                // Path and last opened time
                ImGui::TextDisabled("Path: %s", workspace.path.string().c_str());
                ImGui::SameLine();
                ImGui::TextDisabled("| Last opened: %s", GetRelativeTimeString(workspace.lastOpened));

                // Validation status
                if (!workspace.IsPathValid())
                {
                    ImGui::SameLine();
                    ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "(Invalid - path not found)");
                }

                ImGui::EndGroup();
                ImGui::PopID();
            }
        }

        ImGui::EndChild();

        // Bottom buttons
        ImGui::Separator();

        // Left side buttons
        if (ImGui::Button("Create New Workspace"))
        {
            s_ShowCreateDialog = true;

// Set default path to user's Documents folder
#ifdef _WIN32
            wchar_t *documentsPath = nullptr;
            if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Documents, 0, nullptr, &documentsPath)))
            {
                std::filesystem::path defaultPath = std::filesystem::path(documentsPath) / "VoltrayProjects";
                std::string pathStr = defaultPath.string();
                strncpy(s_NewWorkspacePath, pathStr.c_str(), sizeof(s_NewWorkspacePath) - 1);
                s_NewWorkspacePath[sizeof(s_NewWorkspacePath) - 1] = '\0';
                CoTaskMemFree(documentsPath);
            }
#endif
        }

        ImGui::SameLine();
        if (ImGui::Button("Browse Folder"))
        {
            SelectWorkspaceFolder();
        }

        ImGui::SameLine();
        if (ImGui::Button("Refresh"))
        {
            RefreshWorkspaceList();
        }

        // Right side buttons
        ImGui::SameLine(ImGui::GetWindowWidth() - 200);
        if (ImGui::Button("Cancel"))
        {
            Close();
        }

        ImGui::SameLine();
        ImGui::BeginDisabled(s_SelectedWorkspace < 0 || s_SelectedWorkspace >= static_cast<int>(s_Workspaces.size()));
        if (ImGui::Button("Open Workspace"))
        {
            if (s_SelectedWorkspace >= 0 && s_SelectedWorkspace < static_cast<int>(s_Workspaces.size()))
            {
                s_PendingSelection = s_SelectedWorkspace;
            }
        }
        ImGui::EndDisabled();
    }

    void WorkspaceDialog::RenderCreateDialog()
    {
        ImGui::Text("Create New Workspace");
        ImGui::Separator();

        // Input fields
        ImGui::Text("Workspace Name:");
        ImGui::InputText("##name", s_NewWorkspaceName, sizeof(s_NewWorkspaceName));

        ImGui::Text("Description (optional):");
        ImGui::InputTextMultiline("##description", s_NewWorkspaceDescription, sizeof(s_NewWorkspaceDescription), ImVec2(0, 60));

        ImGui::Text("Location:");
        ImGui::InputText("##path", s_NewWorkspacePath, sizeof(s_NewWorkspacePath));
        ImGui::SameLine();
        if (ImGui::Button("Browse..."))
        {
            SelectWorkspaceFolder();
        }

        // Preview full path
        if (strlen(s_NewWorkspaceName) > 0 && strlen(s_NewWorkspacePath) > 0)
        {
            std::filesystem::path fullPath = std::filesystem::path(s_NewWorkspacePath) / s_NewWorkspaceName;
            ImGui::TextDisabled("Full path: %s", fullPath.string().c_str());
        }

        ImGui::Separator();

        // Buttons
        if (ImGui::Button("Back"))
        {
            s_ShowCreateDialog = false;
        }

        ImGui::SameLine();
        ImGui::BeginDisabled(strlen(s_NewWorkspaceName) == 0 || strlen(s_NewWorkspacePath) == 0 || s_CreateInProgress);
        if (ImGui::Button("Create Workspace"))
        {
            CreateWorkspace();
        }
        ImGui::EndDisabled();

        if (s_CreateInProgress)
        {
            ImGui::SameLine();
            ImGui::Text("Creating workspace...");
        }
    }

    void WorkspaceDialog::RefreshWorkspaceList()
    {
        s_Workspaces = Voltray::Utils::WorkspaceManager::GetAllWorkspaces();

        // Sort by last opened time (most recent first)
        std::sort(s_Workspaces.begin(), s_Workspaces.end(),
                  [](const Voltray::Utils::Workspace &a, const Voltray::Utils::Workspace &b)
                  {
                      return a.lastOpened > b.lastOpened;
                  });

        s_SelectedWorkspace = -1;
    }

    void WorkspaceDialog::SelectWorkspaceFolder()
    {
#ifdef _WIN32
        BROWSEINFOW bi = {};
        bi.lpszTitle = L"Select Workspace Directory";
        bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;

        LPITEMIDLIST pidl = SHBrowseForFolderW(&bi);
        if (pidl)
        {
            wchar_t path[MAX_PATH];
            if (SHGetPathFromIDListW(pidl, path))
            {
                std::filesystem::path selectedPath(path);
                std::string pathStr = selectedPath.string();
                strncpy(s_NewWorkspacePath, pathStr.c_str(), sizeof(s_NewWorkspacePath) - 1);
                s_NewWorkspacePath[sizeof(s_NewWorkspacePath) - 1] = '\0';
            }
            CoTaskMemFree(pidl);
        }
#endif
    }

    void WorkspaceDialog::CreateWorkspace()
    {
        s_CreateInProgress = true;

        std::filesystem::path fullPath = std::filesystem::path(s_NewWorkspacePath) / s_NewWorkspaceName;

        if (Voltray::Utils::WorkspaceManager::CreateWorkspace(s_NewWorkspaceName, fullPath, s_NewWorkspaceDescription))
        {
            std::cout << "Successfully created workspace: " << s_NewWorkspaceName << std::endl;

            // Refresh the list and select the new workspace
            RefreshWorkspaceList(); // Find and select the newly created workspace
            for (int i = 0; i < static_cast<int>(s_Workspaces.size()); ++i)
            {
                if (s_Workspaces[i].path == fullPath)
                {
                    s_PendingSelection = i;
                    return;
                }
            }
        }
        else
        {
            std::cerr << "Failed to create workspace: " << s_NewWorkspaceName << std::endl;
        }

        s_CreateInProgress = false;
    }

    void WorkspaceDialog::OnWorkspaceSelected(const Voltray::Utils::Workspace *workspace)
    {
        if (s_Callback)
        {
            s_Callback(workspace);
            s_Callback = nullptr; // Clear callback to prevent multiple calls
        }
        Close();
    }

    const char *WorkspaceDialog::GetRelativeTimeString(const std::chrono::system_clock::time_point &timePoint)
    {
        static char buffer[64];

        auto now = std::chrono::system_clock::now();
        auto duration = now - timePoint;

        auto hours = std::chrono::duration_cast<std::chrono::hours>(duration).count();
        auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration).count();
        auto days = hours / 24;
        if (days > 0)
        {
            snprintf(buffer, sizeof(buffer), "%ld day%s ago", days, (days == 1) ? "" : "s");
        }
        else if (hours > 0)
        {
            snprintf(buffer, sizeof(buffer), "%ld hour%s ago", hours, (hours == 1) ? "" : "s");
        }
        else if (minutes > 0)
        {
            snprintf(buffer, sizeof(buffer), "%ld minute%s ago", minutes, (minutes == 1) ? "" : "s");
        }
        else
        {
            snprintf(buffer, sizeof(buffer), "Just now");
        }

        return buffer;
    }

    bool WorkspaceDialog::IsOpen()
    {
        return s_IsOpen;
    }

    void WorkspaceDialog::Close()
    {
        s_IsOpen = false;
    }
}
