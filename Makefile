ARES_BIN := /Applications/ares.app/Contents/MacOS/ares

BUILD_DIR := build
SOURCE_DIR := src
PROG_NAME := sblobber64
RELEASE_ROM_NAME := sblobber64-release
BUILD_TYPE ?= debug

N64_MK_PATH := $(N64_INST)/include/n64.mk
ifneq (,$(wildcard $(N64_MK_PATH)))
include $(N64_MK_PATH)
endif

N64_CFLAGS += -Iinclude -I$(N64_INST)/include -Wall -Werror
ifeq ($(BUILD_TYPE),release)
N64_CFLAGS += -DNDEBUG
else
N64_CFLAGS += -O0 -g
endif

.PHONY: all build docker gengfx docker-gengfx rebuild release setup resetup ares ares-release cen64 clean help

all: build

build: ##    Create rom.
	@if command -v docker >/dev/null 2>&1; then \
		echo "Building rom inside docker environment..."; \
		$(MAKE) docker; \
	else \
		echo "Building rom..."; \
		$(MAKE) $(PROG_NAME).z64; \
	fi

gengfx:##   Generate UI gfx.
	@if command -v docker >/dev/null 2>&1; then \
		echo "Generating gfx inside docker environment..."; \
		$(MAKE) docker-gengfx; \
	else \
		echo "Generating gfx..."; \
		./resources/gfx.sh; \
	fi

docker-gengfx: setup
	@docker run --rm --platform linux/amd64 --user $(shell id -u):$(shell id -g) -e GOCACHE=/tmp/go-cache -v ${CURDIR}:/game build ./resources/gfx.sh

docker: setup
	@docker run --rm --platform linux/amd64 --user $(shell id -u):$(shell id -g) -e GOCACHE=/tmp/go-cache -v ${CURDIR}:/game build make BUILD_TYPE=$(BUILD_TYPE) $(PROG_NAME).z64

rebuild: clean build	##  Erase temp files and create rom.

release: clean ##  Create release rom.
	@$(MAKE) BUILD_TYPE=release build
	@mv -f $(PROG_NAME).z64 $(RELEASE_ROM_NAME).z64
	@echo "    [RELEASE] $(RELEASE_ROM_NAME).z64"

# gfx #
PNGS := $(wildcard resources/gfx/*/*.png) $(wildcard resources/gfx/*/*/*.png)
SPRITES := $(subst .png,.sprite,$(subst resources/,filesystem/,$(PNGS)))
filesystem/gfx/%.sprite: resources/gfx/%.png
	@mkdir -p $(dir $@)
	@echo "    [SPRITE] $@"
	@$(N64_MKSPRITE) -f RGBA16 -o $(dir $@) $<

# sfx #
OGGS := $(wildcard resources/sfx/*.ogg)
SOUNDS := $(subst .ogg,.wav64,$(subst resources/,filesystem/,$(OGGS)))
$(BUILD_DIR)/sfx/ogg/%.wav: resources/sfx/%.ogg
	@mkdir -p $(dir $@)
	sox $< -b 16 -e signed-integer -L -r 44100 $@ remix -

filesystem/sfx/%.wav64: $(BUILD_DIR)/sfx/ogg/%.wav
	@mkdir -p $(dir $@)
	@echo "    [WAV64] $@"
	@$(N64_AUDIOCONV) --wav-compress 1 -o $(dir $@) $<

# sfx #
MODS := $(wildcard resources/sfx/*.mod)
MUSICS := $(subst .mod,.wav64,$(subst resources/,filesystem/,$(MODS)))
$(BUILD_DIR)/sfx/mod/%.raw: resources/sfx/%.mod
	@mkdir -p $(dir $@)
	openmpt123 --quiet --batch --stdout --samplerate 44100 --channels 2 --no-float $< > $@

$(BUILD_DIR)/sfx/mod/%.wav: $(BUILD_DIR)/sfx/mod/%.raw
	sox -t raw -e signed-integer -b 16 -c 2 -r 44100 $< $@

$(MUSICS): filesystem/sfx/%.wav64: $(BUILD_DIR)/sfx/mod/%.wav
	@mkdir -p $(dir $@)
	@echo "    [WAV64] $@"
	@$(N64_AUDIOCONV) --wav-compress 1 -o $(dir $@) $<

# maps #
TMXS := $(wildcard resources/maps/*.tmx)
MAPS := $(subst .tmx,.map,$(subst resources/,filesystem/,$(TMXS)))
filesystem/maps/%.map: resources/maps/%.tmx
	@mkdir -p $(dir $@)
	go run tools/map_translator/main.go -i $< -o $@

# code #
SRCS := $(wildcard src/*.c)
OBJS := $(SRCS:$(SOURCE_DIR)/%.c=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

-include $(DEPS)

$(PROG_NAME).z64: N64_ROM_TITLE = "sblobber64"

$(BUILD_DIR)/$(PROG_NAME).elf: $(OBJS)
$(PROG_NAME).z64: $(BUILD_DIR)/$(PROG_NAME).dfs

# dfs #
$(BUILD_DIR)/$(PROG_NAME).dfs: $(SPRITES) $(SOUNDS) $(MUSICS) $(MAPS)
	@mkdir -p ./filesystem/
	@echo `git rev-parse HEAD` > ./filesystem/hash
	@echo "    [DFS] $@"
	@$(N64_MKDFS) $@ ./filesystem/ >/dev/null

setup:		##    Create dev environment (docker image).
	@docker build --platform linux/amd64 -t build - < Dockerfile

resetup:	##  Force recreate the dev environment (docker image).
	@echo "Rebuilding dev environment in docker..."
	@docker build --platform linux/amd64 -t build --no-cache - < Dockerfile

ares: build		##    Start rom in Ares emulator.
	@echo "Starting ares..."
	@mkdir -p .ares/saves
	$(ARES_BIN) --setting "Paths/Saves=$(CURDIR)/.ares/saves" $(PROG_NAME).z64

ares-release: release	## Start release rom in Ares emulator.
	@echo "Starting ares (release)..."
	@mkdir -p .ares/saves
	$(ARES_BIN) --setting "Paths/Saves=$(CURDIR)/.ares/saves" $(RELEASE_ROM_NAME).z64

cen64:		##    Start rom in CEN64 emulator.
	@echo "Starting cen64..."
	$(CEN64_DIR)/cen64 -multithread -is-viewer -controller num=1,pak=rumble $(CEN64_DIR)/pifdata.bin $(PROG_NAME).z64

clean:		##    Cleanup temp files.
	@echo "Cleaning up temp files..."
	rm -rf $(BUILD_DIR) *.z64 *.elf src/*.o *.bin *.dfs ./filesystem .ares

help:		##     Show this help.
	@fgrep -h "##" $(MAKEFILE_LIST) | fgrep -v fgrep | sed -e 's/:.*##/:/' 
