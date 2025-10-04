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
TARGET = $(BUILD_DIR)/cube

# Source and object files
SOURCES = $(wildcard $(SRC_DIR)/*.c)
CORE_SOURCES = $(filter-out $(SRC_DIR)/main.c,$(SOURCES))
CORE_OBJECTS = $(CORE_SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
MAIN_OBJECT = $(BUILD_DIR)/main.o

# Benchmark harness
BENCHMARK_SRC = tools/benchmark.c
BENCHMARK_OBJECT = $(BUILD_DIR)/benchmark.o
BENCHMARK_BINARY = $(BUILD_DIR)/benchmark

# Default target
all: $(TARGET)

# Build the main executable
$(TARGET): $(CORE_OBJECTS) $(MAIN_OBJECT)
	$(CC) -o $@ $^ $(LIBS)

# Build object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BENCHMARK_OBJECT): $(BENCHMARK_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Build benchmark harness
benchmark: $(BENCHMARK_BINARY)
	$(BENCHMARK_BINARY)

$(BENCHMARK_BINARY): $(CORE_OBJECTS) $(BENCHMARK_OBJECT)
	$(CC) -o $@ $^ $(LIBS)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Clean build artifacts
clean:
	@if [ -d $(BUILD_DIR) ]; then find $(BUILD_DIR) -type f -not -name '.gitkeep' -delete; fi
	rm -f $(TARGET) $(BENCHMARK_BINARY)

# Rebuild everything
rebuild: clean all

# Show project structure
structure:
	@echo "Project Structure:"
	@find . -type f -name "*.c" -o -name "*.h" -o -name "Makefile" | sort

.PHONY: all run clean rebuild structure benchmark