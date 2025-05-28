#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "Editor/EditorApp.h"
#include "Utils/ResourceManager.h"
#include "Utils/CrashLogger.h"

#ifdef _WIN32
#include <Windows.h>
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

        GLFWwindow *window = glfwCreateWindow(1280, 720, "Voltray", nullptr, nullptr);
        if (!window)
        {
            glfwTerminate();
            return -1;
        }

        glfwMakeContextCurrent(window);

        if (!gladLoadGL(glfwGetProcAddress))
        {
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
