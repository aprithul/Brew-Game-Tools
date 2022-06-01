SRC_DIR = src
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
BUILD_DIR = build/debug
CC = clang++
INCLUDE_PATHS = -Iinclude -I../BGT/include
LIBRARY_PATHS = ../BGT/build/debug/
LINKER_FLAGS = -lbgt
DEPENDENCIES = $(SRC_FILES) ../BGT/build/debug/*.o ../BGT/include/*.hpp

ifeq ($(OS),Windows_NT)
	COMPILER_FLAGS = -std=c++11 -Wall -O0 -g
	OBJ_NAME = game.exe

all:$(BUILD_DIR)/$(OBJ_NAME)

$(BUILD_DIR)/$(OBJ_NAME):$(DEPENDENCIES)
	$(CC) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(INCLUDE_PATHS) -L$(LIBRARY_PATHS) $(SRC_FILES) -o $(BUILD_DIR)/$(OBJ_NAME)

clean:
	rm -r -f $(BUILD_DIR)/*
else
	COMPILER_FLAGS = -std=c++11 -Wall -O0 -g
	OBJ_NAME = game
	
all:$(BUILD_DIR)/$(OBJ_NAME)

$(BUILD_DIR)/$(OBJ_NAME):$(DEPENDENCIES)
	$(CC) $(COMPILER_FLAGS) $(LINKER_FLAGS) -I$(INCLUDE_PATHS) -L$(LIBRARY_PATHS) $(SRC_FILES) -o $(BUILD_DIR)/$(OBJ_NAME)

clean:
	rm -r -f $(BUILD_DIR)/*
endif

