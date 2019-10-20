#!/bin/bash

mkdir -p build && cd build

conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan
conan install .. --build missing && cd .. && cmake -S. -Bbuild
