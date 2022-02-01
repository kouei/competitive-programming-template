#!/bin/bash
set -x
mkdir -p bin
clang++ src/main.cpp -std=c++17 -Wshadow -Wall -fsanitize=address -fsanitize=undefined -D_GLIBCXX_DEBUG -g -o bin/main 