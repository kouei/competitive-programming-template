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
2. Implement your input data generator in **step0-gen.cpp**.
3. Implement your slow (but absolutely correct) algorithm in **step1-slow.cpp**.
4. Implement your fast (but incorrect) algorithm in **step2-fast.cpp**.
5. Use `$ ./build-test.sh` to build **out0-gen**, **out1-slow** and **out2-fast**.
6. Use `$ ./run-test.sh` to test **out1-slow** against **out2-fast**.
7. When a test failed, you can check **gen-input.txt** which makes **out1-slow** and **out2-fast** output different results.

# Tips
Whenever you solved an algorithm problem and before you turn to another problem, you can enter:  
  
`$ git reset --hard`  
`$ git clean -dfx`  
  
to reset the template.  