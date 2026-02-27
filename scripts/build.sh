#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"
BUILD_TYPE="${1:-Debug}"
BUILD_DIR="${REPO_ROOT}/build"
FETCHCONTENT_FLAG="-DFETCHCONTENT_UPDATES_DISCONNECTED=ON"

if [ -d "${BUILD_DIR}/_deps/glfw-src" ] && [ -d "${BUILD_DIR}/_deps/assimp-src" ] && [ -d "${BUILD_DIR}/_deps/imgui-src" ]; then
    FETCHCONTENT_FLAG="-DFETCHCONTENT_FULLY_DISCONNECTED=ON"
fi

if command -v ninja >/dev/null 2>&1; then
    GENERATOR="Ninja"
elif command -v make >/dev/null 2>&1; then
    GENERATOR="Unix Makefiles"
else
    echo "Neither Ninja nor Make was found. Install one of them first."
    exit 1
fi

echo "Configuring Voltray (${BUILD_TYPE})..."
cmake -S "${REPO_ROOT}" -B "${BUILD_DIR}" -G "${GENERATOR}" -DCMAKE_BUILD_TYPE="${BUILD_TYPE}" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON "${FETCHCONTENT_FLAG}"

echo "Building Voltray (${BUILD_TYPE})..."
cmake --build "${BUILD_DIR}" --config "${BUILD_TYPE}" --parallel

echo "Build successful."