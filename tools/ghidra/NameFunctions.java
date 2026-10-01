// Ghidra headless post-script: give functions the repo's names. A function
// Ghidra auto-named gets f_<ROM offset> (f_14b10 for RAM 0x6014390), the
// convention src/ and splits.txt use, and each symbols.txt entry becomes a
// label (a function name if a function starts there).
//
// args: [symbols.txt path]
//@category tgm2p
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;
import ghidra.program.model.symbol.SourceType;
import java.nio.file.Files;
import java.nio.file.Paths;

public class NameFunctions extends GhidraScript {
	static final long RAM_BASE = 0x06000000L, RAM_END = 0x06030c7cL, DELTA = 0x5fff880L;

	@Override
	public void run() throws Exception {
		int renamed = 0, labels = 0;
		FunctionIterator it = currentProgram.getFunctionManager().getFunctions(true);
		while (it.hasNext()) {
			Function f = it.next();
			if (f.getSymbol().getSource() != SourceType.DEFAULT) continue;
			long a = f.getEntryPoint().getOffset();
			long rom = (a >= RAM_BASE && a < RAM_END) ? a - DELTA : a;
			f.setName(String.format("f_%x", rom), SourceType.IMPORTED);
			renamed++;
		}

		String[] args = getScriptArgs();
		if (args.length > 0) {
			for (String line : Files.readAllLines(Paths.get(args[0]))) {
				String s = line.replaceAll("#.*", "").trim();
				if (s.isEmpty()) continue;
				String[] p = s.split("\\s+");
				if (p.length < 2) continue;
				Address a = toAddr(Long.decode(p[1]));
				if (currentProgram.getMemory().getBlock(a) == null) continue;
				Function f = getFunctionAt(a);
				if (f != null) f.setName(p[0], SourceType.IMPORTED);
				else createLabel(a, p[0], true, SourceType.IMPORTED);
				labels++;
			}
		}
		println("named " + renamed + " functions, applied " + labels + " symbols");
	}
}
