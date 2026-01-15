#!/bin/bash

# Stop the script if any command fails
set -euo pipefail

BUILD_DIR="build"
JOBS="${JOBS:-$(nproc)}"

if [ -d "$BUILD_DIR" ]; then
    echo "Removing existing build directory..."
    rm -rf "$BUILD_DIR"
fi

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

OPT_FLAGS="-O3 -ffast-math -ffunction-sections -fdata-sections -fvisibility=hidden -flto=auto -fno-fat-lto-objects"

cmake -G Ninja -S ../src/ -B . \
    -DCMAKE_C_FLAGS="$OPT_FLAGS" \
    -DCMAKE_CXX_FLAGS="$OPT_FLAGS" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_PREFIX_PATH="$HOME/dev/deps/" \
    -DCMAKE_INTERPROCEDURAL_OPTIMIZATION=ON \
    -DCMAKE_EXE_LINKER_FLAGS="-Wl,--gc-sections"

ninja -j "$JOBS"

cd ..