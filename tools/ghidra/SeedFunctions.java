// Ghidra headless pre-script: create functions at every address listed in
// the seed file (one hex address per line, from tools/seeds.py or a MAME
// coverage dump) so auto-analysis follows code that is only reached through
// tables or RAM-resident trampolines.
//@category tgm2p
import ghidra.app.cmd.disassemble.DisassembleCommand;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.List;

public class SeedFunctions extends GhidraScript {
	@Override
	public void run() throws Exception {
		String[] args = getScriptArgs();
		List<String> lines = Files.readAllLines(Paths.get(args.length > 0 ? args[0] : "build/seeds.txt"));
		int made = 0;
		for (String line : lines) {
			line = line.trim();
			if (line.isEmpty() || line.startsWith("#")) continue;
			long off = Long.parseLong(line, 16);
			Address a = toAddr(off);
			if (currentProgram.getMemory().getBlock(a) == null) continue;
			if (getFunctionAt(a) != null) continue;
			new DisassembleCommand(a, null, true).applyTo(currentProgram, monitor);
			if (createFunction(a, null) != null) made++;
		}
		println("seeded " + made + " functions from " + lines.size() + " addresses");
	}
}
