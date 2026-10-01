// Ghidra headless pre-script, run after SetupMemory.java: map the code image
// at the address it runs from. Boot copies ROM 0x780-0x313fc to RAM
// 0x06000000, so every call and function pointer names RAM (ROM + 0x5fff880).
// This replaces the start of SetupMemory's uninitialized "ram" block with an
// initialized, read-only "ram_code" block backed by the same file bytes, and
// marks the ROM copy of that range non-executable so analysis doesn't make a
// second set of functions there.
//@category tgm2p
import ghidra.app.script.GhidraScript;
import ghidra.program.database.mem.FileBytes;
import ghidra.program.model.address.Address;
import ghidra.program.model.mem.Memory;
import ghidra.program.model.mem.MemoryBlock;

public class MapRamCode extends GhidraScript {
	static final long ROM_START = 0x780L, ROM_END = 0x313fcL, RAM_BASE = 0x06000000L;

	@Override
	public void run() throws Exception {
		Memory mem = currentProgram.getMemory();
		long size = ROM_END - ROM_START;
		Address ramCode = toAddr(RAM_BASE);
		if (mem.getBlock(ramCode) != null && mem.getBlock(ramCode).isInitialized()) {
			println("ram_code already mapped");
			return;
		}

		MemoryBlock ram = mem.getBlock(ramCode);
		if (ram != null) {
			Address tail = toAddr(RAM_BASE + size);
			mem.split(ram, tail);
			mem.removeBlock(mem.getBlock(ramCode), monitor);
			MemoryBlock rest = mem.getBlock(tail);
			rest.setName("ram");
			rest.setExecute(false);
			rest.setComment("work RAM after the code image");
		}

		FileBytes fb = mem.getAllFileBytes().get(0);
		MemoryBlock code = mem.createInitializedBlock("ram_code", ramCode, fb, ROM_START, size, false);
		code.setRead(true);
		code.setWrite(false); // read-only so the decompiler folds literal-pool constants
		code.setExecute(true);
		code.setComment("ROM 0x780-0x313fc as copied to RAM at boot (RAM = ROM + 0x5fff880)");

		MemoryBlock rom = mem.getBlock(toAddr(0));
		mem.split(rom, toAddr(ROM_START));
		MemoryBlock image = mem.getBlock(toAddr(ROM_START));
		mem.split(image, toAddr(ROM_END));
		image = mem.getBlock(toAddr(ROM_START));
		image.setName("rom_image");
		image.setExecute(false);
		image.setComment("load image of ram_code; analyze at 0x06000000 instead");
		MemoryBlock data = mem.getBlock(toAddr(ROM_END));
		data.setName("rom_data");
		data.setExecute(false);
		data.setComment("RAM-copy table at 0x313fc, then data");
		println("mapped ROM 0x780-0x313fc at RAM 0x06000000");
	}
}
