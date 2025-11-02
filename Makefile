
CPP_STD := -std=c++23
CPP_FLAG := -g -O0 -Wall $(CPP_STD)
MAIN_SRC := main.cpp
ARCHIVE_SRC := archive/*
ALL_SRC := $(MAIN_SRC) $(ARCHIVE_SRC)
ARTIFACTS := main

main: $(MAIN_SRC)
	clang++ $(MAIN_SRC) $(CPP_FLAG) -o $@

.PHONY: clang-tidy clang-format clean

tidy:
	clang-tidy $(ALL_SRC) -- $(CPP_STD)

format:
	clang-format -i $(ALL_SRC)

clean:
	rm -f $(ARTIFACTS)