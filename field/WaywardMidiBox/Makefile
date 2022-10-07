# Project Name
TARGET = WaywardMidiBox

# Sources
CPP_SOURCES = WaywardMidiBox.cpp DisplayUtils.cpp MidiBoxBankParams.cpp

# Library Locations
LIBDAISY_DIR = ../../libDaisy
DAISYSP_DIR = ../../DaisySP
LDFLAGS += -u _printf_float

#DEBUG = 1

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile
