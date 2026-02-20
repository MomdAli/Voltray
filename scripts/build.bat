@echo off
setlocal

set BUILD_TYPE=%~1
if "%BUILD_TYPE%"=="" set BUILD_TYPE=Debug

set SCRIPT_DIR=%~dp0
for %%I in ("%SCRIPT_DIR%..") do set REPO_ROOT=%%~fI
set BUILD_DIR=%REPO_ROOT%\build
set GENERATOR=Visual Studio 17 2022

echo Configuring Voltray %BUILD_TYPE%...
cmake -S "%REPO_ROOT%" -B "%BUILD_DIR%" -G "%GENERATOR%" -A x64 -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DFETCHCONTENT_UPDATES_DISCONNECTED=ON
if errorlevel 1 (
    echo Failed to configure the project.
    exit /b 1
)

echo Building Voltray %BUILD_TYPE%...
cmake --build "%BUILD_DIR%" --config %BUILD_TYPE%
if errorlevel 1 (
    echo Build failed.
    exit /b 1
)

echo Build successful.
endlocal
