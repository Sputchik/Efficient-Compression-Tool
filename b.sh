#!/bin/bash

# Stop the script if any command fails
set -e

# Define the build directory
BUILD_DIR="build"

# Clean up previous build if it exists
if [ -d "$BUILD_DIR" ]; then
    echo "Removing existing build directory..."
    rm -rf "$BUILD_DIR"
fi

# Create directory and enter it
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Define Compiler Flags (GCC/Clang equivalents)
# -O3: Maximum optimization (covers /O2 and /Ob2)
# -mavx2: Enable AVX2 instructions (matches /arch:AVX2)
# -ffast-math: Fast floating point arithmetic (matches /fp:fast)
# -ffunction-sections -fdata-sections: Prepare for dead code stripping (matches /Gy /Gw)
OPT_FLAGS="-O3 -march=native -ffast-math -ffunction-sections -fdata-sections"

# Run CMake
# Note: Adjust CMAKE_PREFIX_PATH to match your Linux dependency location
cmake -G Ninja -S ../src/ -B . \
    -DCMAKE_C_FLAGS="$OPT_FLAGS" \
    -DCMAKE_CXX_FLAGS="$OPT_FLAGS" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_PREFIX_PATH="$HOME/dev/deps/" \
    -DCMAKE_INTERPROCEDURAL_OPTIMIZATION=ON \
    -DCMAKE_EXE_LINKER_FLAGS="-Wl,--gc-sections"

# Run Ninja with 4 parallel jobs
ninja -j4

# Return to previous directory
cd ..