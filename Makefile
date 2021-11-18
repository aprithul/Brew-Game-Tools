SRC_DIR = src
BUILD_DIR = build/debug
CC = clang++
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_NAME = game
INCLUDE_PATHS = -Iinclude
LIBRARY_PATHS = -Llib -Llib/SDL2 -Llib/GL
COMPILER_FLAGS = -std=c++11 -Wall -O0 -g -arch arm64

ifeq ($(OS),Windows_NT)
	LINKER_FLAGS = -lsdl2 -lopengl32 -lglew32
else
	LINKER_FLAGS = -lsdl2 -framework OpenGL -lglew
endif

all:
	$(CC) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(SRC_FILES) -o $(BUILD_DIR)/$(OBJ_NAME)

clean:
	rm -r -f $(BUILD_DIR)/*