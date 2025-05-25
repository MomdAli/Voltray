# Voltray

A modern, cross-platform C++ real-time rendering engine. Voltray is a long-term hobby and learning project focused on clean architecture, extensibility, and educational value.

---

## Features

- **Cross-Platform**: Runs on Windows, Linux, and macOS.
- **Real-Time 3D Graphics**: Fast, interactive rendering using OpenGL.
- **Easy to Build**: Simple setup with CMake and clear instructions.
- **Modular Design**: Clean separation between engine, editor, and game code.
- **Live Editor (WIP)**: In-development editor for creating and tweaking scenes.
- **Sample Game**: Example project to help you get started.
- **Automatic Documentation**: Up-to-date API docs generated with Doxygen.
- **Consistent Code Style**: Code formatting enforced automatically.
- **Open Source**: Free to use, learn from, and contribute to.

---

## Project Structure
- `Engine/` – Core engine source code
- `Editor/` – Editor tools (WIP)
- `Game/` – Example game logic (WIP)
- `Shaders/` – GLSL shaders
- `Vendor/` – Third-party libraries (managed via CMake FetchContent or vendored)
- `docs/` – Auto-generated API documentation (see below)

---

## Getting Started

### Prerequisites
- C++17 compiler (MSVC, GCC, or Clang)
- [CMake](https://cmake.org/) 3.17+
- [Doxygen](https://www.doxygen.nl/) (for documentation, optional)

### Build Instructions
1. **Clone the repository:**
    ```sh
    git clone https://github.com/MomdAli/Voltray.git
    cd Voltray
    ```
2. **Build the project:**
    - On Windows: Run `build.bat`
    - On Linux/macOS: Run `./build.sh`
3. **Run the engine:**
    - The executable will be in `build/Engine/Release/Voltray.exe` (Windows) or `build/Engine/Voltray` (Linux/macOS).

### Documentation
- API docs are auto-generated with Doxygen and available in the [`docs/`](docs/) folder.
- To generate locally:
  ```sh
  cmake --build . --target voltray-docs
  # or
  doxygen Doxyfile
  ```
- View the docs [Here](https://MomdAli.github.io/Voltray/).

---

## Contributing
Contributions, bug reports, and suggestions are welcome! Please open an issue or pull request.

---

## License
This project is licensed under the Apache License 2.0. See the [LICENSE](LICENSE) file for details.
