# Runs on nuada (sh-elf binutils, wibo and SHC live there). See README.md.
PY ?= /drive2/tgm2p/venv/bin/python

.PHONY: all split build check clean
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

clean:
	rm -rf asm build/obj build/bin build/roms build/rom.ld build/tgm2p.elf build/tgm2p.bin
