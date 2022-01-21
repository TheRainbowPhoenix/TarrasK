# run `make all` to compile the .hhk and .bin file, use `make` to compile only the .bin file.
# The .hhk file is the original format, the bin file is a newer format.
APP_NAME:=TarrasK

# Directory configuration
SOURCEDIR = src
BUILDDIR = target/deps
PC_BUILDDIR = target/deps_pc
DISTDIR = target
AS_SOURCES:=$(wildcard $(SOURCEDIR)/*.s)
AS_PC_SOURCES:=$(wildcard $(SOURCEDIR)/*.asm)
CC_SOURCES:=$(wildcard $(SOURCEDIR)/*.c)
CXX_SOURCES:=$(wildcard $(SOURCEDIR)/*.cpp)

OBJECTS:=$(patsubst $(SOURCEDIR)/%.s,$(BUILDDIR)/%.o,$(AS_SOURCES)) \
		$(patsubst $(SOURCEDIR)/%.c,$(BUILDDIR)/%.o,$(CC_SOURCES)) \
		$(patsubst $(SOURCEDIR)/%.cpp,$(BUILDDIR)/%.o,$(CXX_SOURCES))

PC_OBJECTS:=$(patsubst $(SOURCEDIR)/%.s,$(PC_BUILDDIR)/%.o,$(AS_PC_SOURCES)) \
		$(patsubst $(SOURCEDIR)/%.c,$(PC_BUILDDIR)/%.o,$(CC_SOURCES)) \
		$(patsubst $(SOURCEDIR)/%.cpp,$(PC_BUILDDIR)/%.o,$(CXX_SOURCES))

# Cross compiler
include config.mk
# Native & Win32 compiler
include config.pc.mk

bin: $(APP_BIN) Makefile

hhk: $(APP_ELF) Makefile

pc: $(APP_PC) Makefile

win32: $(APP_WIN32) Makefile

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

$(APP_PC):  $(CC_SOURCES) $(CXX_SOURCES) $(H_INC) $(HPP_INC)
	$(CC_PC) $(CC_SOURCES) $(CXX_SOURCES) -o $(APP_PC) $(CC_PC_FLAGS)


$(APP_WIN32):  $(PC_OBJECTS) $(CXX_SOURCES) $(H_INC) $(HPP_INC)
	$(CC_WIN32) $(PC_OBJECTS) $(CXX_SOURCES) -o $(APP_WIN32) $(CC_WIN32_FLAGS)


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

tests:
	g++ -Wall -D DEBUG -o $(DISTDIR)/unit_tests -I. $(wildcard tests/*.c)
	./$(DISTDIR)/unit_tests


# TODO: fox win32

$(PC_BUILDDIR)/%.o: $(SOURCEDIR)/%.asm
	@mkdir -p $(PC_BUILDDIR)
	echo "PC ASM"
	$(AS) $< -o $@ $(AS_FLAGS)

$(PC_BUILDDIR)/%.o: $(SOURCEDIR)/%.c
	@mkdir -p $(PC_BUILDDIR)
	echo "PC C"
	$(CC_WIN32) -c $< -o $@ $(CC_WIN32_FLAGS)

$(PC_BUILDDIR)/%.o: $(SOURCEDIR)/%.cpp
	@mkdir -p $(PC_BUILDDIR)
	echo "PC C++"
	$(CXX_WIN32) -c $< -o $@ $(CC_WIN32_FLAGS)
	# @$(READELF) $@ -S | grep ".ctors" > /dev/null && echo "ERROR: Global constructors aren't supported." && rm $@ && exit 1 || exit 0


.PHONY: bin hhk pc all clean tests
