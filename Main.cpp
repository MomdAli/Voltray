#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "EditorApp.h"
#include "ResourceManager.h"
#include "CrashLogger.h"
#include "UserDataManager.h"
#include "Workspace.h"

using namespace Voltray::Utils;
using Voltray::Editor::EditorApp;

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#include <limits.h>
#endif

int main()
{
#ifdef _WIN32
    SetUnhandledExceptionFilter([](PEXCEPTION_POINTERS exInfo) -> LONG
                                {
        CrashLogger::LogCrash(exInfo);
        return EXCEPTION_EXECUTE_HANDLER; });
#endif

    CrashLogger::Initialize();

    try
    {
        if (!glfwInit())
            return -1;        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        GLFWmonitor *monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *mode = glfwGetVideoMode(monitor);

        // Start in windowed mode with reasonable size
        glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE); // Start maximized instead of fake fullscreen

        GLFWwindow *window = glfwCreateWindow(1280, 720, "Voltray Editor", nullptr, nullptr);        if (!window)
        {
            glfwTerminate();
            return -1;
        }

        // Add F11 key callback to toggle fullscreen modes
        glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode, int action, int mods)                           {
            (void)scancode; // Suppress unused parameter warning
            (void)mods;     // Suppress unused parameter warning
            if (key == GLFW_KEY_F11 && action == GLFW_PRESS) {
                static bool isFullscreen = false; // Start in windowed mode
                GLFWmonitor* monitor = glfwGetPrimaryMonitor();
                const GLFWvidmode* mode = glfwGetVideoMode(monitor);

                if (isFullscreen) {
                    // Switch to windowed mode
                    glfwSetWindowMonitor(window, nullptr, 100, 100, 1280, 720, 0);
                } else {
                    // Switch to exclusive fullscreen
                    glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
                }
                isFullscreen = !isFullscreen;
            } });

        glfwMakeContextCurrent(window);

        if (!gladLoadGL(glfwGetProcAddress))
        {
            glfwDestroyWindow(window);
            glfwTerminate();
            return -1;
        } // Initialize UserDataManager before ResourceManager
        if (!UserDataManager::Initialize())
        {
            std::cerr << "Failed to initialize UserDataManager" << std::endl;
            glfwDestroyWindow(window);
            glfwTerminate();
            return -1;
        }

        // Initialize WorkspaceManager
        if (!WorkspaceManager::Initialize())
        {
            std::cerr << "Failed to initialize WorkspaceManager" << std::endl;
            glfwDestroyWindow(window);
            glfwTerminate();
            return -1;
        } // Initialize ResourceManager
#ifdef _WIN32
        char exePath[MAX_PATH];
        GetModuleFileNameA(nullptr, exePath, MAX_PATH);
        ResourceManager::Initialize(exePath);
#else
        char exePath[PATH_MAX];
        readlink("/proc/self/exe", exePath, PATH_MAX);
        ResourceManager::Initialize(exePath);
#endif

        // Initialize and run editor
        EditorApp editor;
        editor.Init(window);

        while (!glfwWindowShouldClose(window))
        {
            editor.RenderUI();
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        editor.Shutdown();
        glfwDestroyWindow(window);
        glfwTerminate();
    }
    catch (const std::exception &e)
    {
        CrashLogger::LogException(e.what());
    }
    catch (...)
    {
        CrashLogger::LogException("Unknown exception");
    }

    return 0;
}
