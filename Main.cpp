#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "Editor/EditorApp.h"
#include <Windows.h>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <string>

// Crash handler: logs exception info to logs/crash.log and shows a message box
LONG WINAPI TopLevelExceptionHandler(PEXCEPTION_POINTERS exInfo)
{
    std::filesystem::path logDir = std::filesystem::current_path() / "logs";
    if (!std::filesystem::exists(logDir))
        std::filesystem::create_directory(logDir);
    auto logFile = logDir / "crash.log";

    std::string errorMsg = "Crash: Exception code 0x" + std::to_string(exInfo->ExceptionRecord->ExceptionCode) +
                           " at address " + std::to_string((uintptr_t)exInfo->ExceptionRecord->ExceptionAddress);

    std::ofstream ofs(logFile.string(), std::ios::app);
    ofs << errorMsg << std::endl;
    ofs.close();

    // Show error message box
    MessageBoxA(nullptr, errorMsg.c_str(), "Voltray Crash Detected", MB_OK | MB_ICONERROR);

    std::cerr << errorMsg << std::endl;
    return EXCEPTION_EXECUTE_HANDLER;
}

int main()
{
    SetUnhandledExceptionFilter(TopLevelExceptionHandler);
// Enable debug runtime error reporting
#ifdef _MSC_VER
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
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
        MessageBoxA(nullptr, e.what(), "Exception Caught", MB_OK | MB_ICONERROR);
    }
    catch (...)
    {
        MessageBoxA(nullptr, "Unknown exception caught", "Exception Caught", MB_OK | MB_ICONERROR);
    }
    return 0;
}
