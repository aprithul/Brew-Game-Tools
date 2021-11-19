SRC_DIR = src
BUILD_DIR = build/debug
CC = clang++
OBJ_NAME = libCanvas
INCLUDE_PATHS = -Iinclude
COMPILER_FLAGS = -std=c++11 -c -arch arm64
BACKENDS = $(wildcard $(BUILD_DIR)/Backend_*.o)


all: backends engine
	ar rvs $(BUILD_DIR)/$(OBJ_NAME).a $(BUILD_DIR)/Canvas.o $(BACKENDS)
	rm $(BUILD_DIR)/Canvas.o
	rm $(BACKENDS)


clean:
	rm -r -f $(BUILD_DIR)/*

backends:
	$(CC) $(INCLUDE_PATHS) $(COMPILER_FLAGS) $(SRC_DIR)/Backend_SDL2_OpenGL.cpp -o $(BUILD_DIR)/Backend_SDL2_OpenGL.o
	$(CC) $(INCLUDE_PATHS) $(COMPILER_FLAGS) $(SRC_DIR)/Backend_Dummy.cpp -o $(BUILD_DIR)/Backend_Dummy.o

engine:
	$(CC) $(COMPILER_FLAGS) $(INCLUDE_PATHS) $(SRC_DIR)/Canvas.cpp -o $(BUILD_DIR)/Canvas.o