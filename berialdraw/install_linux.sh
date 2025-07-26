#!/usr/bin/bash
# Installer les dépendances nécessaires
sudo apt update
sudo apt install -y git cmake build-essential libwayland-dev libxkbcommon-dev libdrm-dev libgbm-dev libpulse-dev libudev-dev libegl1-mesa-dev libgles2-mesa-dev
sudo apt-get install libx11-dev libxext-dev libxrandr-dev libxcursor-dev libxi-dev libxinerama-dev libxxf86vm-dev libwayland-dev libxkbcommon-dev wayland-protocols

# Cloner SDL3
git clone https://github.com/libsdl-org/SDL.git
cd SDL

# Compiler et installer
cmake -S . -B build
cmake --build build -j$(nproc)
sudo cmake --install build


git clone --recurse-submodules https://github.com/remibert/berialdraw.git
