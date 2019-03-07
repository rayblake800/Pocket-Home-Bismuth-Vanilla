##################### Pocket-Home Bismuth Makefile: ############################ 
################################################################################
## Main build targets:	                                                      ##
##	                                                                          ##
##   build:	            Compile default build type.                           ##
##   devinstall:	    Install default build binaries.                       ##
##   debug:	            Compile debug build, install, and open in gdb.        ##
##   release:	        Compile release build and install binaries.           ##
##   check-pkg-config:	Verifies all pkg-config libraries.                    ##
##   clean:	            Removes all build files.                              ##
##   strip:	            Removes symbols from build files.                     ##
##   uninstall:	        Removes installed binaries and data files.            ##
##	                                                                          ##
################################################################################
## Main build options:	                                                      ##
##	                                                                          ##
## CONFIG=Debug	         Build debug binaries skipping optimizations,         ##
##	                     including all test code, and enabling gdb            ##
##	                     debugging symbols.                                   ##
##	                                                                          ##
## CONFIG=Release	     Build release binaries using full optimization,      ##
##	                     removing all test code, and omitting gdb flags.      ##
##	                                                                          ##
## V=1	                 Enable verbose build output.                         ##
##	                                                                          ##
## OPTIMIZE=1            Set to 1 to enable full optimization, or 0 to        ##
##	                     disable all optimization, overriding the behavior    ##
##	                     selected by the CONFIG option.                       ##
##	                                                                          ##
## GDB_SUPPORT=1         Set to 1 to compile with gdb debugging symbols, or 0 ##
##	                     to disable debugging symbols. This will also         ##
##	                     override the behavior selected by the CONFIG option. ##
##	                                                                          ##
################################################################################
######### Build values: #########

# Default Options:
# Build type: either Debug or Release
CONFIG ?= Debug
# Command used to strip unneeded symbols from object files:
STRIP ?= strip
# Use the build system's architecture by default.
TARGET_ARCH ?= -march=native
# Enable or disable verbose output
V ?= 0
# Whether test classes will be built
BUILD_TESTS ?= 1

# Executable name:
JUCE_TARGET_APP = pocket-home
# Version number:
APP_VERSION = 0.0.8.10
# Version hex 
APP_VERSION_HEX = 0x80a

# Build directories:
JUCE_BINDIR := build
JUCE_LIBDIR := build
# Directory where .o files will be created:
JUCE_OBJDIR := build/intermediate
# Directory where executable files will be created:
JUCE_OUTDIR := build
# Data installation directory
DATA_PATH := /usr/share/$(JUCE_TARGET_APP)

# Pkg-config libraries:
PKG_CONFIG_LIBS = NetworkManager libnm-glib alsa freetype2 libssl gio-2.0 \
	              x11 xext xinerama xpm
  
# Additional library flags:
LDFLAGS := -lcrypto -ldl -lpthread -lrt $(LDFLAGS)

# Preprocessor values used by JUCE library code:
JUCE_DEFS	     := -DDONT_SET_USING_JUCE_NAMESPACE=1 \
	                 -DJUCER_LINUX_MAKE_6D53C8B4=1 \
	                 -DJUCE_APP_VERSION=$(APP_VERSION) \
	                 -DJUCE_APP_VERSION_HEX=$(APP_VERSION_HEX) \

JUCE_CPPFLAGS_APP := -DJucePlugin_Build_VST=0 \
	                 -DJucePlugin_Build_VST3=0 \
	                 -DJucePlugin_Build_AU=0 \
	                 -DJucePlugin_Build_AUv3=0 \
	                 -DJucePlugin_Build_RTAS=0 \
	                 -DJucePlugin_Build_AAX=0 \
	                 -DJucePlugin_Build_Standalone=0

# Extra compilation flags:
CPPFLAGS := -pthread $(CPPFLAGS) 

# Extra compilation flags (C++ only):
CXXFLAGS := -std=gnu++14 $(CXXFLAGS)

# Directories to search for header files:
INCLUDE_DIRS := JuceLibraryCode deps/JUCE/modules

# Directories to recursively search for header files:
RECURSIVE_INCLUDE_DIRS := Source Tests

#### Setup: #### 

# build with "V=1" for verbose builds
ifeq ($(V), 1)
	V_AT =
else
	V_AT = @
endif

# Disable dependency generation if multiple architectures are set
DEPFLAGS := $(if $(word 2, $(TARGET_ARCH)), , -MMD)

# Generate the list of directory include flags:
DIR_FLAGS := $(shell echo $(INCLUDE_DIRS) | xargs printf " -I'%s'") \
	         $(shell find $(RECURSIVE_INCLUDE_DIRS) -type d \
	                 -printf " -I'%p'")

# Generate the build label from the /etc/os-release file:
BUILD_NAME := $(shell ./project-scripts/BuildLabel.sh)

# Keep debug and release build files in separate directories:
JUCE_OBJDIR := $(JUCE_OBJDIR)/$(CONFIG)
JUCE_OUTDIR := $(JUCE_OUTDIR)/$(CONFIG)

ifeq ($(CONFIG),Debug)
    # Disable optimization and enable gdb flags unless otherwise specified:
    OPTIMIZATION ?= 0
    GDB_SUPPORT ?= 1
	# Debug-specific preprocessor definitions:
	JUCE_CONFIG_FLAGS = -DDEBUG=1 -D_DEBUG=1
endif

ifeq ($(CONFIG),Release)
    # Enable optimization and disable gdb flags unless otherwise specified:
    OPTIMIZATION ?= 1
    GDB_SUPPORT ?= 0
	# Release-specific preprocessor definitions:
	JUCE_CONFIG_FLAGS = -DNDEBUG=1
endif

# Set optimization level flags:
ifeq ($(OPTIMIZATION), 1)
    CONFIG_CFLAGS := $(CONFIG_CFLAGS) -O3 -flto
    CONFIG_LDFLAGS := $(CONFIG_LDFLAGS) -flto
else
    CONFIG_CFLAGS := $(CONFIG_CFLAGS) -O0
endif

# Set debugging flags:
ifeq ($(GDB_SUPPORT), 1)
    CONFIG_CFLAGS := $(CONFIG_CFLAGS) -g -ggdb
else
    CONFIG_LDFLAGS := $(CONFIG_LDFLAGS) -fvisibility=hidden
endif
  
JUCE_CPPFLAGS := $(DEPFLAGS) \
	             $(JUCE_CONFIG_FLAGS) \
	             $(JUCE_DEFS)\
                 -DBUILD_NAME="\"$(shell ./project-scripts/BuildLabel.sh)\"" \
	             $(shell pkg-config --cflags $(PKG_CONFIG_LIBS)) \
	             $(DIR_FLAGS)

JUCE_CFLAGS   += $(JUCE_CPPFLAGS) \
				 $(TARGET_ARCH) \
				 $(CONFIG_CFLAGS) \
				 $(CFLAGS)

JUCE_CXXFLAGS += $(JUCE_CFLAGS) \
				 $(CXXFLAGS)

JUCE_LDFLAGS  += $(TARGET_ARCH) \
	             -L$(JUCE_BINDIR) \
	             -L$(JUCE_LIBDIR) \
	             $(shell pkg-config --libs $(PKG_CONFIG_LIBS)) \
	             $(CONFIG_LDFLAGS) \
	             $(LDFLAGS)

CLEANCMD = rm -rf $(JUCE_OUTDIR)/$(TARGET) $(JUCE_OBJDIR)


.PHONY: build devInstall debug release clean strip uninstall
build : $(JUCE_OUTDIR)/$(JUCE_TARGET_APP)

# Include makefiles defining each module:
include $(shell pwd)/JuceLibraryCode/Makefile
include $(shell pwd)/makefiles/*.mk

OBJECTS_MAIN := \
  $(JUCE_OBJDIR)/Main.o \
  $(JUCE_OBJDIR)/PocketHomeApplication.o \
  $(JUCE_OBJDIR)/PocketHomeWindow.o \
  $(JUCE_OBJDIR)/HomePage.o 

OBJECTS_MAIN_TEST :=

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_MAIN := $(OBJECTS_MAIN) $(OBJECTS_MAIN_TEST)
endif

MODULES := $(MODULES) main
OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_MAIN)
main : $(OBJECTS_MAIN)
	@echo "Built pocket-home"

$(JUCE_OUTDIR)/$(JUCE_TARGET_APP) : check-pkg-config $(MODULES) $(RESOURCES)
	@echo Linking "pocket-home - App"
	-$(V_AT)mkdir -p $(JUCE_BINDIR)
	-$(V_AT)mkdir -p $(JUCE_LIBDIR)
	-$(V_AT)mkdir -p $(JUCE_OUTDIR)
	$(V_AT)$(CXX) -o $(JUCE_OUTDIR)/$(JUCE_TARGET_APP) $(OBJECTS_APP) \
		             $(JUCE_LDFLAGS) $(JUCE_LDFLAGS_APP) $(RESOURCES) \
					 $(TARGET_ARCH)

$(OBJECTS_APP) :
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "   Compiling: $(<F) -> $(@F)"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
		-o "$@" -c "$<"

devinstall:
	killall $(JUCE_TARGET_APP);\
	sudo cp build/$(CONFIG)/$(JUCE_TARGET_APP) /usr/bin/$(JUCE_TARGET_APP) && \
	if [ ! -d $(DATA_PATH) ]; then \
		sudo mkdir $(DATA_PATH) ; \
	fi && \
	sudo cp -R assets/* $(DATA_PATH)

debug:
	$(MAKE) CONFIG=Debug
	reset
	$(MAKE) devinstall CONFIG=Debug
	gdb $(JUCE_TARGET_APP)

release:
	$(MAKE) CONFIG=Release
	reset
	$(MAKE) devinstall CONFIG=Release

check-pkg-config:
	@command -v pkg-config >/dev/null 2>&1 || { echo >&2 \
		"pkg-config not installed. Please, install it."; exit 1; }
	@pkg-config --print-errors $(PKG_CONFIG_LIBS)

clean:
	@echo Cleaning $(JUCE_TARGET_APP)
	$(V_AT)$(CLEANCMD)

strip:
	@echo Stripping $(JUCE_TARGET_APP)
	-$(V_AT)$(STRIP) --strip-unneeded $(JUCE_OUTDIR)/$(TARGET)

uninstall:
	@echo Uninstalling $(JUCE_TARGET_APP)
	killall $(JUCE_TARGET_APP);\
	sudo rm /usr/bin/$(JUCE_TARGET_APP) && \
	sudo rm -r  $(DATA_PATH) ; \

-include $(OBJECTS_APP:%.o=%.d)

$(JUCE_OBJDIR)/Main.o: \
	Source/Main.cpp
$(JUCE_OBJDIR)/PocketHomeApplication.o: \
	Source/PocketHomeApplication.cpp
$(JUCE_OBJDIR)/PocketHomeWindow.o: \
	Source/PocketHomeWindow.cpp
$(JUCE_OBJDIR)/HomePage.o: \
	Source/HomePage.cpp
