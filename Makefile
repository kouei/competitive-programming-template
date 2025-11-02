
CPP_STD := -std=c++23

CPP_RELEASE_FLAG := -O2 -Wno-unused-result -Wshadow -Wall
CPP_DEBUG_FLAG := -g -O0 $(CPP_STD) -Wshadow -Wall -fsanitize=address -fsanitize=undefined -D_GLIBCXX_DEBUG

SRC_FOLDER := src

BINARY_FOLDER := bin
BINARY_NAME := main

MAIN_SRC := $(SRC_FOLDER)/main.cpp
ARCHIVE_SRC := archive/*
TEMPLATE_SRC := template/*
ALL_SRC := $(MAIN_SRC) $(ARCHIVE_SRC) $(TEMPLATE_SRC)
ARTIFACTS := $(BINARY_FOLDER)/*

debug: $(MAIN_SRC)
	clang++ $(MAIN_SRC) $(CPP_DEBUG_FLAG) -o $(BINARY_FOLDER)/$(BINARY_NAME)

release: $(MAIN_SRC)
	clang++ $(MAIN_SRC) $(CPP_RELEASE_FLAG) -o $(BINARY_FOLDER)/$(BINARY_NAME)


.PHONY: run clang-tidy clang-format clean

# @ can suppress echo of the command
run:
	@./$(BINARY_FOLDER)/$(BINARY_NAME) < $(SRC_FOLDER)/input.txt

tidy:
	clang-tidy $(ALL_SRC) -- $(CPP_STD)

format:
	clang-format -i $(ALL_SRC)

clean:
	rm -f $(ARTIFACTS)