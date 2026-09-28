/* rom: 0xf5cc len: 0x12c func: f_f5cc flags: -macsave=1 -optimize=1 -speed */
struct vm {
	struct vm *parent;         /* 0x00 */
	char pad0[0x48 - 0x04];
	short x48;                 /* 0x48 */
	char pad1[0x52 - 0x4a];
	short x52;                 /* 0x52 */
	short x54;                 /* 0x54 */
	char pad2[0x58 - 0x56];
	short x58;                 /* 0x58 */
	char pad3[0x80 - 0x5a];
	long regs[1];              /* 0x80 */
};
struct spr {
	char pad0[0x2a];
	unsigned char b2a;         /* 0x2a */
	char pad1[0x34 - 0x2b];
	short x34;                 /* 0x34 */
	short x36;                 /* 0x36 */
	char pad2[0x40 - 0x38];
};
extern struct vm *g_606005c;
extern struct spr g_606006c[];
extern short g_6079370;
extern void f_2c658(short *, long, long, long);

void f_f5cc(unsigned long a)
{
	short x = g_606005c->regs[a];
	long y = g_606005c->regs[a + 1];

	g_6079370 = g_606005c->x52 - g_606005c->x54;
	f_2c658(&g_6079370, x, y, 2);
}

void f_f604(short a)
{
	g_606005c->x58 = a;
}

void f_f60e(void)
{
}

void f_f612(long a)
{
	struct vm *p = g_606005c->parent;

	if (p) {
		struct spr *t = g_606006c;
		struct spr *s;
		long v;
		a += t[p->x48].b2a;
		v = a;
		s = &t[g_606005c->x48];
		s->b2a = v;
	}
}

void f_f648(long a)
{
	/* the ROM keeps the index in a stack slot; volatile reproduces that */
	volatile short i;
	long v = a + g_606006c[i = g_606005c->x48].b2a;

	g_606006c[i].b2a = v;
}

void f_f678(unsigned long a)
{
	long v = g_606005c->regs[a];

	g_606006c[g_606005c->x48].x34 = v + 1;
	g_606006c[g_606005c->x48].x36 = v;
}

void f_f6b2(unsigned long a)
{
	long v = g_606005c->regs[a];
	short i = g_606005c->x48;

	g_606006c[i].b2a = v;
}

void f_f6d4(void)
{
}

void f_f6d8(void)
{
}

void f_f6dc(void)
{
}
