
CPP_STD := -std=c++23

CPP_RELEASE_FLAG := -O2 -Wno-unused-result -Wshadow -Wall
CPP_DEBUG_FLAG := -g -O0 $(CPP_STD) -Wshadow -Wall -fsanitize=address -fsanitize=undefined -D_GLIBCXX_DEBUG

SRC_FOLDER := src

BINARY_FOLDER := bin
BINARY_FILE := main

INPUT_FOLDER := input
INPUT_FILE := input.txt

MAIN_SRC := $(SRC_FOLDER)/main.cpp
ARCHIVE_SRC := archive/*
TEMPLATE_SRC := template/*

ALL_SRC := $(MAIN_SRC) $(ARCHIVE_SRC) $(TEMPLATE_SRC)
ARTIFACTS := $(BINARY_FOLDER)

debug: $(MAIN_SRC) prepare_folder
	clang++ $(MAIN_SRC) $(CPP_DEBUG_FLAG) -o $(BINARY_FOLDER)/$(BINARY_FILE)

release: $(MAIN_SRC) prepare_folder
	clang++ $(MAIN_SRC) $(CPP_RELEASE_FLAG) -o $(BINARY_FOLDER)/$(BINARY_FILE)

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

clean:
	rm -rf $(ARTIFACTS)