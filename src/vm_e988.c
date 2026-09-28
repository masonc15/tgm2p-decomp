/* rom: 0xe988 len: 0x148 func: f_e988 flags: -macsave=1 -optimize=1 -speed */
struct vm {
	char pad0[0x30];
	short x30;                 /* 0x30 */
	short x32;                 /* 0x32 */
	char pad2[0x80 - 0x34];
	long regs[1];              /* 0x80 */
};
extern struct vm *g_606005c;
extern unsigned char f_2bd48(long, long);
extern short g_606003a;
extern unsigned char g_60b1819;
extern void f_2ed3e(void);
extern void f_2e5aa(long);
extern void f_2ed06(void);
extern void f_2e6fc(long);

void f_e988(unsigned long a)
{
	unsigned short d = a >> 16;
	short x, y;

	a = (unsigned short)a;
	x = g_606005c->regs[a] - g_606005c->x30;
	y = g_606005c->regs[a + 1] - g_606005c->x32;
	{
		unsigned char s = f_2bd48(x, y);
		g_606005c->regs[d] = s;
	}
}

void f_e9ea(unsigned long a)
{
	unsigned short d = a >> 16;
	short x, y;

	a = (unsigned short)a;
	x = g_606005c->regs[a] - g_606005c->x30;
	y = g_606005c->regs[a + 1] - g_606005c->x32;
	{
		long s = x * x + y * y;
		g_606005c->regs[d] = s;
	}
}

void f_ea36(void)
{
	g_60b1819 = 0;
}

void f_ea3e(void)
{
	if (*(volatile short *)&g_606003a)
		g_60b1819 = 0;
	else
		g_60b1819 = 1;
}

void f_ea56(long a)
{
	if (a == 0) {
		f_2ed3e();
		return;
	}
	if (g_606003a)
		f_2e5aa(a - 1);
}

void f_ea7c(unsigned long a)
{
	if (a == -1) {
		f_2ed06();
		return;
	}
	if (g_606003a)
		f_2e6fc(a);
}

void f_eaa0(void)
{
}

void f_eaa4(void)
{
}

void f_eaa8(void)
{
}
