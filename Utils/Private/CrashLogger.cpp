#include "CrashLogger.h"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <fstream>

#ifdef _WIN32
#include <Windows.h>
#endif

namespace Voltray::Utils
{

    bool CrashLogger::s_Initialized = false;
    std::string CrashLogger::s_LogFile = "";

    void CrashLogger::Initialize()
    {
        s_Initialized = true;
    }

    void CrashLogger::LogException(const std::string &message)
    {
        CreateLogFileIfNeeded();
        WriteLog("EXCEPTION: " + message);

        // Also show message box on Windows
#ifdef _WIN32
        MessageBoxA(nullptr, message.c_str(), "Voltray Exception", MB_OK | MB_ICONERROR);
#endif
    }

#ifdef _WIN32
    void CrashLogger::LogCrash(PEXCEPTION_POINTERS exInfo)
    {
        if (!exInfo || !exInfo->ExceptionRecord)
            return;

        CreateLogFileIfNeeded();

        std::stringstream ss;
        ss << "CRASH: Exception code 0x" << std::hex << exInfo->ExceptionRecord->ExceptionCode
           << " at address 0x" << std::hex << (uintptr_t)exInfo->ExceptionRecord->ExceptionAddress;

        WriteLog(ss.str());

        // Show message box
        MessageBoxA(nullptr, ss.str().c_str(), "Voltray Crash Detected", MB_OK | MB_ICONERROR);
    }
#endif

    void CrashLogger::CreateLogFileIfNeeded()
    {
        if (!s_LogFile.empty())
            return; // Log file already created

        const std::string logDir = GetLogDirectory();

        // Create logs directory if it doesn't exist
        std::filesystem::create_directories(logDir);

        // Create timestamped log file
        const std::string timestamp = GetTimestamp();
        s_LogFile = logDir + "/crash_log_" + timestamp + ".txt";

        // Write header
        std::ofstream log(s_LogFile);
        if (log.is_open())
        {
            log << "Voltray Crash Log - " << timestamp << std::endl;
            log << "=======================================" << std::endl;
            log << "Working directory: " << std::filesystem::current_path().string() << std::endl;
            log << std::endl;
            log.close();
        }
    }

    std::string CrashLogger::GetLogDirectory()
    {
        return (std::filesystem::current_path() / "logs").string();
    }

    std::string CrashLogger::GetTimestamp()
    {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);

        std::tm tm = {};
#ifdef _WIN32
        localtime_s(&tm, &time_t);
#else
        localtime_r(&time_t, &tm);
#endif

        std::stringstream ss;
        ss << std::put_time(&tm, "%Y%m%d_%H%M%S");
        return ss.str();
    }

    void CrashLogger::WriteLog(const std::string &message)
    {
        if (!s_Initialized)
            Initialize();

        std::ofstream log(s_LogFile, std::ios::app);
        if (log.is_open())
        {
            auto now = std::chrono::system_clock::now();
            auto time_t = std::chrono::system_clock::to_time_t(now);

            std::tm tm = {};
#ifdef _WIN32
            localtime_s(&tm, &time_t);
#else
            localtime_r(&time_t, &tm);
#endif

            log << "[" << std::put_time(&tm, "%H:%M:%S") << "] " << message << std::endl;
            log.close();
        } // Also output to console for debugging
        std::cerr << message << std::endl;
    }

}
