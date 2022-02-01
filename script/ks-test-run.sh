#!/bin/bash

# below is for displaying colorful text in the output
green=`tput setaf 2`
red=`tput setaf 1`
reset=`tput sgr0`

mkdir -p test/out

for((i = 1; ; ++i)); do
    test/bin/bin0-gen > test/out/out0-gen-input.txt
    test/bin/bin1-fast < test/out/out0-gen-input.txt > test/out/out1-fast-output.txt
    test/bin/bin2-slow < test/out/out0-gen-input.txt > test/out/out2-slow-output.txt
    diff -wy --suppress-common-lines test/out/out1-fast-output.txt test/out/out2-slow-output.txt > test/out/out3-diff-output.txt
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