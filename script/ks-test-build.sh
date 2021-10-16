clang++ -O2 -std=c++17 -Wno-unused-result -Wshadow -Wall -o test/out0-gen test/step0-gen.cpp && \
clang++ -O2 -std=c++17 -Wno-unused-result -Wshadow -Wall -o test/out1-fast test/step1-fast.cpp && \
clang++ -O2 -std=c++17 -Wno-unused-result -Wshadow -Wall -o test/out2-slow test/step2-slow.cpp