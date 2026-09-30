/* rom: 0xf6f8 len: 0x260 func: f_f6f8 flags: -macsave=1 -optimize=1 -speed */
/* Script ops 0xf6f8-0xf958 (same 16.16 fixmul as vm_ef04 / board_28334).
 * f_f776 needs p1's low half written as (v & 0xffff): it makes SHC CSE
 * (int)u and (int)v, which leaves u and v as spilled shorts like the ROM.
 * f_2443c's 7th parameter is short (the ROM re-sign-extends o->x34 for p3),
 * and p2 = f_f6f8 shares one pool literal between the two calls.
 * f_f8ca and f_f928 schedule like the ROM only after the real f_f776. */
struct slot {
	char pad0[26];
	unsigned short x1a;        /* 0x1a */
	char pad1[36 - 28];
	long x24;                  /* 0x24 */
	long x28;                  /* 0x28 */
	char pad2[0x80 - 0x2c];
};
struct obj {
	char pad0[0x1a];
	unsigned short x1a;        /* 0x1a */
	short x1c;                 /* 0x1c */
	short x1e;
	short x20;                 /* 0x20 */
	char pad1[0x30 - 0x22];
	short x30;                 /* 0x30 */
	char pad2[0x34 - 0x32];
	short x34;                 /* 0x34 */
};
struct vm {
	char pad0[0x20];
	struct obj *obj;           /* 0x20 */
	char pad1[0x30 - 0x24];
	short x30;                 /* 0x30 */
	short x32;                 /* 0x32 */
	char pad2[0x48 - 0x34];
	short x48;                 /* 0x48 */
	char pad3[0x50 - 0x4a];
	short x50;
	short x52;                 /* 0x52 */
	char pad4[0x80 - 0x54];
	long regs[32];             /* 0x80 */
	struct slot sl[1];         /* 0x100 */
};
struct script {
	short x0, x2, x4, x6, cnt;
};
struct spr {
	char pad0[0x2c];
	unsigned short x2c;
	char pad1[0x40 - 0x2e];
};
extern struct vm *g_606005c;
extern long *g_6060060;
extern struct script *g_6060064;
extern struct spr g_606006c[];
extern long g_6033440[];
extern void f_f6f8(void);
extern void f_243be(struct vm *, long *);
extern short f_2bd48(long, long);
extern long f_2be48(long);
extern long f_2be88(long);
extern void f_253bc(short, short, short, long);
extern short f_2443c(void (*)(void), long, long, long, long, long, short, long);
extern void f_2418e(struct vm *, void (*)(void), long, long, long);

#pragma inline_asm(fixmul)
static long fixmul(long a, long b)
{
	DMULS.L R4,R5
	STS MACH,R4
	STS MACL,R0
	XTRCT R4,R0
}

void f_f6f8(void)
{
	struct script *s = g_6060064;
	short x = s->x0;
	short y = s->x2;
	short n = s->x4;
	short d = s->x6;
	struct slot *p;
	long r1, r2;
	r1 = (x << 16) / d;
	r2 = (y << 16) / d;
	p = (struct slot *)((long)g_606005c + 0x100 + n * 0x80);
	p->x24 = r1;
	p->x28 = r2;
	if (--s->cnt < 0) {
		p->x24 = 0;
		p->x28 = 0;
		p->x1a &= 0xfffd;
		f_243be(g_606005c, g_6060060);
	}
}

void f_f776(unsigned long a)
{
	long A, B, C, D, E, F;
	struct obj *o;
	long x0, y0;
	short u, v;
	long D2, E2;
	long p0, p1, p3;
	void (*p2)(void);

	A = g_606005c->regs[a];
	B = g_606005c->regs[a + 1];
	C = g_606005c->regs[a + 2];
	D = g_606005c->regs[a + 3];
	E = g_606005c->regs[a + 4];
	F = g_606005c->regs[a + 5];
	o = g_606005c->obj;
	o->x1a |= 2;
	x0 = o->x1c;
	y0 = o->x20;
	D2 = D + (short)(fixmul(f_2be48(A), B << 16) >> 16);
	E2 = E + (short)(fixmul(f_2be88(A), C << 16) >> 16);
	u = D2 - x0;
	v = E2 - y0;
	o->x30 = f_2bd48(u, v);
	p1 = (u << 16) | (v & 0xffff);
	p3 = (o->x34 << 16) | (unsigned long)(unsigned short)F;
	p0 = (unsigned short)F << 16;
	f_2443c(p2 = f_f6f8, 16, 0, 0, 0, 0, o->x34, 0);
	f_2418e(g_606005c, p2, p1, p3, p0);
}


void f_f8be(void)
{
}

void f_f8c2(void)
{
}

void f_f8c6(void)
{
}

void f_f8ca(unsigned long a)
{
	short n;
	f_253bc(g_606005c->x48, g_606005c->x30, g_606005c->x32, *(long *)g_606005c->regs[a]);
	n = g_606005c->x48;
	g_606006c[n].x2c &= 0x7fff;
}

void f_f918(void)
{
}

void f_f91c(void)
{
}

void f_f920(void)
{
}

void f_f924(void)
{
}

long f_f928(short i)
{
	return g_6033440[i];
}
