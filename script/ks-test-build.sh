#!/bin/bash
set -x
mkdir -p test/bin
clang++ test/src/step0-gen.cpp -O2 -std=c++17 -Wno-unused-result -Wshadow -Wall -o test/bin/bin0-gen && \
clang++ test/src/step1-fast.cpp -O2 -std=c++17 -Wno-unused-result -Wshadow -Wall -o test/bin/bin1-fast && \
clang++ test/src/step2-slow.cpp -O2 -std=c++17 -Wno-unused-result -Wshadow -Wall -o test/bin/bin2-slow