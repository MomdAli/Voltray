#!/usr/bin/env bash
set -e

# === CONFIG ===
BUILD_TYPE=Release
GENERATOR="Ninja"
BUILD_DIR=build
BUILD_DOC=voltray-docs

# === GENERATE PROJECT FILES ===
echo "Configuring project with CMake..."
cmake -B "$BUILD_DIR" -G $GENERATOR -DCMAKE_BUILD_TYPE=$BUILD_TYPE

# === BUILD PROJECT ===
echo "Building project..."
cmake --build "$BUILD_DIR" --config $BUILD_TYPE

if [ $? -ne 0 ]; then
    echo "Build failed."
    exit 1
fi

echo "Build successful!"

# === GENERATE DOCUMENTATION ===
if [ ! -d "$BUILD_DOC" ]; then
    mkdir "$BUILD_DOC"
fi
echo "Generating documentation..."
cmake --build "$BUILD_DIR" --target doc --config $BUILD_TYPE

# === RUN THE ENGINE ===
EXE_PATH="$BUILD_DIR/$BUILD_TYPE/Voltray"
if [ -f "$EXE_PATH" ]; then
    echo "Running Voltray..."
    "$EXE_PATH"
else
    echo "Error: Voltray executable not found at $EXE_PATH"
    exit 1
fi
