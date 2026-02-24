@echo off
setlocal

REM === CONFIG ===
set BUILD_TYPE=Release
set GENERATOR="Visual Studio 17 2022"
set BUILD_DIR=build
set BUILD_DOC=voltray-docs

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


echo Generating documentation...
cmake --build %BUILD_DIR% --target doc --config %BUILD_TYPE%

REM === RUN THE ENGINE ===
set EXE_PATH=%BUILD_DIR%\%BUILD_TYPE%\Voltray.exe
set EXE_DIR=%BUILD_DIR%\%BUILD_TYPE%

if exist "%EXE_PATH%" (
    echo Running Voltray...
    pushd "%EXE_DIR%"
    Voltray.exe
    popd
) else (
    echo Error: Voltray.exe not found at %EXE_PATH%
)

endlocal
pause
