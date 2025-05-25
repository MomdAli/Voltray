# Cross-Platform Build Guide for Voltray

This guide outlines how to build Voltray on different operating systems.

## Prerequisites

### Windows
- Visual Studio 2022 or later
- CMake 3.17+
- Git

### macOS
- Xcode Command Line Tools
- CMake 3.17+
- Homebrew (recommended for installing dependencies)

### Linux
- GCC 9+ or Clang 10+
- CMake 3.17+
- X11 development packages
- OpenGL development packages

## Building

### Windows

1. Clone the repository:
   ```
   git clone https://github.com/yourusername/Voltray.git
   cd Voltray
   ```

2. Run the build script:
   ```
   build.bat
   ```

3. The executable will be in the `build\Release` directory.

### macOS

1. Install dependencies:
   ```
   brew install cmake glfw
   ```

2. Clone the repository:
   ```
   git clone https://github.com/yourusername/Voltray.git
   cd Voltray
   ```

3. Make the build script executable and run it:
   ```
   chmod +x build.sh
   ./build.sh
   ```

4. The executable will be in the `build` directory.

### Linux

1. Install dependencies:
   ```
   sudo apt-get update
   sudo apt-get install cmake build-essential xorg-dev libgl1-mesa-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev
   ```

2. Clone the repository:
   ```
   git clone https://github.com/yourusername/Voltray.git
   cd Voltray
   ```

3. Make the build script executable and run it:
   ```
   chmod +x build.sh
   ./build.sh
   ```

4. The executable will be in the `build` directory.

## CI/CD

This project includes GitHub Actions CI/CD workflow that builds on Windows, macOS, and Linux. See `.github/workflows/build.yml` for details.

## Troubleshooting

### Windows
- If you get "missing DLL" errors, ensure that you copied the necessary DLLs to the executable directory.

### macOS
- If you encounter "Library not loaded" errors, ensure that you have the correct versions of GLFW installed.

### Linux
- If you get OpenGL or X11 errors, make sure all development packages are properly installed.
