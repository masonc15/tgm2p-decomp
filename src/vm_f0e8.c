/* rom: 0xf0e8 len: 0x118 func: f_f0e8 flags: -macsave=1 -optimize=1 -speed */
struct vm {
	char pad0[0x30];
	short x30;                 /* 0x30 */
	short x32;                 /* 0x32 */
	char pad2[0x52 - 0x34];
	short x52;                 /* 0x52 */
	char pad3[0x80 - 0x54];
	long regs[1];              /* 0x80 */
};
extern struct vm *g_606005c;
extern unsigned char g_6064758, g_6064759;
extern void f_2733a(short, long, long, long);

void f_f0e8(unsigned long a)
{
	if ((short)(a >> 16) & 0x8000) {
	}
}

void f_f0f2(void)
{
}

void f_f0f6(void)
{
}

void f_f0fa(void)
{
}

void f_f0fe(unsigned long a)
{
	long dx = (a >> 16) - g_606005c->x30;
	long dy = (unsigned short)a - g_606005c->x32;

	if ((dx < 0 ? -dx : dx) <= 32 && (dy < 0 ? -dy : dy) <= 32)
		g_606005c->regs[0] = 1;
	else
		g_606005c->regs[0] = 0;
}

void f_f146(void)
{
}

void f_f14a(void)
{
}

void f_f14e(void)
{
}

void f_f152(unsigned long a)
{
	g_606005c->regs[a] = 0;
	if (g_6064758 & 14)
		g_606005c->regs[a] = 1;
	if (g_6064759 & 14)
		g_606005c->regs[a] = 1;
}

void f_f194(unsigned long a)
{
	long x = g_606005c->regs[a];
	long y = g_606005c->regs[a + 1];
	long i = g_606005c->regs[a + 2];
	long *t = (long *)g_606005c->regs[a + 3];
	/* t points at 28-byte records whose first word is a table pointer */
	register long *p = (long *)*(long *)((char *)t + i * 28) + (x * 8 + y);

	f_2733a(g_606005c->x52, *p, 0, 0);
}
