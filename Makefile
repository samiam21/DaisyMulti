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
CPP_SOURCES = src/DaisyMulti.cpp src/DaisyDisplay.cpp $(wildcard lib/DaisyEffects/*.cpp) $(wildcard lib/Inputs/*.cpp) $(wildcard lib/Helpers/*.cpp)

# Library Locations
LIBDAISY_DIR = lib/libdaisy
DAISYSP_DIR = lib/DaisySP

# Linker flags
# This is not really required, used only for profiling! Increases executable size by ~8kB
LDFLAGS = -u _printf_float

# Core location, and generic makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile

