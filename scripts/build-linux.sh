#!/bin/bash
# Build script for Linux

set -e

echo "=== Building Transporti for Linux ==="

# Check dependencies
echo "Checking dependencies..."
command -v cmake >/dev/null 2>&1 || { echo "CMake is required but not installed. Aborting." >&2; exit 1; }

# Create build directory
mkdir -p build
cd build

# Configure
echo "Configuring..."
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build
echo "Building..."
cmake --build . -j$(nproc)

echo ""
echo "=== Build complete! ==="
echo "Run with: ./build/Transporti"
