# Runs on nephele (~/tgm2p) or nuada (/drive2/tgm2p); see tools/toolchain.py.
TGM2P_TOOLCHAIN ?= $(firstword $(wildcard /drive2/tgm2p $(HOME)/tgm2p) /drive2/tgm2p)
PY ?= $(TGM2P_TOOLCHAIN)/venv/bin/python
OBJDIFF ?= $(TGM2P_TOOLCHAIN)/bin/objdiff-cli
SHC_COMPILERS ?= $(TGM2P_TOOLCHAIN)/compilers
export TGM2P_TOOLCHAIN SHC_COMPILERS
export PATH := $(TGM2P_TOOLCHAIN)/bin:$(PATH)

.PHONY: all split build check report status clean
all: check

build/prog.bin:
	$(PY) tools/interleave.py join

split: build/prog.bin
	$(PY) tools/build.py split

build: build/prog.bin
	@test -d asm || $(PY) tools/build.py split
	$(PY) tools/build.py build

check: build/prog.bin
	@test -d asm || $(PY) tools/build.py split
	$(PY) tools/build.py check

report: build/prog.bin
	@test -d asm || $(PY) tools/build.py split
	$(PY) tools/build.py report
	$(OBJDIFF) report generate -o build/report.json

status: build/prog.bin
	@test -d asm || $(PY) tools/build.py split
	@$(PY) tools/frontier.py > /dev/null
	@$(PY) tools/status.py

clean:
	rm -rf asm build/obj build/bin build/roms build/rom.ld build/tgm2p.elf build/tgm2p.bin build/report build/report.json objdiff.json
