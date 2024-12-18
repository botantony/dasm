# thank you, chatgpt
# Compiler and flags
CC := gcc
# CFLAGS := -Wall -Wextra -Werror -g
# LDFLAGS := -static

# Binary directory
BIN_DIR := bin

# Binary names
COMPILATOR_BINARY := $(BIN_DIR)/compilator
EMULATOR_BINARY := $(BIN_DIR)/emulator

# Source files
COMPILATOR_SRC := assembler.c common.c
EMULATOR_SRC := emulator.c common.c

# Default target
all: $(COMPILATOR_BINARY) $(EMULATOR_BINARY)

# Create the bin directory if it doesn't exist
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Rule for 'compiler' (direct compilation)
$(COMPILATOR_BINARY): $(COMPILATOR_SRC) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

# Rule for 'emulator' (direct compilation)
$(EMULATOR_BINARY): $(EMULATOR_SRC) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

# Clean up build artifacts
.PHONY: clean
clean:
	rm -rf $(BIN_DIR)
