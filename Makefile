# Runs on nuada (sh-elf binutils, wibo and SHC live there). See README.md.
PY ?= /drive2/tgm2p/venv/bin/python
OBJDIFF ?= /drive2/tgm2p/bin/objdiff-cli

.PHONY: all split build check report clean
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

clean:
	rm -rf asm build/obj build/bin build/roms build/rom.ld build/tgm2p.elf build/tgm2p.bin build/report build/report.json objdiff.json
