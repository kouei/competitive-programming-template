#!/bin/bash

ks_build_release() {
    set -x # Trace commands being executed
    mkdir -p bin
    g++ src/main.cpp -O2 -std=c++17 -Wno-unused-result -Wshadow -Wall -o bin/main_release
    set +x # Turn off command trace
}

ks_build_debug() {
    set -x # Trace commands being executed
    mkdir -p bin
    g++ src/main.cpp -O0 -std=c++17 -Wshadow -Wall -fsanitize=address -fsanitize=undefined -D_GLIBCXX_DEBUG -g -o bin/main_debug 
    set +x # Turn off command trace
}

ks_run_release() {
    bin/main_release < src/input.txt
}

ks_run_debug() {
    bin/main_debug < src/input.txt
}

ks_test_build() {
    set -x # Trace commands being executed
    mkdir -p test/bin
    g++ test/src/step0-gen.cpp -O2 -std=c++17 -Wno-unused-result -Wshadow -Wall -o test/bin/bin0-gen && \
    g++ test/src/step1-fast.cpp -O2 -std=c++17 -Wno-unused-result -Wshadow -Wall -o test/bin/bin1-fast && \
    g++ test/src/step2-slow.cpp -O2 -std=c++17 -Wno-unused-result -Wshadow -Wall -o test/bin/bin2-slow
    set +x # Turn off command trace
}

ks_test_run() {
    # below is for displaying colorful text in the output
    green=`tput setaf 2`
    red=`tput setaf 1`
    reset=`tput sgr0`

    mkdir -p test/out

    for((i = 1; ; ++i)); do
        test/bin/bin0-gen > test/out/out0-gen-input.txt
        test/bin/bin1-fast < test/out/out0-gen-input.txt > test/out/out1-fast-output.txt
        test/bin/bin2-slow < test/out/out0-gen-input.txt > test/out/out2-slow-output.txt
        diff --ignore-all-space --side-by-side --suppress-common-lines test/out/out1-fast-output.txt test/out/out2-slow-output.txt > test/out/out3-diff-output.txt
        if [ $? -eq 0 ]
        then
            echo "Test $i: ${green}PASSED${reset}"
        else
            echo "Test $i: ${red}FAILED${reset}"
            echo ""
            echo "${green}Input Data:${reset}"
            cat test/out/out0-gen-input.txt
            echo ""
            echo "${red}diff output:${reset}"
            cat test/out/out3-diff-output.txt
            break
        fi
    done
}