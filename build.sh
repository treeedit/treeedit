#!/bin/bash

rm -rf build
qt-cmake -S . -B build
cmake --build build
cd build/src
macdeployqt treeedit.app -dmg
