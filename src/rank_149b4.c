/* rom: 0x149b4 len: 0x15c func: f_149b4 flags: -macsave=1 -optimize=1 -speed */
/* Rank/state screen file, first span 0x149b4-0x14b10 (f_149b4, f_14a08 and their shared pool).
 * f_14b10 onward (same source file, calls f_14a08 by bsr) is still asm.
 * g_60b161c/g_60b1620 must be arrays and g_60b1630 a scalar for f_149b4's registers. */
struct field { char pad[0x3b4]; };
extern char g_60b161c[], g_60b1620[], g_60b1630;
extern unsigned char g_6064758, g_6064759;
extern long g_6064750, g_606487c, g_6065648;
extern unsigned long g_6060008;
extern char g_607953c;
extern short g_6066184;
extern struct field g_6064898[2];
extern int f_2da90(int, int);
extern void f_29f94(unsigned long, unsigned long, unsigned char, unsigned char);
extern void f_2b1cc(void);
extern void f_247b0(long);
extern void f_23f10(void);
extern void f_2b31e(void);
extern void f_2ab56(unsigned short);
extern void f_2491c(void);
extern void f_1715c(void);
extern void f_2eeaa(int);
extern void f_1278(void);
extern void f_2d06(struct field *);
extern int f_2b4e8(void);
extern void f_2b196(void);

short f_149b4(void)
{
	if (g_60b161c[0] || g_60b1620[0] || g_60b1630 || (f_2da90(0, 0) && (g_6064758 & 15)) || (f_2da90(1, 0) && (g_6064759 & 15)))
		return 1;
	return 0;
}

void f_14a08(void)
{
	f_29f94(0, 0, 0, 255);
	*(char *)0x2405ffe8 = 19;
	*(char *)0x2405ffe9 = 102;
	f_2b1cc();
	if (g_6064750)
		f_247b0(g_6064750);
	f_23f10();
	f_2b31e();
	f_2ab56(500);
	f_2491c();
	f_1715c();
	f_2eeaa(0);
	f_1278();
	f_2d06(&g_6064898[0]);
	f_2d06(&g_6064898[1]);
	{
		long z = 0;
		char c;

		g_6064750 = z;
		g_606487c = z;
		g_6060008 = z;
		g_607953c = c = z;	/* the char copy gives the ROM's second zero register */
		g_6065648 = c;
		g_6066184 = z;
	}
	f_2b4e8();
	f_2b196();
}
