#!/bin/bash

set -ex

mkdir -p build_cmake
cd build_cmake

cmake ..
make
