SRC_DIR = src
BUILD_DIR = build/debug
CC = clang++
OBJ_NAME = libDrawGL
INCLUDE_PATHS = -Iinclude
COMPILER_FLAGS = -std=c++11 -c -arch arm64

all: graphics engine
	ar rvs $(BUILD_DIR)/$(OBJ_NAME).a $(BUILD_DIR)/Engine.o $(BUILD_DIR)/Graphics.o
	rm $(BUILD_DIR)/Graphics.o
	rm $(BUILD_DIR)/Engine.o


clean:
	rm -r -f $(BUILD_DIR)/*

graphics:
	$(CC) $(INCLUDE_PATHS) $(COMPILER_FLAGS) $(SRC_DIR)/Graphics.cpp -o $(BUILD_DIR)/Graphics.o

engine:
	$(CC) $(COMPILER_FLAGS) $(INCLUDE_PATHS) $(SRC_DIR)/Engine.cpp -o $(BUILD_DIR)/Engine.o