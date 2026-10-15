/* rom: 0x27e90 len: 0x4a4 func: f_27e90 context-rom: 0x27e70 local-loop-jumps: 1 flags: -macsave=1 -optimize=1 -speed */
/* Matching wave functions 0x27e90-0x28334. The preceding helpers and genuine
 * following function provide excluded context. Existing fixmul is unchanged. */
struct board {
	char pad0[6];
	short sel;                 /* 0x06 */
	char pad8[2];
	short w0a;                 /* 0x0a */
	unsigned short w0c;        /* 0x0c */
	unsigned short w0e;        /* 0x0e */
	char pad10[0x18 - 0x10];
	short set[16];             /* 0x18 */
	short w38;                 /* 0x38 */
	short w3a;
	unsigned char b3c;
	unsigned char b3d;
	short w3e;
	char pad40[0x4c - 0x40];
	long w4c;                  /* 0x4c */
	long a[224];               /* 0x050 */
	unsigned long x[224];      /* 0x3d0 */
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
extern short g_60b13a6, g_60b13aa;
extern long d_3b9e4, d_3b9e8, d_3b9ec, d_3b9f0, d_3b9f4, d_3b9f8;
extern unsigned char d_3b8e4[];
extern long f_2be48(int);
extern long f_2be88(int);

#pragma inline_asm(fixmul)
static long fixmul(long a, long b)
{
	DMULS.L R4,R5
	STS MACH,R4
	STS MACL,R0
	XTRCT R4,R0
}

void f_27e70(int on)
{
	g_60b13a6 = on ? 1 : 0;
}

void f_27e80(int on)
{
	g_60b13aa = on ? 1 : 0;
}

void f_27e90(struct board *bd)
{
	struct pal *q;
	short k;
	unsigned long attr;
	union { long l; short w; } s, c;
	long ang;
	int i;

	q = (struct pal *)((char *)g_60ad228 + (short)(bd->set[bd->sel] * 104));
	k = q->n;
	attr = (bd->w0a << 24) | (bd->w0c << 15) | (bd->w0e << 8) | q->w0c[k];
	ang = d_3b9e4;
	for (i = 0; i < 224; i++) {
		ang = (unsigned char)ang;
		c.l = fixmul(f_2be88(ang), 0xf0000);
		s.l = fixmul(f_2be48(ang), 0x1f0000);
		bd->x[i] = attr;
		bd->a[i] = ((q->l18[k] - c.w) & 0x3ff) << 16 |
		           ((q->l20[k] - s.w - 64) & 0x3ff);
		ang += 3;
	}
	d_3b9e4 += 3;
	if (0) { d_3b9e4 = ang; }
	d_3b9e8 += d_3b9ec;
	if (d_3b9e8 > 63)
		d_3b9ec = -1;
	else if (d_3b9e8 < 1)
		d_3b9ec = 1;
	d_3b9e4 = (unsigned char)d_3b9e4;
}
void f_2808a(struct board *bd)
{
	struct pal *q;
	short k;
	union { long l; short w; } c, s;
	register long ang;
	int sh;
	int j;
	register int h;
	register int n;
	register int half;
	register unsigned long attr;
	register int w;
	register int i;
	register int m;

	w = *(short *)((char *)bd + 56);
	q = (struct pal *)((char *)g_60ad228 + (short)(bd->set[bd->sel] * 104));
	k = q->n;
	n = 224;
	half = 112;
	attr = (bd->w0a << 24) | (bd->w0c << 15) | (bd->w0e << 8) | q->w0c[k];
	sh = d_3b9f0;
	ang = (unsigned char)sh;
	j = half;
	m = half - 1;
	for (i = 0; i < half; i++) {
		c.l = fixmul(f_2be48((unsigned char)((i >> 2) + 64)), 0x400000);
		h = c.w;
		h = -h;
		h += 64;
		h = h < 0 ? 0 : h;
		bd->x[j] = bd->x[m] = (h << 16) | attr;
		c.l = fixmul(f_2be48((unsigned char)(ang + i)), 0x100000);
		s.l = fixmul(f_2be48((unsigned char)(ang - i)), 0x100000);
		bd->a[j] = (q->l20[k] - d_3b8e4[h] - c.w) & 0x3ff;
		bd->a[m] = (q->l20[k] - d_3b8e4[h] - s.w) & 0x3ff;
		sh = bd->w4c + i;
		if (sh > 223)
			sh -= n;
		bd->a[j] |= ((q->l18[k] - sh + j) & 0x3ff) << 16;
		bd->a[m] |= ((q->l18[k] - sh + m) & 0x3ff) << 16;
		j++;
		m--;
	}
	bd->w4c -= w;
	if (bd->w4c >= n)
		bd->w4c -= n;
	if (bd->w4c < 0)
		bd->w4c += n;
	d_3b9f0 += d_3b9f8;
	if (d_3b9f0 > 0xff) {
		if (d_3b9f8 == 4)
			d_3b9f4 = -1;
		else if (d_3b9f8 == 1)
			d_3b9f4 = 1;
		d_3b9f8 += d_3b9f4;
		d_3b9f0 = (unsigned char)d_3b9f0;
	}
}

/* Genuine following function establishes the original wave boundary. */
void f_28334(struct board *bd)
{
	struct pal *q;
	short k;
	unsigned long attr;
	union { long l; short w; unsigned char b[4]; } c;
	short i;
	short t;
	long amp;
	long amp2;

	q = (struct pal *)((char *)g_60ad228 + (short)(bd->set[bd->sel] * 104));
	k = q->n;
	attr = (bd->w0a << 24) | (bd->w0c << 15) | (bd->w0e << 8) | q->w0c[k];
	amp = bd->w3a << 12;
	
	if (amp) {
		for (i = 0; i < 224; i++) {
			t = (unsigned char)(*((unsigned char *)&bd->w38 + 1) + i);
			c.l = fixmul(f_2be48(t), amp);
			bd->a[i] = (q->l20[k] + c.w) & 0x1ff;
			bd->x[i] = attr;
		}
	} else {
		for (i = 0; i < 224; i++) {
			bd->a[i] = q->l20[k] & 0x1ff;
			bd->x[i] = attr;
		}
	}
	
	amp2 = bd->w3e << 12;
	if (amp2) {
		for (i = 0; i < 224; i++) {
			t = (unsigned char)(bd->b3d + i);
			c.l = fixmul(f_2be48(t), amp2);
			if (q->l18[k] + i + c.w < 0 || q->l18[k] + i + c.w > 223)
				c.w = 0;
			bd->a[i] |= (unsigned char)(c.w + q->l18[k]) << 16;
		}
	} else {
		for (i = 0; i < 224; i++)
			bd->a[i] |= (unsigned char)q->l18[k] << 16;
	}
}

