/* rom: 0xef04 len: 0x1e4 func: f_ef04 flags: -macsave=1 -optimize=1 -speed */
struct obj {
	char pad0[0x10];
	long x10;                  /* 0x10 */
	char pad1[0x16 - 0x14];
	short x16;                 /* 0x16 */
	char pad2[0x1c - 0x18];
	long x1c;                  /* 0x1c */
	char pad3[0x2c - 0x20];
	long x2c;                  /* 0x2c */
	char pad4[0x31 - 0x30];
	unsigned char b31;         /* 0x31 */
};
struct vm {
	char pad0[0x20];
	struct obj *obj;           /* 0x20 */
	char pad1[0x30 - 0x24];
	short x30;                 /* 0x30 */
	short x32;                 /* 0x32 */
	char pad2[0x48 - 0x34];
	short x48;                 /* 0x48 */
	char pad3[0x52 - 0x4a];
	short x52;                 /* 0x52 */
	short x54;                 /* 0x54 */
	char pad4[0x7c - 0x56];
	void *argp;                /* 0x7c */
	long regs[1];              /* 0x80 */
};
struct spr {
	char pad0[0x14];
	unsigned char b14;         /* 0x14 */
	unsigned char b15;         /* 0x15 */
	char pad1[0x18 - 0x16];
	short x18;                 /* 0x18 */
	short x1a;                 /* 0x1a */
	unsigned char b1c;         /* 0x1c */
	char pad2[0x22 - 0x1d];
	short x22;                 /* 0x22 */
	char pad3[0x2a - 0x24];
	unsigned char b2a;         /* 0x2a */
	char pad4[0x40 - 0x2b];
};
struct e50 {
	char pad0[6];
	short n;                   /* 0x6 */
	char pad1[0x18 - 8];
	short list[1];             /* 0x18 */
	char pad2[0xe50 - 0x1a];
};
struct s68 {
	char pad0[0x28];
	long v[1];                 /* 0x28 */
	char pad1[0x54 - 0x2c];
	short n;                   /* 0x54 */
	char pad2[0x68 - 0x56];
};
extern struct vm *g_606005c;
extern struct spr g_606006c[];
extern long g_60775a8[];
extern struct { unsigned char b; char pad[63]; } g_6060096[];  /* g_606006c[].b2a */
extern struct e50 g_60ad6a0[];
extern struct s68 g_60ad228[];
extern long f_2be88(int);
extern short f_2c186(long);
extern void f_2c3ca(int);
extern short f_2bfca(long, long, long);

#pragma inline_asm(fixmul)
static long fixmul(long a, long b)
{
	DMULS.L R4,R5
	STS MACH,R4
	STS MACL,R0
	XTRCT R4,R0
}

void f_ef04(unsigned long a)
{
	struct obj *o = g_606005c->obj;
	long *r = &g_606005c->regs[a & 0xff];

	*r += (a & 0xff00) >> 8;
	o->x1c += fixmul(f_2be88((unsigned char)*r), (a & 0xffff0000) >> 4);
}

void f_ef7e(unsigned long a)
{
	short x = g_606005c->regs[1];
	short y = g_606005c->regs[2];
	short f = g_606005c->regs[4];
	short w = f_2c186(g_606005c->regs[0]);
	short spr;

	f_2c3ca(15);
	switch (g_606005c->regs[3]) {
	case 0:
		spr = f_2bfca(g_606005c->regs[0], x, y);
		break;
	case 1:
		spr = f_2bfca(g_606005c->regs[0], x, y - w);
		break;
	case 2:
		spr = f_2bfca(g_606005c->regs[0], x, y - w / 2);
		break;
	}
	g_606006c[spr].b1c = (g_606006c[spr].b1c & 0x8f) | (a << 4);
	g_606006c[spr].x18 = f & 3;
	g_606006c[spr].x22 = 0;
}

void f_f036(unsigned long a)
{
	long v = g_60775a8[a];
	short i = g_606005c->x48;

	g_606006c[i].b2a = v;
}

void f_f052(void)
{
}

void f_f056(void)
{
}

void f_f05a(void)
{
}

void f_f05e(void)
{
}

void f_f062(void)
{
}

void f_f066(void)
{
}

long f_f06a(unsigned long a)
{
	struct e50 *p = (struct e50 *)((char *)g_60ad6a0 + (short)(g_606005c->x52 * 0xe50));
	struct s68 *q = (struct s68 *)((char *)g_60ad228 + (short)(p->list[p->n] * 104));
	short n = q->n;

	g_606005c->regs[a] = q->v[n];
	return 1;
}

void f_f0b8(void)
{
}
