/* rom: 0xe834 len: 0x154 func: f_e834 flags: -macsave=1 -optimize=1 -speed */
struct obj {
	char pad0[0x2c];
	long x2c;
	char pad1[0x31 - 0x30];
	unsigned char b31;
};
struct vm {
	char pad0[0x20];
	struct obj *obj;           /* 0x20 */
	char pad1[0x30 - 0x24];
	short x30;                 /* 0x30 */
	short x32;                 /* 0x32 */
	char pad2[0x52 - 0x34];
	short x52;                 /* 0x52 */
	short x54;                 /* 0x54 */
	char pad3[0x7c - 0x56];
	void *argp;                /* 0x7c */
	long regs[1];              /* 0x80 */
};
extern struct vm *g_606005c;
extern short f_2bc64(long, long);
extern short f_2bd48(long, long);

void f_e834(unsigned long a)
{
	long v;
	unsigned short d = a >> 16;

	g_606005c->argp = &v;
	g_606005c->regs[d] = v;
}

void f_e854(unsigned long a)
{
	long v[2];
	unsigned short d = a >> 16;

	g_606005c->argp = v;
	g_606005c->regs[d] = v[0];
	g_606005c->regs[d + 1] = v[1];
}

void f_e882(unsigned long a)
{
	struct { long p0, p1; unsigned short d; } s;
	short r;

	s.d = a >> 16;
	g_606005c->argp = &s;
	r = f_2bc64(s.p0, s.p1);
	g_606005c->regs[s.d] = r;
}

void f_e8b8(unsigned long a)
{
	struct { long p0, p1; unsigned short d; } s;
	short r;

	s.d = a >> 16;
	g_606005c->argp = &s;
	r = f_2bd48(s.p0, s.p1);
	g_606005c->regs[s.d] = r;
}

void f_e8ee(unsigned long a)
{
	long d = g_606005c->x52;
	long v = (g_606005c->x54 << 10) / d;

	g_606005c->regs[a] = v;
}

void f_e91a(void)
{
}

void f_e91e(unsigned long a)
{
	g_606005c->regs[a] = g_606005c->obj->b31;
}

void f_e938(unsigned long a)
{
	g_606005c->regs[a] = g_606005c->obj->x2c >> 12;
}

void f_e966(void)
{
}

void f_e96a(void)
{
}

void f_e96e(void)
{
}

void f_e972(void)
{
}
