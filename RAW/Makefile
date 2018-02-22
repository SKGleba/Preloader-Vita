TITLE_ID = SKGM4NU37
TARGET = ManfactFlagger

PLUGIN_OBJS = preloader.o
HEADERS = $(wildcard *.h)

PLUGIN_LIBS = -ltaihen_stub -lSceSysclibForDriver_stub -lSceModulemgrForKernel_stub -lSceIofilemgrForDriver_stub

PREFIX  = arm-vita-eabi
CC      = $(PREFIX)-gcc
CFLAGS  = -Wl,-q -Wall -O3
ASFLAGS = $(CFLAGS)

all: preloader.skprx

preloader.skprx: preloader.velf
	vita-make-fself -c $< $@

preloader.velf: preloader.elf
	vita-elf-create -e exports.yml $< $@

preloader.elf: $(PLUGIN_OBJS)
	$(CC) $(CFLAGS) $^ $(PLUGIN_LIBS) -o $@ -nostdlib


