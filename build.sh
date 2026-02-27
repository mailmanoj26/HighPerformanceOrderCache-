#!/bin/bash

# Exit immediately if a command fails
set -e

# Project variables
BUILD_DIR="build"
EXECUTABLE="HighPerformanceOrderCache"

# Clean previous build if requested
if [ "$1" == "clean" ]; then
    echo "Cleaning build directory..."
    rm -rf $BUILD_DIR
fi

# Create build directory if it doesn't exist
mkdir -p $BUILD_DIR
cd $BUILD_DIR

# Configure CMake (default to Release build)
cmake -DCMAKE_BUILD_TYPE=Release ..

# Build project
cmake --build .

# Run tests
echo "Running tests..."
./$EXECUTABLE
