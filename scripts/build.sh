#!/bin/bash
set -e

# === CONFIG ===
BUILD_TYPE=Release
BUILD_DIR=build

# Clean up any existing build directory if running in WSL
if grep -q Microsoft /proc/version 2>/dev/null; then
    echo "WSL environment detected, cleaning build directory..."
    rm -rf "$BUILD_DIR"
fi

# Detect available generator
if command -v ninja &> /dev/null; then
    GENERATOR="Ninja"
    echo "Using Ninja build system..."
elif command -v make &> /dev/null; then
    GENERATOR="Unix Makefiles"
    echo "Using Make build system..."
else
    echo "Neither Ninja nor Make build systems found. Installing required packages..."
    if command -v apt-get &> /dev/null; then
        # Debian/Ubuntu
        echo "Installing build tools and dependencies..."
        sudo apt-get update
        sudo apt-get install -y cmake build-essential ninja-build g++ xorg-dev libgl1-mesa-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev
        GENERATOR="Ninja"
    else
        echo "Unsupported package manager. Please install CMake, a C++ compiler, and Ninja or Make manually."
        exit 1
    fi
fi

# === GENERATE PROJECT FILES ===
echo "Configuring project with CMake..."

# Check for ccache to speed up compilation
if command -v ccache &> /dev/null; then
    echo "Using ccache for faster compilation..."
    CMAKE_CCACHE_FLAGS="-DCMAKE_CXX_COMPILER_LAUNCHER=ccache"
else
    CMAKE_CCACHE_FLAGS=""
fi

# Additional flags for WSL/Linux to handle compiler warnings
if grep -q Microsoft /proc/version 2>/dev/null || [ "$(uname)" == "Linux" ]; then
    echo "Linux/WSL environment detected, adding compiler flags for external dependencies..."
    cmake -B "$BUILD_DIR" -G "$GENERATOR" -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
        -DCMAKE_CXX_FLAGS="-Wno-error=array-bounds -Wno-error=stringop-overflow" \
        -DASSIMP_WARNINGS_AS_ERRORS=OFF \
        $CMAKE_CCACHE_FLAGS
else
    cmake -B "$BUILD_DIR" -G "$GENERATOR" -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
        $CMAKE_CCACHE_FLAGS
fi

if [ $? -ne 0 ]; then
    echo "Failed to configure the project."
    exit 1
fi

# === BUILD PROJECT ===
echo "Building project..."

# Determine number of parallel jobs
if [ "$(uname)" == "Darwin" ]; then
    JOBS=$(sysctl -n hw.ncpu)
elif command -v nproc &> /dev/null; then
    JOBS=$(nproc)
else
    JOBS=4  # fallback
fi

echo "Building with $JOBS parallel jobs..."
cmake --build "$BUILD_DIR" --config $BUILD_TYPE --parallel $JOBS

if [ $? -ne 0 ]; then
    echo "Build failed."
    exit 1
fi

echo "Build successful!"

# === GENERATE DOCUMENTATION ===
echo "Generating documentation..."
cmake --build "$BUILD_DIR" --target doc --config $BUILD_TYPE

# === RUN THE ENGINE ===
# Check if we're on macOS, Linux or WSL and adjust paths accordingly
if [ "$(uname)" == "Darwin" ]; then
    # macOS path
    EXE_PATH="$BUILD_DIR/Voltray"
    EXE_DIR="$BUILD_DIR"
elif grep -q Microsoft /proc/version 2>/dev/null; then
    # WSL path - need to use Windows path format for executable
    echo "WSL environment detected, won't attempt to run the executable directly."
    echo "Please run the executable from Windows environment."
    exit 0
else
    # Regular Linux path
    EXE_PATH="$BUILD_DIR/Voltray"
    EXE_DIR="$BUILD_DIR"
fi

if [ -z "$EXE_PATH" ]; then
    echo "Environment not supported for automatic execution."
    exit 0
elif [ -f "$EXE_PATH" ]; then
    echo "Running Voltray..."
    cd "$EXE_DIR"
    ./Voltray
    cd - > /dev/null
else
    echo "Error: Voltray executable not found at $EXE_PATH"
    exit 1
fi