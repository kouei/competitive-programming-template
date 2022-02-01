# Competitive Programming Template
My Template for Competitive Programming.
Some Ideas are borrowed from https://github.com/Errichto/youtube/tree/master/testing.

# Coding

1. `source init.sh`
2. Implement your solution in **main.cpp**.
3. Add your input to **input.txt**.
4. Use `ks-fast` to build a **fast** version of your program.
5. (Optional) Use `ks-slow` to build a **slow** version of your program with more debug information to help you debug.
6. Use `ks-run` to test your program with the input.

# Testing

1. Implement your input data generator in **test/src/step0-gen.cpp**.
2. Implement your slow (but absolutely correct) algorithm in **test/src/step1-slow.cpp**.
3. Implement your fast (but incorrect) algorithm in **test/src/step2-fast.cpp**.
4. Use `ks-test-build` to build the above 3 programs.
5. Use `ks-test-run` to compare the output of fast program and slow program.

# Tips
Whenever you solved an algorithm problem and before you turn to another problem, you can enter:  
  
`$ git reset --hard`  
`$ git clean -dfx`  
  
to reset the template.  