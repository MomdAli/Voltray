#!/usr/bin/env bash
set -e

# === CONFIG ===
BUILD_TYPE=Release
BUILD_DIR=build
BUILD_DOC=voltray-docs

# === GENERATE PROJECT FILES ===
echo "Configuring project with CMake..."
cmake -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=$BUILD_TYPE

# === BUILD PROJECT ===
echo "Building project..."
cmake --build "$BUILD_DIR" --config $BUILD_TYPE --target $BUILD_DOC

echo "Build successful!"

# === RUN THE ENGINE ===
EXE_PATH="$BUILD_DIR/Engine/Voltray"
if [ -f "$EXE_PATH" ]; then
    echo "Running Voltray..."
    "$EXE_PATH"
else
    echo "Error: Voltray executable not found at $EXE_PATH"
    exit 1
fi
