# FPGA Image Processing Pipeline Simulator Makefile

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic

# Directories
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build
BIN_DIR = bin
ASSETS_DIR = assets
OUTPUT_DIR = output

# Source files (updated with new modules)
SRC = $(SRC_DIR)/main.cpp \
      $(SRC_DIR)/pipeline.cpp \
      $(SRC_DIR)/io.cpp \
      $(SRC_DIR)/colour_converter.cpp \
      $(SRC_DIR)/smoothing_filter.cpp \
      $(SRC_DIR)/edge_filter.cpp \
      $(SRC_DIR)/convolution.cpp \
      $(SRC_DIR)/buffer.cpp  # NEW: Buffer implementation

# Object files
OBJ = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC))

# Executable
TARGET = $(BIN_DIR)/pipeline_sim

# Build configurations
.PHONY: all debug release fixed hw_sim clean run test

# Default: Debug build with all features
all: CXXFLAGS += -I$(INC_DIR) -DDEBUG -DUSE_FIXED_POINT -DHW_SIMULATION -g -O0
all: directories $(TARGET)

# Debug build (maximum debugging)
debug: CXXFLAGS += -I$(INC_DIR) -DDEBUG -DDEBUG_LEVEL=5 -g -O0 -fsanitize=address
debug: directories $(TARGET)

# Release build (optimized)
release: CXXFLAGS += -I$(INC_DIR) -DNDEBUG -O3
release: directories $(TARGET)

# Fixed-point only (FPGA simulation)
fixed: CXXFLAGS += -I$(INC_DIR) -DUSE_FIXED_POINT -DDEBUG -g
fixed: directories $(TARGET)

# Hardware simulation mode
hw_sim: CXXFLAGS += -I$(INC_DIR) -DHW_SIMULATION -DDEBUG -DUSE_FIXED_POINT -g
hw_sim: directories $(TARGET)

# Create directories
directories:
	@mkdir -p $(BUILD_DIR) $(BIN_DIR) $(OUTPUT_DIR) $(ASSETS_DIR)

# Link executable
$(TARGET): $(OBJ)
	@echo "Linking executable..."
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)
	@echo "Build complete: $(TARGET)"

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compiling $<..."
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run with default parameters
run: all
	@echo "Running pipeline..."
	@./$(TARGET) $(ASSETS_DIR)/test_pattern.ppm $(OUTPUT_DIR)/output.ppm

# Run comprehensive tests
test: all
	@echo "=== Running Comprehensive Tests ==="
	@echo "1. Basic pipeline test..."
	@./$(TARGET) $(ASSETS_DIR)/test_pattern.ppm $(OUTPUT_DIR)/test_basic.ppm
	@echo "2. Hardware simulation test..."
	@$(MAKE) hw_sim
	@./$(BIN_DIR)/pipeline_sim $(ASSETS_DIR)/test_pattern.ppm $(OUTPUT_DIR)/test_hw.ppm
	@echo "3. Fixed-point test..."
	@$(MAKE) fixed
	@./$(BIN_DIR)/pipeline_sim $(ASSETS_DIR)/test_pattern.ppm $(OUTPUT_DIR)/test_fixed.ppm
	@echo "=== Tests Complete ==="

# Generate test pattern
generate_test:
	@echo "Generating test image..."
	@mkdir -p $(ASSETS_DIR)
	@echo "P3\n256 256\n255\n" > $(ASSETS_DIR)/test_pattern.ppm
	@for i in $$(seq 1 256); do \
		for j in $$(seq 1 256); do \
			r=$$(( (i * j) % 255 )); \
			g=$$(( (i * 2) % 255 )); \
			b=$$(( (j * 2) % 255 )); \
			echo "$$r $$g $$b"; \
		done; \
	done >> $(ASSETS_DIR)/test_pattern.ppm
	@echo "Test image created: $(ASSETS_DIR)/test_pattern.ppm"

# Clean build artifacts
clean:
	@echo "Cleaning..."
	rm -rf $(BUILD_DIR) $(BIN_DIR) $(OUTPUT_DIR)/*.ppm

# Deep clean
distclean: clean
	rm -rf $(ASSETS_DIR)/test_*.ppm

# Help
help:
	@echo "Available targets:"
	@echo "  all        - Build with all features (default)"
	@echo "  debug      - Build with maximum debugging"
	@echo "  release    - Build optimized release version"
	@echo "  fixed      - Build with fixed-point arithmetic"
	@echo "  hw_sim     - Build with hardware simulation"
	@echo "  run        - Build and run with default image"
	@echo "  test       - Run comprehensive tests"
	@echo "  generate_test - Create test pattern"
	@echo "  clean      - Remove build artifacts"
	@echo "  distclean  - Remove all generated files"
	@echo "  help       - Show this help"