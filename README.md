# Voltray
A modern, cross-platform C++ real-time rendering engine with an integrated editor.

![Voltray Editor](screenshots/Screenshot-1.png)

<div align="center">

![Build Status](https://img.shields.io/badge/build-passing-brightgreen)
![C++](https://img.shields.io/badge/C%2B%2B-17-blue)
![OpenGL](https://img.shields.io/badge/OpenGL-4.6-red)
![License](https://img.shields.io/badge/license-Apache%202.0-green)
![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey)

</div>

---

## About
Voltray is a comprehensive 3D graphics engine designed for learning, experimentation, and educational purposes. It features a clean architecture, extensible design, and professional-grade development practices.

## Features
*   **Real-Time 3D Rendering**: OpenGL 4.6 pipeline
*   **Integrated Editor**: ImGui-based workspace
*   **Cross-Platform**: Windows, Linux, macOS Support

## Getting Started

### Prerequisites
*   **C++17 Compiler** (MSVC, GCC, Clang)
*   **CMake 3.17+**
*   **Git**

### Build Instructions

**Windows**:
```cmd
git clone https://github.com/MomdAli/Voltray.git
cd Voltray
build.bat
```

**Linux/macOS**:
```bash
git clone https://github.com/MomdAli/Voltray.git
cd Voltray
chmod +x build.sh
./build.sh
```

## Usage

### Running the Editor
After building the project, navigate to the `build` directory (or `build\Release` on Windows) and run the `Voltray` executable.

### Basic Controls
*   **Orbit Camera**: Middle Mouse Button + Drag
*   **Pan Camera**: Shift + Middle Mouse Button + Drag
*   **Zoom**: Mouse Wheel
*   **Select Object**: Left Click on object in Viewport
*   **Transform Object**: Use the **Inspector** panel to modify position, rotation, and scale.

## License
This project is licensed under the **Apache License 2.0**. See the [LICENSE](LICENSE) file for details.

---

<div align="center">

**Built with ❤️ for the graphics programming community**

[⭐ Star this project](https://github.com/MomdAli/Voltray) • [🐛 Report Bug](https://github.com/MomdAli/Voltray/issues)

</div>
