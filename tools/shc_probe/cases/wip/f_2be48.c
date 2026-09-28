/* rom: 0x2be48 len: 0x110 func: f_2be48 flags: -macsave=1 -optimize=1 -speed */
/* NOT FINAL: f_2be48 and f_2be88 are 100%, f_2beca is 65/68 (95.6%).
 * Continues the angle file that starts at 0x2bc64 (final_2bc64.c).
 * f_2be48 and f_2be88 look up the quarter-wave table g_60356e0 with the
 * usual quadrant symmetry; f_2be88 is f_2be48 a quarter turn (0x40) ahead.
 * f_2beca steps the seed at g_60b13c0 (seed * 7 + g_6060008, times g_606000c
 * when g_606487c is clear), rotates its bytes, and returns
 * fixmul(seed, f_2be88(byte 3)) % n.
 * fixmul is a #pragma inline_asm helper (dmuls.l/xtrct 16.16 multiply); the
 * ROM shows SHC's inline_asm signature: a literal pool dumped and branched
 * over right before the asm body. inline_asm needs -code=asmcode plus asmsh,
 * so this only compiles through agent_b/shcc_asm.sh (score it with
 * `python agent_b/fsa.py funcscore|probe ...`).
 * Remaining diff in f_2beca: in `s *= g_606000c` the ROM loads the address
 * into r3 and the value into r2; this build swaps them. */
extern long g_60356e0[];

long f_2be48(int a)
{
	a = (unsigned char)a;
	switch ((unsigned short)a >> 6) {
	case 0:
		return g_60356e0[(short)a];
	case 1:
		return g_60356e0[0x80 - (short)a];
	case 2:
		return -g_60356e0[(short)a - 0x80];
	default:
		return -g_60356e0[0x100 - (short)a];
	}
}

long f_2be88(int a)
{
	a = (unsigned char)a;
	switch ((unsigned short)a >> 6) {
	case 0:
		return g_60356e0[0x40 - (short)a];
	case 1:
		return -g_60356e0[(short)a - 0x40];
	case 2:
		return -g_60356e0[0xc0 - (short)a];
	default:
		return g_60356e0[(short)a - 0xc0];
	}
}

#pragma inline_asm(fixmul)
static long fixmul(long a, long b)
{
	DMULS.L R4,R5
	STS MACH,R4
	STS MACL,R0
	XTRCT R4,R0
}

union seed {
	unsigned long l;
	unsigned char b[4];
};
extern union seed g_60b13c0;
extern long g_6060008;
extern long g_606487c;
extern long g_606000c;

unsigned long f_2beca(unsigned long n)
{
	unsigned char t;
	unsigned long s;

	s = g_60b13c0.l * 7 + g_6060008;
	if (g_606487c == 0)
		s *= g_606000c;
	g_60b13c0.l = s;
	t = g_60b13c0.b[3];
	g_60b13c0.b[3] = g_60b13c0.b[1];
	g_60b13c0.b[1] = g_60b13c0.b[2];
	g_60b13c0.b[2] = g_60b13c0.b[0];
	g_60b13c0.b[0] = t;
	return fixmul(g_60b13c0.l, f_2be88(g_60b13c0.b[3])) % n;
}
