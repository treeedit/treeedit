#!/bin/bash

rm -rf build
qt-cmake -S . -B build
cmake --build build

pushd build/test
./treeedit-test
popd

pushd build/src
macdeployqt "Tree Edit.app" -codesign="Developer ID Application: Paul Borza (S4M4K4L3FB)" -dmg
popd
