@echo off
setlocal

REM === CONFIG ===
set BUILD_TYPE=Release
set GENERATOR="Visual Studio 17 2022"
set BUILD_DIR=build

REM === GENERATE PROJECT FILES ===
echo Configuring project with CMake...
cmake -B %BUILD_DIR% -G %GENERATOR% -A x64 -DCMAKE_BUILD_TYPE=%BUILD_TYPE%

if errorlevel 1 (
    echo Failed to configure the project.
    exit /b 1
)

REM === BUILD PROJECT ===
echo Building project...
cmake --build %BUILD_DIR% --config %BUILD_TYPE%

if errorlevel 1 (
    echo Build failed.
    exit /b 1
)

echo Build successful!

REM === RUN THE ENGINE ===
set EXE_PATH=%BUILD_DIR%\Engine\%BUILD_TYPE%\Voltray.exe

if exist "%EXE_PATH%" (
    echo Running Voltray...
    "%EXE_PATH%"
) else (
    echo Error: Voltray.exe not found at %EXE_PATH%
)

endlocal
pause
