#!/usr/bin/bash
# Install all dependencies for BerialDraw on Linux

# XCB dependencies (default backend - lightweight X11)
sudo apt update
sudo apt install -y git cmake build-essential pkg-config
sudo apt install -y libxcb1-dev libxcb-image0-dev libxcb-keysyms1-dev libxcb-icccm4-dev

# Build BerialDraw with XCB backend (default)
REPO_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$REPO_DIR"
cmake -S . -B build
cmake --build build -j$(nproc)

# To use SDL3 instead (optional), run:
# cmake -S . -B build -DUSE_SDL=ON
# (requires SDL3 to be installed separately)

