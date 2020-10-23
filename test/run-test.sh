for((i = 1; ; ++i)); do
    echo -n "Test $i: "
    ./out0-gen $i > gen-input.txt
    diff -w <(./out1-fast < gen-input.txt) <(./out2-slow < gen-input.txt) || break
    echo "PASSED"
done
