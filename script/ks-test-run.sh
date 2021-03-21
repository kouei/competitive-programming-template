for((i = 1; ; ++i)); do
    echo -n "Test $i: "
    test/out0-gen $i > test/gen-input.txt
    diff -w <(test/out1-fast < test/gen-input.txt) <(test/out2-slow < test/gen-input.txt) || break
    echo "PASSED"
done
