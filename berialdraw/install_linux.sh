#!/usr/bin/bash
# Install all dependencies
sudo apt update
sudo apt install -y git cmake build-essential libwayland-dev libxkbcommon-dev libdrm-dev libgbm-dev libpulse-dev libudev-dev libegl1-mesa-dev libgles2-mesa-dev
sudo apt-get install -y libx11-dev libxext-dev libxrandr-dev libxcursor-dev libxi-dev libxinerama-dev libxxf86vm-dev libwayland-dev libxkbcommon-dev wayland-protocols

# SDL3 git clone
git clone https://github.com/libsdl-org/SDL.git
cd SDL

# Build SDL3
cmake -S . -B build
cmake --build build -j$(nproc)

# Install SDL3
sudo cmake --install build

# Berial draw clone
git clone --recurse-submodules https://github.com/remibert/berialdraw.git
cd berialdraw/berialdraw
cmake -S . -B build
cmake --build build

