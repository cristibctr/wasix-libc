#!/bin/sh

./build32.sh
cp -r sysroot/* /home/x33f3/wasix-sysroot2025-local
cp tools/clang-wasix.cmake_toolchain /home/x33f3/wasix-sysroot2025-local