# run `make all` to compile the .hhk and .bin file, use `make` to compile only the .bin file.
# The .hhk file is the original format, the bin file is a newer format.
APP_NAME:=TarrasK

SDK_DIR:=/home/phoebe/cross/src/hollyhock-2/sdk
PATH:=/home/phoebe/cross/opt/sh4eb-nofpu-elf/bin/:$(PATH)

ifndef SDK_DIR
$(error You need to define the SDK_DIR environment variable, and point it to the sdk/ folder)
endif

AS:=sh4eb-nofpu-elf-as
AS_FLAGS:=

CC:=sh4eb-nofpu-elf-gcc
CC_FLAGS:=-Os -mb -m4a-nofpu -mhitachi -ffreestanding -fno-strict-aliasing -fshort-wchar -Wall -Wextra -I $(SDK_DIR)/include/ \
	-I $(SDK_DIR)/newlib/sh-elf/include

CXX:=sh4eb-nofpu-elf-g++
CXX_FLAGS:=-Os -mb -m4a-nofpu -mhitachi -ffreestanding -fno-strict-aliasing -fno-exceptions -fno-rtti -fshort-wchar -Wall -Wextra -O2 -I $(SDK_DIR)/include/ \
	-I $(SDK_DIR)/newlib/sh-elf/include

LD:=sh4eb-nofpu-elf-gcc
LD_FLAGS:=-nostartfiles -m4-nofpu -Wno-undef -L$(SDK_DIR)/newlib/sh-elf/lib/ -lm

READELF:=sh4eb-nofpu-elf-readelf
OBJCOPY:=sh4eb-nofpu-elf-objcopy

SOURCEDIR = src
BUILDDIR = target/deps
DISTDIR = target
AS_SOURCES:=$(wildcard $(SOURCEDIR)/*.s)
CC_SOURCES:=$(wildcard $(SOURCEDIR)/*.c)
CXX_SOURCES:=$(wildcard $(SOURCEDIR)/*.cpp)

# OBJECTS:=$(AS_SOURCES:.s=.o) $(CC_SOURCES:.c=.o) $(CXX_SOURCES:.cpp=.o)
OBJECTS:=$(patsubst $(SOURCEDIR)/%.s,$(BUILDDIR)/%.o,$(AS_SOURCES)) \
		$(patsubst $(SOURCEDIR)/%.c,$(BUILDDIR)/%.o,$(CC_SOURCES)) \
		$(patsubst $(SOURCEDIR)/%.cpp,$(BUILDDIR)/%.o,$(CXX_SOURCES))

APP_ELF:=$(DISTDIR)/$(APP_NAME).hhk
APP_BIN:=$(DISTDIR)/$(APP_NAME).bin

bin: $(APP_BIN) Makefile

hhk: $(APP_ELF) Makefile

all: $(APP_ELF) $(APP_BIN) Makefile

clean:
	rm -f $(BUILDDIR)/*.o $(APP_ELF) $(APP_BIN)

$(APP_ELF): $(OBJECTS) $(SDK_DIR)/sdk.o linker_hhk.ld
	@mkdir -p $(DISTDIR)
	$(LD) -T linker_hhk.ld -o $@ $(LD_FLAGS) $(OBJECTS) $(SDK_DIR)/sdk.o $(LUAOBJ)
	$(OBJCOPY) --set-section-flags .hollyhock_name=contents,strings,readonly $(APP_ELF) $(APP_ELF)
	$(OBJCOPY) --set-section-flags .hollyhock_description=contents,strings,readonly $(APP_ELF) $(APP_ELF)
	$(OBJCOPY) --set-section-flags .hollyhock_author=contents,strings,readonly $(APP_ELF) $(APP_ELF)
	$(OBJCOPY) --set-section-flags .hollyhock_version=contents,strings,readonly $(APP_ELF) $(APP_ELF)

$(APP_BIN): $(OBJECTS) $(SDK_DIR)/sdk.o linker_bin.ld
	@mkdir -p $(DISTDIR)
	$(LD) -Wl,--oformat=binary -T linker_bin.ld -o $@ $(LD_FLAGS) $(OBJECTS) $(SDK_DIR)/sdk.o

# We're not actually building sdk.o, just telling the user they need to do it
# themselves. Just using the target to trigger an error when the file is
# required but does not exist.
$(SDK_DIR)/sdk.o:
	$(error You need to build the SDK before using it. Run make in the SDK directory, and check the README.md in the SDK directory for more information)

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.s
	@mkdir -p $(BUILDDIR)
	$(AS) $< -o $@ $(AS_FLAGS)

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.c
	@mkdir -p $(BUILDDIR)
	$(CC) -c $< -o $@ $(CC_FLAGS)

# Break the build if global constructors are present:
# Read the sections from the object file (with readelf -S) and look for any
# called .ctors - if they exist, give the user an error message, delete the
# object file (so that on subsequent runs of make the build will still fail)
# and exit with an error code to halt the build.
$(BUILDDIR)/%.o: $(SOURCEDIR)/%.cpp
	$(CXX) -c $< -o $@ $(CXX_FLAGS)
	@$(READELF) $@ -S | grep ".ctors" > /dev/null && echo "ERROR: Global constructors aren't supported." && rm $@ && exit 1 || exit 0

.PHONY: bin hhk all clean
