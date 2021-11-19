SRC_DIR = Demo
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
BUILD_DIR = $(SRC_DIR)
CC = clang++
OBJ_NAME = game
INCLUDE_PATHS = -I../include
LIBRARY_PATHS = -L../lib -L../lib/SDL2 -L../lib/GL -L../build/debug
COMPILER_FLAGS = -std=c++11 -Wall -O0 -g -arch arm64

ifeq ($(OS),Windows_NT)
	LINKER_FLAGS = -lsdl2 -lopengl32 -lglew32 -lCanvas
else
	LINKER_FLAGS = -lsdl2 -framework OpenGL -lglew -lCanvas
endif

all:
	$(CC) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(SRC_FILES) -o $(BUILD_DIR)/$(OBJ_NAME)

clean:
	rm -r -f $(BUILD_DIR)/*