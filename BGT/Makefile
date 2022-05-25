BUILD_DIR = build/debug
TARGET = $(BUILD_DIR)/bgt
OBJS = src/test.o src/BrewGameTool.cpp
INCDIR = include

CXXFLAGS = -std=c++14 -fno-rtti -fno-exceptions
LIBS = -lm

BUILD_PRX = 1
PSP_FW_VERSION = 500
PSP_LARGE_MEMORY = 1

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Brew Game Tools

PSPSDK = $(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak