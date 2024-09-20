
INC_FOLDERS = -Iinc
CFLAGS = -std=c++23 -O2 $(INC_FOLDERS)
LDFLAGS = -lglfw -lvulkan -ldl -lpthread

BUILD_DIR = ./build
SHADERS_DIR = $(shell realpath ./src/vulkan/shaders)

MODULES = $(shell find src -name "*.cpp")

.PHONY: run clean debug test_main

shaders: $(SHADERS_DIR)/*
	glslc $(SHADERS_DIR)/shader.vert -o $(BUILD_DIR)/vert.spv
	glslc $(SHADERS_DIR)/shader.frag -o $(BUILD_DIR)/frag.spv

test: $(MODULES) shaders
	g++ $(CFLAGS) -o $(BUILD_DIR)/test $(MODULES) $(LDFLAGS)

debug: $(MODULES)
	g++ $(CFLAGS) -g -o $(BUILD_DIR)/test $(MODULES) $(LDFLAGS)

run: test
	$(BUILD_DIR)/test

test_main: src/main.cpp
	g++ $(CFLAGS) -o $(BUILD_DIR)/test src/main.cpp
	$(BUILD_DIR)/test

clean: 
	rm -f $(BUILD_DIR)/*


