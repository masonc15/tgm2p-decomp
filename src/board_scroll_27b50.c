/* rom: 0x27b50 len: 0x320 func: f_27b50 context-rom: 0x27b50 flags: -macsave=1 -optimize=1 -speed */
/* Complete per-frame board scroll and attribute update.
 * Keep raw_hi separate from its packed successor, and attr_mid as a short:
 * their distinct lifetimes and conversions reproduce the original allocation.
 * Standard C register hints apply to values, never to physical registers.
 * Decrement the loop count after advancing the five real array pointers. */
struct board {
	char pad0[6];
	short sel;                 /* 0x06 */
	char pad8[2];
	short w0a;                 /* 0x0a */
	unsigned short w0c;        /* 0x0c */
	unsigned short w0e;        /* 0x0e */
	char pad10[0x18 - 0x10];
	short set[28];             /* 0x18 */
	long a[224];               /* 0x050 */
	unsigned long x[224];      /* 0x3d0 */
	long b[224];               /* 0x750 */
	unsigned short c[224];     /* 0xad0 */
	short d[224];              /* 0xc90 */
};
struct pal {
	char pad0[0xc];
	unsigned short w0c[3];     /* 0x0c */
	char pad12[0x18 - 0x12];
	long l18[2];               /* 0x18 */
	long l20[13];              /* 0x20 */
	short n;                   /* 0x54 */
};
extern struct pal g_60ad228[];
extern unsigned short d_3b724[];
extern short g_60b13a6;
extern short g_60b13aa, g_60b13ac;

void f_27b50(struct board *input)
{
	register unsigned short *pc;
	register long *pb;
	register unsigned long *px;
	register long *pa;
	register unsigned short *tbl;
	register long base;
	register int i;
	register unsigned long attr;
	struct board *bd;
	struct pal *q;
	short k;
	long hi, v;
	register long raw_hi;
	register unsigned short attr_mid;

	pa = (bd = input)->a;
	px = bd->x;
	pb = bd->b;
	pc = bd->c;
	tbl = d_3b724;
	q = (struct pal *)((char *)g_60ad228 + (short)(bd->set[bd->sel] * 104));
	k = q->n;
	base = q->l20[k];
	raw_hi = q->l18[k];
	attr_mid = bd->w0e;
	attr = (bd->w0a << 24) | (bd->w0c << 15) | (attr_mid << 8) | q->w0c[k];
	if (g_60b13aa) {
		g_60b13ac += 32;
		if (g_60b13ac > 0x1ff00)
			g_60b13ac = -0x100;
	}
	hi = (raw_hi & 0x3ff) << 16;
	for (i = 224; i > 0; pa++, pb++, px++, pc++, tbl++, i--) {
		*px = attr;
		v = *tbl;
		if (g_60b13aa)
			v += g_60b13ac;
		*pb += v;
		if (g_60b13a6 && (*pc & 0x3f00) < 0x3f00) {
			*pc += *tbl;
			if (*pc >= 0x3f00)
				*pc = 0x3f00;
		}
		*px |= (*pc & 0x3f00) << 8;
		*pa = (base - ((*pb >> 8) & 0x1ff)) & 0x3ff;
		*pa |= hi;
	}
}
