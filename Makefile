# Project Name
TARGET = DaisyMulti

# Enable debugging for J-Link
DEBUG = 1

# Set optimization level so code will fit (this should also be set in libdaisy and DaisySP)
OPT = -Os

CPPFLAGS += -DUSBCON
CPPFLAGS += -DUSBD_VID=0x0483
CPPFLAGS += -DUSBD_PID=0x5740
CPPFLAGS += -DUSB_MANUFACTURER="Unknown"
CPPFLAGS += -DUSB_PRODUCT="\"ELECTROSMITH_DAISY CDC in FS Mode\""
CPPFLAGS += -DHAL_PCD_MODULE_ENABLED

# Sources
CPP_SOURCES = src/DaisyMulti.cpp $(wildcard lib/DaisyEffects/*.cpp) $(wildcard lib/DaisyEffects/Hardware/*.cpp) $(wildcard lib/DaisyInputs/*.cpp) $(wildcard lib/Helpers/*.cpp)

# Library Locations
LIBDAISY_DIR = lib/libdaisy
DAISYSP_DIR = lib/DaisySP
DAISYSP_LGPL_DIR = lib/DaisySP/DaisySP-LGPL

# DaisySP v1.0.0 moved Compressor, ReverbSc, and Fold into a separate LGPL
# sub-library. Add the LGPL Source dir and its -D flag together via C_INCLUDES
# so both survive the core Makefile's "CPPFLAGS = $(CFLAGS)" override.
# (The core Makefile uses "C_INCLUDES ?=" which respects a pre-set value.)
C_INCLUDES += -I$(DAISYSP_LGPL_DIR)/Source -DUSE_DAISYSP_LGPL

# Linker flags
# This is not really required, used only for profiling! Increases executable size by ~8kB
LDFLAGS = -u _printf_float

# Link the LGPL sub-library (Compressor, ReverbSc, Fold, etc.)
LIBS += -ldaisysp-lgpl
LIBDIR += -L$(DAISYSP_LGPL_DIR)/build

# Core location, and generic makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile

