/* rom: 0x29bf8 len: 0x4dc func: f_29bf8 context-rom: 0x29bf8 literal-tail-calls: 1 flags: -macsave=1 -optimize=1 -speed */
/* Complete matching palette routines, 0x29bf8-0x2a0d4. */
struct task {
	void (*func)();
	long a;
	long b;
	long c;
};
struct fade {
	char pad0[0x1a];
	short mode;                /* 0x1a */
	char pad1[0x20 - 0x1c];
	long color;                /* 0x20 */
	short target;              /* 0x24 */
	short pad2;
	union { long l; short w; } pos;   /* 0x28 */
	union { long l; short w; } spd;   /* 0x2c */
	short keep;                /* 0x30 */
};
extern struct task g_6064350[];
extern unsigned short g_606434c;
extern struct fade *g_6064750;
extern void *g_606005c;
extern char g_60356d0[];
extern struct fade *f_2467e(void *, int);
extern void f_247b0(void *);

void f_29c10(long a, long b, long c);
void f_29c40(long a, long b, long c);
void f_29c76(long a, long b, long c);

void f_29bf8(void)
{
	g_6064350[g_606434c++].func = f_29c10;
}

void f_29c10(long a, long b, long c)
{
	*(unsigned char *)0x2405fffc = 8;
}

void f_29c18(short x)
{
	g_6064350[g_606434c].func = f_29c40;
	g_6064350[g_606434c++].a = x;
}

void f_29c40(long a, long b, long c)
{
	*(unsigned char *)0x2405ffeb = (*(unsigned char *)0x2405ffeb & 0xf8) | (a & 7);
}

void f_29c50(long c)
{
	g_6064350[g_606434c].func = f_29c76;
	g_6064350[g_606434c++].a = c;
}

void f_29c76(long a, long b, long c)
{
	unsigned long i;
	long *p = (long *)0x24004400;

	for (i = 0; i < 0x100; i += 8) {
		*p++ = a; *p++ = a; *p++ = a; *p++ = a; *p++ = a; *p++ = a; *p++ = a; *p++ = a;
	}
}

/* The equivalent condition and inert local branch preserve SHC
 * scratch-register allocation without emitting extra instructions. */
void f_29cc6(short mode, short div, long target_, long a3_)
{
	short target = target_;
	short a3 = a3_;
	if (mode == 4) {
		if (g_6064750) {
			f_247b0(g_6064750);
			g_6064750 = 0;
		}
		return;
	}
	if (g_6064750) ; else g_6064750 = f_2467e(g_60356d0, 0);
	if (1) ; else target = 0;
	if (a3 & 0x8000)
		g_6064750->keep = 1;
	else
		g_6064750->keep = 0;
	a3 &= 7;
	g_6064750->spd.l = 0;
	g_6064750->spd.w = 0x80;
	g_6064750->spd.l /= div;
	f_29bf8();
	if (a3 > 0)
		f_29c18(a3);
	g_6064750->mode = mode;
	switch (mode) {
	case 0:
		g_6064750->color = 0xff;
		if (target)
			g_6064750->target = target;
		else
			g_6064750->target = 0x80;
		g_6064750->spd.l *= -1;
		break;
	case 1:
		g_6064750->color = -1;
		if (target)
			g_6064750->target = target;
		else
			g_6064750->target = 0x80;
		g_6064750->spd.l *= -1;
		break;
	case 2:
		g_6064750->color = 0;
		if (target)
			g_6064750->target = target;
		else
			g_6064750->target = 0;
		break;
	case 3:
		g_6064750->color = 0xffffff00;
		if (target)
			g_6064750->target = target;
		else
			g_6064750->target = 0;
		break;
	}
	g_6064750->pos.l = 0;
	g_6064750->pos.w = g_6064750->target;
	f_29c50(g_6064750->color);
}

/* Palette control prefix continues here. */
void f_29e8c(void)
{
	g_6064750->pos.l += g_6064750->spd.l;
	if (g_6064750->spd.l > 0) {
		if (g_6064750->pos.w >= 0x80) {
			f_247b0(g_606005c);
			return;
		}
	} else if (g_6064750->pos.w <= 0) {
		f_247b0(g_606005c);
		return;
	}
	g_6064750->color = (g_6064750->color & 0xffffff00) | g_6064750->pos.w;
	f_29c50(g_6064750->color);
}

void f_29ede(void)
{
	if (g_6064750->spd.w > 0)
		g_6064750->color |= 0xff;
	else if (g_6064750->keep == 0) {
		f_29c18(0);
		g_6064750->color = 0x111111ff;
	} else
		g_6064750->color &= 0xffffff00;
	f_29c50(g_6064750->color);
	g_6064750 = 0;
}

void f_29f24(void)
{
	register int i;
	struct task *t;

	g_606434c = g_606434c < 32 ? g_606434c : 32;
	for (t = g_6064350, i = 0; i < g_606434c; i++, t++)
		t->func(t->a, t->b, t->c);
	g_606434c = 0;
}

void f_29f70(void)
{
	g_606434c = 0;
}

/* Consumed component and divisor copies preserve SHC register allocation. */
#define SWAP(a, b) { b ^= a; a ^= b; b ^= a; }

void f_29f94(unsigned long c1, unsigned long c2, register unsigned char i0, unsigned char i1)
{
	unsigned char r0;
	unsigned char g0;
	unsigned char b0;
	register short dr, dg, db;
	register unsigned long ar, ag, ab;
	register int n;
	unsigned int i;
	unsigned char g_base;
	unsigned char b_base;
	int divisor;
	unsigned long *pal = (unsigned long *)0x24004000;

	if (i0 > i1) {
		SWAP(i0, i1);
		SWAP(c1, c2);
	}
	n = i1 - i0;
	r0 = c1 >> 24;
	g0 = c1 >> 16;
	b0 = c1 >> 8;
	g_base = g0;
	b_base = b0;
	dr = (c2 >> 24) - r0;
	dg = (unsigned char)(c2 >> 16) - g_base;
	db = (unsigned char)(c2 >> 8) - b_base;
	ag = 0;
	ar = 0;
	ab = 0;
	divisor = n;
	for (i = i0; i < i1; i++)
		pal[i] = ((r0 + (ar += dr) / divisor) << 24 & 0xff000000) | ((g_base + (ag += dg) / divisor) << 16 & 0xff0000) | ((b_base + (ab += db) / divisor) << 8 & 0xff00);
}

struct obj { char pad[4]; short a, b; char pad2[0x150 - 8]; };
extern struct obj g_606194c[];

/* Clear palette object state. */
void f_2a08c(void)
{
	register short i;

	for (i = 0; i < 32; i++) {
		g_606194c[i].b = 0;
		g_606194c[i].a = -1;
	}
}
