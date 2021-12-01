SRC_DIR = Demo
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
BUILD_DIR = $(SRC_DIR)
CC = clang++
OBJ_NAME = game
INCLUDE_PATHS = -I../include -I../Vendor/SDL2/include -I../Vendor/GL/include
LIBRARY_PATHS = -L../lib -L../Vendor/SDL2/lib -L../Vendor/GL/lib -L../build/debug/
COMPILER_FLAGS = -std=c++11 -Wall -O0 -g -arch arm64

ifeq ($(OS),Windows_NT)
	LINKER_FLAGS = -lsdl2 -lopengl32 -lglew32 -lCanvas
else
	LINKER_FLAGS = -framework OpenGL -lsdl2 -lglew -lCanvas
endif

all:
	$(CC) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(SRC_FILES) -o $(BUILD_DIR)/$(OBJ_NAME)

clean:
	rm -r -f $(BUILD_DIR)/*