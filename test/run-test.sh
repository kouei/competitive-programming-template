for((i = 1; ; ++i)); do
    echo -n "Test $i: "
    ./gen $i > gen-input.txt
    diff -w <(./a < gen-input.txt) <(./b < gen-input.txt) || break
    echo "PASSED"
done
