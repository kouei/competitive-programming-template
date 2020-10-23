clang++ -O2 -std=c++17 -Wno-unused-result -Wshadow -Wall -o out0-gen step0-gen.cpp && \
clang++ -O2 -std=c++17 -Wno-unused-result -Wshadow -Wall -o out1-fast step1-fast.cpp && \
clang++ -O2 -std=c++17 -Wno-unused-result -Wshadow -Wall -o out2-slow step2-slow.cpp