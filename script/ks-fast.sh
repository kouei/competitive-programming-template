#!/bin/bash
set -x
mkdir -p bin
g++ src/main.cpp -O2 -std=c++17 -Wno-unused-result -Wshadow -Wall -o bin/main