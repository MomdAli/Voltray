#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "Editor/EditorApp.h"
#include <fstream>
#include <filesystem>
#include <iostream>
#include <string>

#ifdef _WIN32
#include <Windows.h>
#include <crtdbg.h>
#endif

// Cross-platform error logging
void LogError(const std::string &message)
{
    std::filesystem::path logDir = std::filesystem::current_path() / "logs";
    if (!std::filesystem::exists(logDir))
        std::filesystem::create_directory(logDir);
    auto logFile = logDir / "error.log";

    std::ofstream ofs(logFile.string(), std::ios::app);
    ofs << message << std::endl;
    ofs.close();

    std::cerr << message << std::endl;
}

// Cross-platform message display
void ShowErrorMessage(const std::string &message, const std::string &title = "Voltray Error")
{
#ifdef _WIN32
    MessageBoxA(nullptr, message.c_str(), title.c_str(), MB_OK | MB_ICONERROR);
#else
    // On Linux/macOS, just print to stderr
    std::cerr << title << ": " << message << std::endl;
#endif
}

#ifdef _WIN32
// Windows-specific crash handler
LONG WINAPI TopLevelExceptionHandler(PEXCEPTION_POINTERS exInfo)
{
    std::string errorMsg = "Crash: Exception code 0x" +
                           std::to_string(exInfo->ExceptionRecord->ExceptionCode) +
                           " at address " +
                           std::to_string((uintptr_t)exInfo->ExceptionRecord->ExceptionAddress);

    LogError(errorMsg);
    ShowErrorMessage(errorMsg, "Voltray Crash Detected");

    return EXCEPTION_EXECUTE_HANDLER;
}
#endif

int main()
{
#ifdef _WIN32
    SetUnhandledExceptionFilter(TopLevelExceptionHandler);
// Enable debug runtime error reporting
#ifdef _MSC_VER
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
#endif
#endif

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

        // Initialize glad
        if (!gladLoadGL(glfwGetProcAddress))
        {
            glfwDestroyWindow(window);
            glfwTerminate();
            return -1;
        }

        // Editor setup
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
        ShowErrorMessage(e.what(), "Exception Caught");
    }
    catch (...)
    {
        ShowErrorMessage("Unknown exception caught", "Exception Caught");
    }
    return 0;
}
