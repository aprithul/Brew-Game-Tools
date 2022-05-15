SRC_DIR = src
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
BUILD_DIR = build/debug
CC = clang++
OBJ_NAME = game
INCLUDE_PATHS = -Iinclude -I../BGT/include
LINKER_FLAGS = -lbgt

ifeq ($(OS),Windows_NT)
	COMPILER_FLAGS = -std=c++11 -Wall -O0 -g
	LIBRARY_PATHS = -L../BGT/build/debug/
else
	COMPILER_FLAGS = -std=c++11 -Wall -O0 -g -arch arm64
	LIBRARY_PATHS = -L../BGT/build/debug/
endif

all:
	$(CC) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(SRC_FILES) -o $(BUILD_DIR)/$(OBJ_NAME)

clean:
	rm -r -f $(BUILD_DIR)/*