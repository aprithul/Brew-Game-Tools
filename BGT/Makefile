SRC_DIR = src
BUILD_DIR = build/debug
CC = clang++
INCLUDE_PATHS = -Iinclude -I../Vendor/SDL2/include -I../Vendor/GL/include
LIBRARY_PATHS = -L../BGT/build/debug/ -L../Vendor/SDL2/lib -L../Vendor/GL/lib
LIBRARY_PATHS = -L../Vendor/SDL2/lib -L../Vendor/GL/lib

#BACKENDS = $(wildcard $(BUILD_DIR)/Backend_*.o)

ifeq ($(OS),Windows_NT)
	LINKER_FLAGS = -lsdl2 -lopengl32 -lsdl2_mixer -lglew32 -Wl,--out-implib,$(BUILD_DIR)/libbgt_dll.a
	COMPILER_FLAGS = -std=c++11 -c -Wall -O0 -g
	
all: bgt
	$(CC) -shared -o $(BUILD_DIR)/bgt.dll $(BUILD_DIR)/bgt.o $(LIBRARY_PATHS) $(LINKER_FLAGS) 

clean:
	rm -r -f $(BUILD_DIR)/*

bgt:
	$(CC) -DBACKEND_SDL2_OPENGL -DEXPORT_AS_DLL $(COMPILER_FLAGS) $(INCLUDE_PATHS) $(SRC_DIR)/BrewGameTool.cpp -o $(BUILD_DIR)/bgt.o

else
	LINKER_FLAGS = -framework OpenGL -lsdl2 -lsdl2_mixer -lglew
	COMPILER_FLAGS = -std=c++11 -c -Wall -fPIC -O0 -g -arch arm64
endif


	