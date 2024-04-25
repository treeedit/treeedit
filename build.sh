#!/bin/bash

rm -rf build
qt-cmake -S . -B build
cmake --build build
pushd build/test
./treeedit-test
popd
pushd build/src
macdeployqt treeedit.app -dmg
popd
