@echo off
REM Voltray Docker Build Script for Windows
REM This script builds Voltray for multiple platforms using Docker

setlocal enabledelayedexpansion

echo === Voltray Multi-Platform Docker Build ===
echo.

REM Build configuration
if not defined BUILD_TYPE set BUILD_TYPE=Release
if not defined PARALLEL_JOBS set PARALLEL_JOBS=4

echo Build configuration:
echo   Build Type: %BUILD_TYPE%
echo   Parallel Jobs: %PARALLEL_JOBS%
echo.

REM Create output directory
if not exist artifacts mkdir artifacts

REM Function to build for a specific platform
:build_platform
set "platform=%~1"
set "dockerfile=%~2"
set "output_name=%~3"

echo [INFO] Building for %platform%...

REM Build the Docker image
docker build --target builder -f "%dockerfile%" -t "voltray-%platform%:latest" --build-arg BUILD_TYPE="%BUILD_TYPE%" .
if errorlevel 1 (
    echo [ERROR] Failed to build Docker image for %platform%
    exit /b 1
)

REM Create a container to extract the binary
for /f %%i in ('docker create "voltray-%platform%:latest"') do set container_id=%%i

REM Extract the built binary
docker cp "%container_id%:/workspace/build/Voltray" "artifacts/%output_name%"
if errorlevel 1 (
    echo [WARNING] Failed to extract binary for %platform%
)

REM Extract shaders
docker cp "%container_id%:/workspace/Shaders" "artifacts/Shaders-%platform%" 2>nul

REM Clean up container
docker rm "%container_id%" >nul

echo [SUCCESS] %platform% build completed
goto :eof

REM Main build process
if "%1"=="linux" goto build_linux
if "%1"=="" goto build_all

:build_linux
call :build_platform ubuntu Dockerfile Voltray-linux.exe
goto package

:build_all
call :build_platform ubuntu Dockerfile Voltray-linux.exe

:package
if "%1" NEQ "" goto end

echo [INFO] Creating packages...
cd artifacts

REM Ubuntu package
if exist "Voltray-linux.exe" (
    if not exist voltray-linux mkdir voltray-linux
    copy "Voltray-linux.exe" "voltray-linux\voltray.exe" >nul
    if exist "Shaders-ubuntu" xcopy /E /I "Shaders-ubuntu" "voltray-linux\Shaders" >nul
    powershell -command "Compress-Archive -Path 'voltray-linux\*' -DestinationPath 'voltray-linux.zip' -Force"
    rmdir /s /q voltray-linux
    echo [SUCCESS] Created voltray-linux.zip
)

cd ..

:end
echo [SUCCESS] All builds completed successfully!
echo [INFO] Artifacts available in the 'artifacts' directory
echo.

echo Built artifacts:
dir artifacts

pause
