#!/bin/bash
# Build script for Windows (run in WSL, Git Bash or MSYS2)

set -e

echo "=== Building Transporti for Windows ==="

# Check dependencies
command -v cmake >/dev/null 2>&1 || { echo "CMake is required but not installed. Aborting." >&2; exit 1; }

# Create build directory
mkdir -p build
cd build

# Configure - use Unix Makefiles for cross-platform compatibility
echo "Configuring..."
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build
echo "Building..."
cmake --build . -j$(nproc)

echo ""
echo "=== Build complete! ==="
echo "Run with: ./build/Transporti"
