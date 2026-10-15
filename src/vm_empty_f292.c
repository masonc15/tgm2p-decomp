/* rom: 0xf200 len: 0x9a func: f_f200 context-rom: 0xf200 literal-tail-calls: 1 leaf-argument-tails: 1 flags: -macsave=1 -optimize=1 -speed */
/* Retained ranges: f200-f29a and f4c6-f5cc. The fifteen callbacks
 * at f4dc-f514 are actual empty ROM callbacks. All other routines
 * are genuine unlinked context, including the reconstructed f3a2. */
/* The two argument tails preserve the original register ABI. f_2c6fc
 * consumes low16 of the long coordinate/mode arguments; its short result
 * is ignored by this void VM handler. */
struct obj {
	char pad0[0x1a];
	short x1a;                 /* 0x1a */
	char pad1[0x31 - 0x1c];
	unsigned char b31;         /* 0x31 */
	char pad2[0x34 - 0x32];
	short x34;                 /* 0x34 */
};
struct vm {
	char pad0[0x20];
	struct obj *obj;           /* 0x20 */
	char pad1[0x80 - 0x24];
	long regs[1];              /* 0x80 */
};
struct spr {
	char pad0[0x18];
	short x18;                 /* 0x18 */
	char pad1[0x1c - 0x1a];
	unsigned char b1c;         /* 0x1c */
	char pad2[0x22 - 0x1d];
	short x22;                 /* 0x22 */
	char pad3[0x40 - 0x24];
};
extern struct vm *g_606005c;
extern struct spr g_606006c[];
extern long g_6060004;
extern void f_2c3ca(long);
extern short f_2ce54(long, long, long, long);
extern short f_2c042(long, long, long);
extern void f_2443c(void (*)(void), long, long, long, long, long, long, long);
extern void f_2418e(struct vm *, void (*)(void), long, long, long);
extern void f_29c50(long);
extern short f_2cf62(long, long, long);
extern void f_2c6fc(long *, long, long, long);

/* Genuine unlinked preceding compiler context begins here. */
void f_f200(unsigned long a)
{
	register long *tbl = (long *)g_606005c->regs[a];
	short y = g_606005c->regs[a + 1];
	short x = g_606005c->regs[a + 2];
	register short k = g_606005c->regs[a + 3];
	register long *e;
	register long v0;
	long v3;
	register short spr;

	tbl += y * 21;
	e = (long *)tbl[x];
	v0 = e[k];
	v3 = e[k + 3];

	if (g_6060004) {
		f_2c3ca(15);
		spr = f_2ce54(v3, 65, 15, 0);
	} else
		spr = f_2c042(v0, 20, 68);
	{
		short zero = 0;
		struct spr *s = &g_606006c[spr];

		s->x18 = zero;
		s->x22 = zero;
	}
}

void f_f292(void)
{
}

void f_f296(void)
{
}

void f_f29a(long a)
{
	register short n = a;
	register short t = (a & 0xff0000) >> 16;
	struct obj *o = g_606005c->obj;
	short cur = o->b31;
	short dir = 0;
	short d;
	long p0, p1, p3;
	void (*p2)(void);

	if (n > 0 && cur != t) {
		o->x1a |= 2;
		d = (t == 0 ? t | 0x100 : (unsigned char)t) - cur;
		if (d < 0) {
			dir = 1;
			d = d < 0 ? -d : d;
		} else if (d > 0x80) {
			if ((d < 0 ? -d : d) < 0x100) {
				dir = 1;
				d = 0xff - d;
			}
		}
		d = (unsigned char)d;
		p1 = ((unsigned char)d << 8) | (n << 16) | cur;
		p0 = ((d << 8) / n) << 16;
		p3 = (o->x34 << 16) | dir;
		f_2443c(p2 = f_f296, 16, 0, 0, 0, 0, o->x34, 0);
		f_2418e(g_606005c, p2, p1, p0, p3);
	}
}

void f_f3a2(unsigned long a)
{
 long v = g_606005c->regs[a];
 register short n = v;
 short t = (v & 0xffff0000) >> 16;
 register struct obj *o = g_606005c->obj;
 short dir = 0;
 short cur;
 short d;
 long p0, p1, p3;
 void (*p2)(void);
 o->x1a |= 2;
 if (t == 0) d = t | 0x100;
 else d = (unsigned char)t;
 cur = g_606005c->obj->b31;
 if (n <= 0 || cur == d) {
  o->x1a &= 0xfffd;
  return;
 }
 d -= cur;
 if (d < 0) {
  dir = 1;
  d = d < 0 ? -d : d;
 } else if (d > 0x80) {
  if ((d < 0 ? -d : d) < 0x100) {
   dir = 1;
   d = (unsigned char)(0xff - d);
  }
 }
 d = (unsigned char)d;
 p1 = ((unsigned char)d << 8) | (n << 16) | cur;
 p0 = ((d << 8) / n) << 16;
 p3 = (o->x34 << 16) | dir;
 f_2443c(p2 = f_f296, 16, 0, 0, 0, 0, o->x34, 0);
 f_2418e(g_606005c, p2, p1, p0, p3);
}

void f_f4c6(unsigned long a)
{
	f_29c50(g_606005c->regs[a]);
}

/* Verified retained range begins here: 0xf4dc-0xf574. */
void f_f4dc(void)
{
}

void f_f4e0(void)
{
}

void f_f4e4(void)
{
}

void f_f4e8(void)
{
}

void f_f4ec(void)
{
}

void f_f4f0(void)
{
}

void f_f4f4(void)
{
}

void f_f4f8(void)
{
}

void f_f4fc(void)
{
}

void f_f500(void)
{
}

void f_f504(void)
{
}

void f_f508(void)
{
}

void f_f50c(void)
{
}

void f_f510(void)
{
}

void f_f514(void)
{
}

void f_f518(unsigned long a)
{
	short x = g_606005c->regs[1];
	short y = g_606005c->regs[2];
	short spr = f_2cf62(g_606005c->regs[3], x, y);

	g_606006c[spr].b1c = (g_606006c[spr].b1c & 0x8f) | (a << 4);
	g_606006c[spr].x18 = 2;
	g_606006c[spr].x22 = 0;
}

/* Genuine unlinked following compiler context begins here. */
void f_f574(unsigned long a)
{
	long x = g_606005c->regs[a + 1];
	long y = g_606005c->regs[a + 2];

	f_2c6fc(&g_606005c->regs[a], x, y, 2);
}
