#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"
BUILD_TYPE="${1:-Debug}"
BUILD_DIR="${REPO_ROOT}/build"

if command -v ninja >/dev/null 2>&1; then
    GENERATOR="Ninja"
elif command -v make >/dev/null 2>&1; then
    GENERATOR="Unix Makefiles"
else
    echo "Neither Ninja nor Make was found. Install one of them first."
    exit 1
fi

echo "Configuring Voltray (${BUILD_TYPE})..."
cmake -S "${REPO_ROOT}" -B "${BUILD_DIR}" -G "${GENERATOR}" -DCMAKE_BUILD_TYPE="${BUILD_TYPE}" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DFETCHCONTENT_UPDATES_DISCONNECTED=ON

echo "Building Voltray (${BUILD_TYPE})..."
cmake --build "${BUILD_DIR}" --config "${BUILD_TYPE}" --parallel

echo "Build successful."
