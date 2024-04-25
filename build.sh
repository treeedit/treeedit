#!/bin/bash

rm -rf build
qt-cmake -S . -B build
cmake --build build

pushd build/test
./treeedit-test
popd

pushd build/src
macdeployqt treeedit.app -codesign="Mac Developer: Paul Borza (976ZM2SB6H)" -dmg
popd
