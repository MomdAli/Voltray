#pragma once

#include <vector>
#include <string>
#include <functional>
#include "../../Utils/Workspace.h"

namespace Editor::UI
{
    /**
     * @brief Modal dialog for workspace selection and creation
     *
     * This dialog appears on application startup and allows users to:
     * - Select from existing workspaces
     * - Create new workspaces
     * - Browse for workspace directories
     * - Remove invalid workspaces from the list
     */
    class WorkspaceDialog
    {
    public:
        /**
         * @brief Callback function type for workspace selection
         * @param workspace Pointer to selected workspace, nullptr if cancelled
         */
        using WorkspaceCallback = std::function<void(const Workspace *)>;

        /**
         * @brief Show the workspace selection dialog
         * @param callback Function to call when workspace is selected or dialog is cancelled
         */
        static void Show(WorkspaceCallback callback);

        /**
         * @brief Render the dialog (call this in your main render loop)
         * @return true if dialog is still open, false if closed
         */
        static bool Render();

        /**
         * @brief Check if the dialog is currently open
         * @return true if dialog is open
         */
        static bool IsOpen();

        /**
         * @brief Force close the dialog
         */
        static void Close();

    private:
        static bool s_IsOpen;
        static WorkspaceCallback s_Callback;
        static std::vector<Workspace> s_Workspaces;
        static int s_SelectedWorkspace;
        static bool s_ShowCreateDialog;

        // Create workspace dialog state
        static char s_NewWorkspaceName[256];
        static char s_NewWorkspaceDescription[512];
        static char s_NewWorkspacePath[1024];
        static bool s_CreateInProgress;

        static void RenderWorkspaceList();
        static void RenderCreateDialog();
        static void RefreshWorkspaceList();
        static void SelectWorkspaceFolder();
        static void CreateWorkspace();
        static void OnWorkspaceSelected(const Workspace *workspace);
        static const char *GetRelativeTimeString(const std::chrono::system_clock::time_point &timePoint);
    };
}
