#pragma once

#include <string>
#include <fstream>
#include <filesystem>

#ifdef _WIN32
#include <Windows.h>
#endif

namespace Voltray::Utils
{

    class CrashLogger
    {
    public:
        static void Initialize();
        static void LogException(const std::string &message);

#ifdef _WIN32
        static void LogCrash(PEXCEPTION_POINTERS exInfo);
#endif

    private:
        static std::string GetLogDirectory();
        static std::string GetTimestamp();
        static void CreateLogFileIfNeeded();
        static void WriteLog(const std::string &message);

        static bool s_Initialized;
        static std::string s_LogFile;
    };

}
