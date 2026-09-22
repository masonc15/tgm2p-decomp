// Ghidra headless post-script: dump every function's decompiled C and a
// symbol list so the split/probe tooling can consume them.
//
// analyzeHeadless <proj> tgm2p -import build/prog.bin -processor SuperH:BE:32:SH-2 \
//     -loader BinaryLoader -loader-baseAddr 0x0 -preScript SetupMemory.java \
//     -postScript ExportDecomp.java <outdir>
//@category tgm2p
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;

public class ExportDecomp extends GhidraScript {
	@Override
	public void run() throws Exception {
		String[] args = getScriptArgs();
		File outDir = new File(args.length > 0 ? args[0] : "build/ghidra");
		new File(outDir, "decomp").mkdirs();
		DecompInterface ifc = new DecompInterface();
		ifc.openProgram(currentProgram);
		try (PrintWriter funcs = new PrintWriter(new FileWriter(new File(outDir, "functions.txt")))) {
			FunctionIterator it = currentProgram.getFunctionManager().getFunctions(true);
			int n = 0;
			while (it.hasNext() && !monitor.isCancelled()) {
				Function f = it.next();
				long start = f.getEntryPoint().getOffset();
				long size = f.getBody().getNumAddresses();
				funcs.printf("%08X %6X %s%n", start, size, f.getName());
				DecompileResults res = ifc.decompileFunction(f, 60, monitor);
				String c = res.decompileCompleted() ? res.getDecompiledFunction().getC()
						: "/* decompile failed: " + res.getErrorMessage() + " */\n";
				try (PrintWriter w = new PrintWriter(new FileWriter(
						new File(outDir, String.format("decomp/%08X_%s.c", start, f.getName()))))) {
					w.print(c);
				}
				n++;
			}
			println("exported " + n + " functions to " + outDir);
		} finally {
			ifc.dispose();
		}
	}
}
