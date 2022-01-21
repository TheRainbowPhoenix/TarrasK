
# PC compiled SDK location, could be safely ignored.
PC_SDK_DIR:=/mnt/c/Users/Phoebe/CLionProjects/sdk


# Native C compiler
CC_PC:=gcc
CC_PC_FLAGS:=-W -Wall -DPC -I/usr/include/SDL2/ -lSDL2 -I $(SDK_DIR)/include-pc/ -L$(PC_SDK_DIR)/target/  -lsdk -lmingw32 -lSDL2main

# SDL location
WIN32_SDL_DIR = /mnt/c/Users/Phoebe/cross/src/SDL2-2.0.20/x86_64-w64-mingw32/
# MinGW C/C++ compiler
CC_WIN32:=x86_64-w64-mingw32-gcc
CXX_WIN32:=x86_64-w64-mingw32-g++
CC_WIN32_FLAGS:=-W -Wall -DPC -I$(WIN32_SDL_DIR)/include -L$(WIN32_SDL_DIR)/lib -I$(PC_SDK_DIR)/include/ -L$(PC_SDK_DIR)/target/ -lsdk  -lSDL2 -lSDL2main

APP_PC:=$(APP_NAME).elf
APP_WIN32:=$(APP_NAME).exe
