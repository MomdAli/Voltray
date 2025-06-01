#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "Editor/EditorApp.h"
#include "Utils/ResourceManager.h"
#include "Utils/CrashLogger.h"
#include "Utils/UserDataManager.h"
#include "Utils/Workspace.h"

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
            return -1;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        GLFWmonitor *monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *mode = glfwGetVideoMode(monitor);

        // Configure for windowed fullscreen (borderless windowed)
        glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        GLFWwindow *window = glfwCreateWindow(mode->width, mode->height, "Voltray Editor", nullptr, nullptr);
        if (!window)
        {
            glfwTerminate();
            return -1;
        }

        // Set window position to top-left corner
        glfwSetWindowPos(window, 0, 0); // Optional: Add F11 key callback to toggle fullscreen modes
        glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode, int action, int mods)
                           {
            (void)scancode; // Suppress unused parameter warning
            (void)mods;     // Suppress unused parameter warning
            if (key == GLFW_KEY_F11 && action == GLFW_PRESS) {
                static bool isFullscreen = true;
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
        }

        // Initialize ResourceManager
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
        Editor::EditorApp editor;
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
