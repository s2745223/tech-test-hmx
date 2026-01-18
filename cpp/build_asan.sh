#!/bin/bash
set -e

# Go to project root (directory containing this script)
cd "$(dirname "$0")"

BUILD_DIR=build-asan

echo "==> Creating build directory: $BUILD_DIR"
mkdir -p "$BUILD_DIR"

echo "==> Configuring CMake with AddressSanitizer"
cmake -S . -B "$BUILD_DIR" \
  -DCMAKE_C_COMPILER=clang \
  -DCMAKE_CXX_COMPILER=clang++ \
  -DCMAKE_BUILD_TYPE=RelWithDebInfo \
  -DCMAKE_C_FLAGS="-O1 -g -fsanitize=address -fno-omit-frame-pointer" \
  -DCMAKE_CXX_FLAGS="-O1 -g -fsanitize=address -fno-omit-frame-pointer" \
  -DCMAKE_EXE_LINKER_FLAGS="-fsanitize=address"

echo "==> Building"
cmake --build "$BUILD_DIR" -j

echo "==> Running Tests with ASan"
./"$BUILD_DIR"/Tests

echo "==> Running ConsoleApp with ASan"
./"$BUILD_DIR"/ConsoleApp

echo "==> Done."



