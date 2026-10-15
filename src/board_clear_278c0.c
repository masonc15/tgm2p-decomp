/* rom: 0x278c0 len: 0x290 func: f_278c0 context-rom: 0x276e8 flags: -macsave=1 -optimize=1 -speed */
/* Retain only the complete board clear278c0–27b50. The genuine callback,
 * setters, toggle and cache-update bodies supply context and are excluded.
 * The cache-update index is scaled once and consumed by both real accesses.
 * Historical standard C register qualifiers in the clear loop are preserved. */
struct field {
	short w0;
	short w2;
	short w4;
	short w6;
	short w8;
	short w10;
	short w12;
	short w14;
	short w16;
	char pad18[2];
	long l20;
	short a24[11];
	char pad46[2];
	void (*cb48)();
	void (*cb52)();
	short a56[4];
	long l64;
	long l68;
	short w72;
	char pad74[2];
	long l76;
	long x80[224];
	long x976[224];
	char pad1872[0xe50 - 1872];
};
struct anim {
	long flags;
	short w4;
	short w6;
};
struct animent {
	struct anim *a;
	long n;
};
struct rec104 {
	short w0;
	char pad2[2];
	long *l4[2];
	short w12[2];
	struct animent *ae;
	long l20;
	long l24[2];
	long l32[2];
	long l40[2];
	short w48[2];
	short w52[2];
	short w56[2];
	short w60[2];
	short w64[2];
	short w68;
	short w70[2];
	short w74;
	long l76;
	long l80;
	short w84;
	short w86;
	short w88;
	short w90[2];
	char pad94[2];
	long l96;
	long l100;
};
struct cell {
	void (*fn)();
	long a4, a8, a12, a16, a20;
};
struct qent {
	long fn;
	long pad[3];
};
extern struct field g_60ad6a0[4];
extern long f_f928(int);
extern char *g_606005c;
extern short g_60b13b0, g_60b13b2, g_60b13ae;
struct tsk {
	char pad0[8];
	short w8;
	char pad10[6];
	short w16;
};
#define REC(i) ((struct field *)((char *)g_60ad6a0 + (short)((i) * (short)sizeof(struct field))))
extern struct rec104 g_60ad228[11];
extern struct cell g_60b0fe0[2][20];
extern short g_60b13a4;
extern short g_60b13a0[2];
extern unsigned short g_6060000;
extern unsigned short g_606434c;
extern struct qent g_6064350[];
void f_259ea(void);
void f_25806(void);
short f_26098(void);

void f_26e18(short, struct animent *, struct rec104 *, short, short, short, unsigned long, long);
void f_26cb0(short, struct rec104 *, short, short, short);
struct animent *f_2722c(short, short, short *, short *);
void f_26ff0(short, short, short);
void f_26f64(short, int, long *);
void f_274c0(short, short, short, long, long, long);
void f_27310();


void f_276e8(short i, register int a, short b)
{
	a = (short)a;
	if (a == 0)
		g_60ad6a0[i].cb48 = 0;
	else
		g_60ad6a0[i].cb48 = (void (*)())f_f928(a);
	{
		int second = b;
		if (second == 0)
			g_60ad6a0[i].cb52 = 0;
		else
			g_60ad6a0[i].cb52 = (void (*)())f_f928(second);
	}
}

void f_2774a(short i, short v)
{
	g_60ad6a0[i].w2 = v;
}

void f_2775c(short i, short v)
{
	g_60ad6a0[i].w10 = v;
}

void f_2776e(short i, short v)
{
	g_60ad6a0[i].w6 = v;
}

void f_27780(short i, short v)
{
	g_60ad6a0[i].w4 = v;
}

void f_27792(short i, short k, short v)
{
	g_60ad6a0[i].a56[k] = v;
}

void f_277ac(short i, short v)
{
	g_60ad6a0[i].w14 = v;
}

void f_277be(short i, short v)
{
	g_60ad6a0[i].w12 = v;
}

void f_277d0(struct tsk *t)
{
	int id;
	t->w16 = (t->w16 + 1) & 1;
	if (t->w16) { if (t->w8 == 0) { id = f_26098(); t->w8 = id; return; } g_60ad228[t->w8].w0 = 0; id = f_26098(); t->w8 = id; return; }
	if (t->w8) { g_60ad228[t->w8].w0 = 0; t->w8 = 0; }
}

void f_27862(int idx)
{
	short s;
	long *p;

	idx *= 4;
	s = *(short *)(g_606005c + 82);
	g_60b13b0 = (unsigned long)*(long *)(g_606005c + 128 + idx) >> 16;
	p = (long *)(g_606005c + 128 + idx);
	g_60b13b2 = *p;
	g_60b13ae = 1;
	f_26f64(s, 0, p);
	g_60b13ae = 0;
}

struct game {
	char pad[0x52];
	short player;              /* 0x52 */
};
struct board {
	char pad0[0x50];
	long a[224];               /* 0x050 */
	long x[224];               /* 0x3d0 */
	long b[224];               /* 0x750 */
	short c[224];              /* 0xad0 */
	short d[224];              /* 0xc90 */
};
extern short g_60b13a6, g_60b13a8, g_60b13aa, g_60b13ac;


void f_278c0(void)
{
	struct board *p;
	struct game *g;
	short slot;
	register long *pa, *pb;
	register short *pc, *pd;
	register unsigned int i;

	g = (struct game *)g_606005c;
	slot = g->player;
	p = (struct board *)((char *)g_60ad6a0 + (short)(slot * 0xe50));
	pa = p->a;
	pb = p->b;
	pc = p->c;
	pd = p->d;
	g_60b13ae = 0;
	g_60b13b0 = 0;
	g_60b13b2 = 0;
	g_60b13a6 = 0;
	g_60b13a8 = 0;
	g_60b13aa = 0;
	g_60b13ac = 0;
	for (i = 0; i < 224; i += 8) {
		*pa++ = 0;
		*pa++ = 0;
		*pa++ = 0;
		*pa++ = 0;
		*pa++ = 0;
		*pa++ = 0;
		*pa++ = 0;
		*pa++ = 0;
		*pb = 0; pb++;
		*pb++ = 0;
		*pb++ = 0;
		*pb++ = 0;
		*pb++ = 0;
		*pb++ = 0;
		*pb++ = 0;
		*pb++ = 0;
		*pc++ = 0;
		*pc++ = 0;
		*pc++ = 0;
		*pc++ = 0;
		*pc++ = 0;
		*pc++ = 0;
		*pc++ = 0;
		*pc++ = 0;
		*pd++ = 0;
		*pd++ = 0;
		*pd++ = 0;
		*pd++ = 0;
		*pd++ = 0;
		*pd++ = 0;
		*pd++ = 0;
		*pd++ = 0;
	}
}
