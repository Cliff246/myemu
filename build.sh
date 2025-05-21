#!/bin/bash

BUILD_DIR="build"
CMAKE_OPTIONS="-DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug "

# Clean the build directory if '--clean' flag is provided
if [[ "$1" == "--clean" ]]; then
    echo "Cleaning build directory..."
    rm -rf "$BUILD_DIR"
fi

# Create build directory if it doesn't exist
if [ ! -d "$BUILD_DIR" ]; then
    mkdir "$BUILD_DIR"
fi

# Run CMake and build
cd "$BUILD_DIR" || exit 1
cmake .. $CMAKE_OPTIONS
make
