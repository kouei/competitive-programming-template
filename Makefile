
CPP_STD := -std=c++23

CPP_RELEASE_FLAG := -O2 -Wno-unused-result -Wshadow -Wall
CPP_DEBUG_FLAG := -g -O0 $(CPP_STD) -Wshadow -Wall -fsanitize=address -fsanitize=undefined -D_GLIBCXX_DEBUG

SRC_FOLDER := src

BINARY_FOLDER := bin
BINARY_FILE := main

INPUT_FOLDER := input
INPUT_FILE := input.txt

TEST_FOLDER := test
TEST_SRC_FOLDER := src
TEST_BINARY_FOLDER := bin

MAIN_SRC := $(SRC_FOLDER)/main.cpp
ARCHIVE_SRC := archive/*
TEMPLATE_SRC := template/*

TEST_GEN_SRC := $(TEST_FOLDER)/$(TEST_SRC_FOLDER)/step0-gen.cpp
TEST_FAST_SRC := $(TEST_FOLDER)/$(TEST_SRC_FOLDER)/step1-fast.cpp
TEST_SLOW_SRC := $(TEST_FOLDER)/$(TEST_SRC_FOLDER)/step2-slow.cpp

ALL_SRC := $(MAIN_SRC) $(ARCHIVE_SRC) $(TEMPLATE_SRC) $(TEST_GEN_SRC) $(TEST_FAST_SRC) $(TEST_SLOW_SRC)
ARTIFACTS := $(BINARY_FOLDER)

debug: $(MAIN_SRC) prepare_folder
	clang++ $(MAIN_SRC) $(CPP_DEBUG_FLAG) -o $(BINARY_FOLDER)/$(BINARY_FILE)

release: $(MAIN_SRC) prepare_folder
	clang++ $(MAIN_SRC) $(CPP_RELEASE_FLAG) -o $(BINARY_FOLDER)/$(BINARY_FILE)


test: $(TEST_GEN_SRC) $(TEST_FAST_SRC) $(TEST_SLOW_SRC) prepare_folder
	clang++ $(TEST_GEN_SRC)  $(CPP_RELEASE_FLAG) -o test/bin/bin0-gen && \
	clang++ $(TEST_FAST_SRC) $(CPP_RELEASE_FLAG) -o test/bin/bin1-fast && \
	clang++ $(TEST_SLOW_SRC) $(CPP_RELEASE_FLAG) -o test/bin/bin2-slow

.PHONY: run clang-tidy clang-format prepare_folder clean

# @ can suppress echo of the command
run:
	@./$(BINARY_FOLDER)/$(BINARY_FILE) < $(INPUT_FOLDER)/$(INPUT_FILE)

tidy:
	clang-tidy $(ALL_SRC) -- $(CPP_STD)

format:
	clang-format -i $(ALL_SRC)

prepare_folder:
	mkdir -p $(BINARY_FOLDER)
	mkdir -p $(TEST_FOLDER)/$(TEST_BINARY_FOLDER)

clean:
	rm -rf $(ARTIFACTS)