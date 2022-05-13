SRC_DIR = src
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
BUILD_DIR = build/debug
CC = clang++
OBJ_NAME = game
INCLUDE_PATHS = -Iinclude -I../Canvas/include -I../Vendor/SDL2/include -I../Vendor/GL/include
LIBRARY_PATHS = -L../Canvas/build/debug/ -L../Vendor/SDL2/lib -L../Vendor/GL/lib
COMPILER_FLAGS = -std=c++11 -Wall -O0 -g -arch arm64

ifeq ($(OS),Windows_NT)
	LINKER_FLAGS = -lsdl2 -lopengl32 -lsdl2_mixer -lglew32 -lCanvas
else
	LINKER_FLAGS = -framework OpenGL -lsdl2 -lsdl2_mixer -lglew -lCanvas
endif

all:
	$(CC) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(SRC_FILES) -o $(BUILD_DIR)/$(OBJ_NAME)

clean:
	rm -r -f $(BUILD_DIR)/*