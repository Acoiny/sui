MAKEFLAGS=-j

BINARY := libsui.a

# prefix with debug, to make running as easy as typing d-TAB
BINARY_DEBUG := debug_$(BINARY)

# flags only for debugging
DEBUG_FLAGS = -g3 -O0 -ggdb

# flags only for release
RELEASE_FLAGS := -O3

# CFLAGS = -Wall -Wextra -Werror -std=gnu++23 -march=native -Wno-unused-parameter
CFLAGS = -std=gnu++23 -Wall -Wno-unused-parameter # -finline-functions -fno-exceptions

CFLAGS += -I hdr

SRCDIR := src

OUTDIR := bin

SOURCE_FILES = $(wildcard src/*.cpp)
SOURCE_FILES += $(wildcard src/*/*.cpp)
SOURCE_FILES += $(wildcard src/*/*/*.cpp)

OBJECT_FILES = $(patsubst %.cpp,$(OUTDIR)/release/%.o,$(shell basename -a $(SOURCE_FILES)))

DOBJECT_FILES = $(patsubst %.cpp,$(OUTDIR)/debug/%.o,$(shell basename -a $(SOURCE_FILES)))

ASSEMBLY_FILES = $(DOBJECT_FILES:$(OUTDIR)/debug/%.o=$(OUTDIR)/assembly/%.s)

$(shell mkdir -p $(OUTDIR)/release)
$(shell mkdir -p $(OUTDIR)/debug)

LDFLAGS +=-lSDL3 -lSDL3_ttf

.PHONY := all debug release clean

all: debug

install: release
	mv $(BINARY) $(INSTALL_DIR)

release: $(BINARY)
release: CFLAGS += $(RELEASE_FLAGS)

embeds:
	xxd -i assets/Archivo-Regular.ttf > hdr/embed_data.hpp

debug: $(BINARY_DEBUG)
debug: CFLAGS += $(DEBUG_FLAGS)

$(BINARY): $(OBJECT_FILES)
	ar rcs $@ $^
	# $(CXX) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BINARY_DEBUG): $(DOBJECT_FILES)
	ar rcs $@ $^
	# $(CXX) $(CFLAGS) $^ -o $@ $(LDFLAGS)

## Release objects
# rule for all object files directly in the src folder
$(OUTDIR)/release/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

# rule for all subfolders
$(OUTDIR)/release/%.o: $(SRCDIR)/*/%.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

$(OUTDIR)/release/%.o: $(SRCDIR)/*/*/%.cpp
	$(CXX) $(CFLAGS) -c $< -o $@
## --

## Debug objects
$(OUTDIR)/debug/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

$(OUTDIR)/debug/%.o: $(SRCDIR)/*/%.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

$(OUTDIR)/debug/%.o: $(SRCDIR)/*/*/%.cpp
	$(CXX) $(CFLAGS) -c $< -o $@
## --

# rule to dump assembly
assembly: $(ASSEMBLY_FILES)
assembly: CFLAGS += $(DEBUG_FLAGS)

$(ASSEMBLY_FILES): $(DOBJECT_FILES)
	$(shell mkdir -p $(OUTDIR)/assembly)
	objdump -d -S $< > $@

clean:
	rm -f $(OBJECT_FILES) $(DOBJECT_FILES) $(ASSEMBLY_FILES) $(BINARY) $(BINARY_DEBUG)

rebuild:
	$(DEL)
	make all
