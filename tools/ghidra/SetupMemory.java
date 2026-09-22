// Ghidra headless pre-script: lay out the PS5V2 address space around the
// imported program ROM (MAME psikyosh.cpp ps5_map) and seed the entry point.
//@category tgm2p
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.mem.Memory;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.program.model.symbol.SourceType;

public class SetupMemory extends GhidraScript {
	private void block(String name, long start, long size, boolean write, boolean exec, String comment) throws Exception {
		Address a = toAddr(start);
		Memory mem = currentProgram.getMemory();
		if (mem.getBlock(a) != null) return;
		MemoryBlock b = mem.createUninitializedBlock(name, a, size, false);
		b.setRead(true);
		b.setWrite(write);
		b.setExecute(exec);
		b.setVolatile(!exec && name.startsWith("io"));
		b.setComment(comment);
	}

	@Override
	public void run() throws Exception {
		MemoryBlock rom = currentProgram.getMemory().getBlock(toAddr(0));
		rom.setName("rom");
		rom.setWrite(false);
		rom.setExecute(true);
		block("io_inputs", 0x03000000L, 0x8, true, false, "inputs / JP4 / EEPROM (psikyosh io_map)");
		block("io_ymf278b", 0x03100000L, 0x8, true, false, "YMF278B OPL4");
		block("vram", 0x04000000L, 0x10000, true, false, "sprite / background RAM");
		block("palette", 0x04040000L, 0x5000, true, false, "palette");
		block("zoomtbl", 0x04050000L, 0x200, true, false, "sprite zoom lookup");
		block("vidregs", 0x0405FFDCL, 0x24, true, false, "IRQ control + video registers; 0405FFDC = watchdog");
		block("gfxbank", 0x04060000L, 0x20000, false, false, "GFX ROM bank window");
		block("datarom", 0x05000000L, 0x80000, false, false, "data ROM window (unpopulated on tgm2p)");
		block("ram", 0x06000000L, 0x100000, true, true, "work RAM (code is copied here too)");
		createFunction(toAddr(0x400), "reset");
		currentProgram.getSymbolTable().createLabel(toAddr(0x400), "reset", SourceType.USER_DEFINED);
		addEntryPoint(toAddr(0x400));
	}
}
