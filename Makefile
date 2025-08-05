# Modular build system for 3D Cube project
# Organized directory structure: src/, include/, build/

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build

# Compiler and flags  
CC = gcc
CFLAGS = -Wall -Wextra -std=gnu99 -I$(INCLUDE_DIR)
LIBS = -lm

# Target executable
TARGET = cube

# Source and object files
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Default target
all: $(TARGET)

# Build the main executable
$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(LIBS)

# Build object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Rebuild everything
rebuild: clean all

# Show project structure
structure:
	@echo "Project Structure:"
	@find . -type f -name "*.c" -o -name "*.h" -o -name "Makefile" | sort

.PHONY: all run clean rebuild structure