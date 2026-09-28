/* rom: 0x2b4e8 len: 0x77c func: f_2b4e8 flags: -macsave=1 -optimize=1 -speed */
/* Four variants of the per-frame driver. Each runs a list of subsystem
 * updates, then busy-waits for vblank while counting idle loops in
 * g_606000c (irq_vblank in frt.c sets g_6060030 and bumps g_606002c).
 * f_2ba08 swaps the bytes at *a and *b into g_6064754/g_6064755 (and the
 * copies at g_606475a) for the first three updates, then restores them;
 * g_606475e gets the bits newly set since the previous call.
 * Each returns 1 when the top-level state isn't 2, g_6060020 is clear and
 * bit 5 of the input byte at 0x03000003 is low; MAME's psikyosh driver maps
 * that bit to the service switch (active low).
 * Matching notes: `while (g_60b13bc)` and `|| g_6060020` must not spell out
 * `!= 0`, or the temp registers rotate differently. */
#define IRQ_ACK   (*(unsigned char *)0x2405ffdd)  /* cache-through 0x0405ffdd */
#define IN_SYS    (*(volatile unsigned char *)0x23000003)  /* cache-through 0x03000003 */

extern volatile long g_6060030;
extern volatile long g_606002c;
extern void f_2d2e0(void);
extern void f_2521e(void);
extern void f_23f5e(void);
extern void f_2a92(void);
extern void f_17672(void);
extern void f_2c0a(void);
extern void f_249c4(void);
extern void f_10de6(void);
extern void f_17446(void);
extern void f_2cd42(void);
extern void f_251cc(void);
extern void f_257f8(void);
extern void f_259bc(void);
extern void f_2e254(void);
extern void f_2c1f0(void);
extern void f_29f24(void);
extern void f_2b1e4(void);
extern void f_2a0d4(void);

extern short g_6060002;
extern long g_6060008;
extern long g_606000c;
extern short g_6060020;
extern short g_6060022;
extern unsigned char g_6064754;
extern unsigned char g_6064755;
extern unsigned char g_606475a[2];
extern unsigned char g_606475e[2];
extern unsigned char g_60b13be;
extern unsigned char g_60b13bf;
extern short g_60618ec;
extern long g_6065644;
extern long g_6065648;
extern short g_60b13bc;

int f_2b4e8(void)
{
	g_6060002 = g_6060008 & 1;
	f_2d2e0();
	f_2521e();
	f_23f5e();
	f_249c4();
	f_17672();
	f_257f8();
	f_259bc();
	f_17446();
	f_2cd42();
	f_251cc();
	f_2e254();
	g_60618ec = 2;
	while (g_60b13bc) {
		f_2d2e0();
		g_606000c = 0;
		g_606002c = 0;
		while (IRQ_ACK & 1)
			g_606000c++;
		g_6060030 = 0;
		while (g_6060030 == 0)
			g_606000c++;
		g_6060030 = 0;
		if (g_6064754 & 1) {
			f_2a92();
			break;
		}
	}
	f_2c1f0();
	g_606000c = 0;
	while (IRQ_ACK & 1)
		g_606000c++;
	g_6060030 = 0;
	while (g_6060030 == 0)
		g_606000c++;
	f_29f24();
	f_2b1e4();
	f_2a0d4();
	g_6060030 = 0;
	IRQ_ACK |= 1;
	g_6060008++;
	g_6065644++;
	g_6065648++;
	if (!(g_6060022 == 2 || g_6060020) && !(IN_SYS & 0x20))
		return 1;
	return 0;
}

int f_2b64a(void)
{
	g_6060002 = g_6060008 & 1;
	f_2d2e0();
	f_2521e();
	f_2a92();
	f_257f8();
	f_259bc();
	f_2cd42();
	f_251cc();
	f_2e254();
	g_60618ec = 2;
	while (g_60b13bc) {
		f_2d2e0();
		g_606000c = 0;
		g_606002c = 0;
		while (IRQ_ACK & 1)
			g_606000c++;
		g_6060030 = 0;
		while (g_6060030 == 0)
			g_606000c++;
		g_6060030 = 0;
		if (g_6064754 & 1) {
			f_2a92();
			break;
		}
	}
	f_2c1f0();
	g_606000c = 0;
	while (IRQ_ACK & 1)
		g_606000c++;
	g_6060030 = 0;
	while (g_6060030 == 0)
		g_606000c++;
	f_29f24();
	f_2b1e4();
	f_2a0d4();
	g_6060030 = 0;
	IRQ_ACK |= 1;
	g_6060008++;
	g_6065644++;
	g_6065648++;
	if (!(g_6060022 == 2 || g_6060020) && !(IN_SYS & 0x20))
		return 1;
	return 0;
}

int f_2b816(void)
{
	f_2d2e0();
	g_6060002 = g_6060008 & 1;
	f_2521e();
	f_17672();
	f_2a92();
	f_2c0a();
	f_23f5e();
	f_249c4();
	f_10de6();
	f_17446();
	f_2cd42();
	f_251cc();
	f_257f8();
	g_60618ec = 2;
	g_6060008++;
	g_6065644++;
	g_6065648++;
	f_259bc();
	f_2e254();
	while (g_60b13bc) {
		f_2d2e0();
		g_606000c = 0;
		g_606002c = 0;
		while (IRQ_ACK & 1)
			g_606000c++;
		g_6060030 = 0;
		while (g_6060030 == 0)
			g_606000c++;
		g_6060030 = 0;
		if (g_6064754 & 1) {
			f_2a92();
			break;
		}
	}
	f_2c1f0();
	g_606000c = 0;
	while (IRQ_ACK & 1)
		g_606000c++;
	g_6060030 = 0;
	while (g_6060030 == 0)
		g_606000c++;
	f_29f24();
	f_2b1e4();
	f_2a0d4();
	g_6060030 = 0;
	IRQ_ACK |= 1;
	if (!(g_6060022 == 2 || g_6060020) && !(IN_SYS & 0x20))
		return 1;
	return 0;
}

int f_2ba08(unsigned char *a, unsigned char *b)
{
	unsigned char s0, s1;

	f_2d2e0();
	g_6060002 = g_6060008 & 1;
	s0 = g_6064754;
	s1 = g_6064755;
	g_6064754 = *a;
	g_6064755 = *b;
	g_606475a[0] = *a;
	g_606475a[1] = *b;
	g_606475e[0] = ~g_60b13be & g_606475a[0];
	g_606475e[1] = g_606475a[1] & ~g_60b13bf;
	g_60b13be = g_606475a[0];
	g_60b13bf = g_606475a[1];
	f_2521e();
	f_23f5e();
	f_2a92();
	g_6064754 = s0;
	g_6064755 = s1;
	g_606475a[0] = s0;
	g_606475a[1] = s1;
	f_17672();
	f_2c0a();
	f_249c4();
	f_10de6();
	f_17446();
	f_2cd42();
	f_251cc();
	f_257f8();
	g_60618ec = 2;
	g_6060008++;
	g_6065644++;
	g_6065648++;
	f_259bc();
	f_2e254();
	while (g_60b13bc) {
		f_2d2e0();
		g_606000c = 0;
		g_606002c = 0;
		while (IRQ_ACK & 1)
			g_606000c++;
		g_6060030 = 0;
		while (g_6060030 == 0)
			g_606000c++;
		g_6060030 = 0;
		if (g_6064754 & 1) {
			f_2a92();
			break;
		}
	}
	f_2c1f0();
	g_606000c = 0;
	while (IRQ_ACK & 1)
		g_606000c++;
	g_6060030 = 0;
	while (g_6060030 == 0)
		g_606000c++;
	f_29f24();
	f_2b1e4();
	f_2a0d4();
	g_6060030 = 0;
	IRQ_ACK |= 1;
	if (!(g_6060022 == 2 || g_6060020) && !(IN_SYS & 0x20))
		return 1;
	return 0;
}
