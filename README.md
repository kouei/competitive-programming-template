# Competitive Programming Template
My Template for Competitive Programming.
Some Ideas are borrowed from https://github.com/Errichto/youtube/tree/master/testing.

# Coding

1. Implement your solution in **main.cpp**.
2. Add your input to **input.txt**.
3. Use `$ ./fast.sh` to build a **fast** version of your program.
4. (Optional) Use `$ ./slow.sh` to build a **slow** version of your program but with more debug information to help you debug.
5. Use `$ ./run.sh` to test your program with the input.

# Testing

1. `$ cd test`.
2. Implement your input data generator in **gen.cpp**.
3. Implement your clever version program in **a.cpp**.
4. Implement your brute-force version program in **b.cpp**.
5. Use `$ ./build-test.sh` to build **gen**, **a** and **b**.
6. Use `$ ./run-test.sh` to test **a** against **b**.
7. When a test failed, you can check **gen-input.txt** which makes **a** and **b** output different results.
