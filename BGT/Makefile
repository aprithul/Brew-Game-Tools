SRC_DIR = src
INC_DIR = include
BUILD_DIR = build/debug
CC = clang++
INCLUDE_PATHS = -Iinclude -I../Vendor/SDL2/include -I../Vendor/GL/include

#BACKENDS = $(wildcard $(BUILD_DIR)/Backend_*.o)

ifeq ($(OS),Windows_NT) 
    detected_OS := Windows
else
    detected_OS := $(shell sh -c 'uname 2>/dev/null || echo Unknown')
endif 


ifeq ($(detected_OS),Windows)
LINKER_FLAGS = -lsdl2 -lopengl32 -lsdl2_mixer -lsdl2_ttf  -lglew32 -Wl,--out-implib,$(BUILD_DIR)/libbgt_dll.a
COMPILER_FLAGS = -std=c++11 -c -Wall -O2 -g
LIBRARY_PATHS = -L../BGT/build/debug/ -L../Vendor/SDL2/lib -L../Vendor/GL/lib
	
all: $(BUILD_DIR)/bgt.o

clean:
	rm -r -f $(BUILD_DIR)/*

$(BUILD_DIR)/bgt.o:$(SRC_DIR)/*.cpp $(INC_DIR)/*.hpp
	$(CC) -DEXPORT_AS_DLL $(COMPILER_FLAGS) $(INCLUDE_PATHS) $(SRC_DIR)/BrewGameTool.cpp -o $(BUILD_DIR)/bgt.o
	$(CC) -shared -o $(BUILD_DIR)/bgt.dll $(BUILD_DIR)/bgt.o $(LIBRARY_PATHS) $(LINKER_FLAGS) 
endif


ifeq ($(detected_OS),Darwin)
LINKER_FLAGS = -framework OpenGL -lsdl2 -lsdl2_mixer -lsdl2_ttf -lglew
COMPILER_FLAGS = -std=c++11 -c -Wall -fPIC -O2 -g -v
LIBRARY_PATHS = -L../BGT/build/debug/ -L../Vendor/GL/lib

all: $(BUILD_DIR)/bgt.o

clean:
	rm -r -f $(BUILD_DIR)/*

$(BUILD_DIR)/bgt.o:$(SRC_DIR)/*.cpp
	$(CC) $(COMPILER_FLAGS) $(INCLUDE_PATHS) $(SRC_DIR)/BrewGameTool.cpp -o $(BUILD_DIR)/bgt.o
	$(CC) -dynamiclib -o $(BUILD_DIR)/libbgt.dylib $(BUILD_DIR)/bgt.o $(LIBRARY_PATHS) $(LINKER_FLAGS) 

endif
	
ifeq ($(detected_OS),Linux)
LINKER_FLAGS = -lSDL2 -lSDL2_mixer -lSDL2_ttf -lGLEW
COMPILER_FLAGS = -std=c++11 -c -Wall -fPIC -O2 -g -v
LIBRARY_PATHS = -L../BGT/build/debug/ -L../Vendor/GL/lib
CC = clang++
all: $(BUILD_DIR)/bgt.o

clean:
	rm -r -f $(BUILD_DIR)/*

$(BUILD_DIR)/bgt.o:$(SRC_DIR)/*.cpp
	$(CC) $(COMPILER_FLAGS) $(INCLUDE_PATHS) $(SRC_DIR)/BrewGameTool.cpp -o $(BUILD_DIR)/bgt.o
	$(CC) -shared -o $(BUILD_DIR)/libbgt.so $(BUILD_DIR)/bgt.o $(LIBRARY_PATHS) $(LINKER_FLAGS) 

endif
