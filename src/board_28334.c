/* rom: 0x28334 len: 0x18c4 func: f_28334 flags: -macsave=1 -optimize=1 -speed */
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
	short w3a;                 /* 0x3a */
	unsigned char b3c;         /* 0x3c */
	unsigned char b3d;         /* 0x3d */
	short w3e;                 /* 0x3e */
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
struct boardb {
	char pad0[6];
	short sel;
	char pad8[2];
	short w0a;
	unsigned short w0c;
	unsigned short w0e;
	char pad10[0x18 - 0x10];
	short set[16];
	short wob[4];              /* 0x38 */
	char pad40[0x4c - 0x40];
	long w4c;
	long a[224];
	unsigned long x[224];
};
extern struct pal g_60ad228[];
extern long f_2be48(short);
extern long f_2be88(short);
extern void f_313e4(void *, int, int);
extern long d_3b710, d_3b718, d_3b720;
extern unsigned char d_3b8e4[];
extern unsigned short g_60b13b4;

#pragma inline_asm(fixmul)
static long fixmul(long a, long b)
{
	DMULS.L R4,R5
	STS MACH,R4
	STS MACL,R0
	XTRCT R4,R0
}

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

void f_286e0(struct board *bd)
{
	struct pal *q;
	short k;
	unsigned long attr;
	union { long l; short w; unsigned char b[4]; } c;
	short i;
	short t;
	long amp;
	int y;

	q = (struct pal *)((char *)g_60ad228 + (short)(bd->set[bd->sel] * 104));
	k = q->n;
	attr = (bd->w0a << 24) | q->w0c[k] | (bd->w0e << 8) | (bd->w0c << 15);
	amp = bd->w3a << 12;
	if (0) amp += 1;
	for (i = 0; i < 224; i++) {

		t = (unsigned char)(*((unsigned char *)&bd->w38 + 1) + i);
		c.l = fixmul(f_2be48(t), amp);
		y = q->l18[k] + i + c.w;
		if (y < 0 || y > 223)
			c.w = 0;
		bd->a[i] = ((unsigned char)(c.w + q->l18[k]) << 16) | (q->l20[k] & 0x1ff);
		bd->x[i] = attr;

	}
}

void f_2898e(void)
{
}

void f_28992(struct board *bd0)
{
	struct board *bd;
	struct pal *q;
	short k;
	register unsigned long attr;
	union { long l; short w; unsigned char b[4]; } c;
	long pad[2];
	register int i;
	long buf1[256];
	long buf2[256];

	f_313e4(buf1, 0, 0x400);
	f_313e4(buf2, 0, 0x400);
	bd = bd0;
	q = (struct pal *)((char *)g_60ad228 + (short)(bd->set[bd->sel] * 104));
	k = q->n;
	attr = (bd->w0a << 24) | (bd->w0c << 15) | (bd->w0e << 8) | q->w0c[k];
	for (i = 0; i < 224; i++) {
		c.l = fixmul(f_2be48((unsigned char)(i / 2 + 0x88)), 0x7f0000);
		bd->x[i] = ((unsigned char)(c.w + 0x80) << 16) | attr;
		bd->a[i] = (q->l20[k] - d_3b8e4[(unsigned char)(c.w + 0x80)]) & 0x3ff;
		d_3b720 = 1;
	}
	d_3b710 = d_3b710 & 0x1ff;
	if (d_3b710 > 0xff && d_3b710 < 0x200)
		d_3b718 = (unsigned char)(0xff - d_3b710);
	else
		d_3b718 = (unsigned char)d_3b710;
	d_3b710 += d_3b720;
	d_3b720 += 1;
}

void f_28b88(struct board *bd)
{
	struct pal *q;
	short k;
	register unsigned long attr;
	int s;
	union { long l; short w; unsigned char b[4]; } c;
	register int i;

	q = (struct pal *)((char *)g_60ad228 + (short)(bd->set[bd->sel] * 104));
	k = q->n;
	s = (unsigned char)(*(short *)((char *)bd + 0x38) >> 8);
	attr = (bd->w0a << 24) | (bd->w0c << 15) | (bd->w0e << 8) | q->w0c[k];

	for (i = 0; i < 224; i++) {
		c.l = fixmul(f_2be48((unsigned char)(i / 2 + 0x88)), s << 16);
		bd->x[i] = ((unsigned char)(c.w + 0x80) << 16) | attr;
		bd->a[i] = (q->l20[k] - d_3b8e4[(unsigned char)(c.w + 0x80)]) & 0x3ff;
		bd->a[i] |= (q->l18[k] & 0x3ff) << 16;
	}
}

void f_28d66(struct board *bd)
{
	struct pal *q;
	short k;
	register unsigned long attr;
	long amp;
	union { long l[2]; short s[4]; } c;
	short *w;
	register unsigned i;
	
	unsigned ph;

	q = (struct pal *)((char *)g_60ad228 + (short)(bd->set[bd->sel] * 104));
	w = &bd->w38;
	k = q->n;
	ph = *w >> 8;
	amp = w[1] >> 8;
	attr = (bd->w0a << 24) | (bd->w0c << 15) | (bd->w0e << 8) | q->w0c[k];
	for (i = 0; (int)i < 224; i += 2) {
		c.l[1] = fixmul(f_2be48((unsigned char)((unsigned)ph + i)), amp << 16);
		c.l[0] = fixmul(f_2be88((unsigned char)((int)i + (int)ph)), amp << 16);
		bd->x[i] = attr;
		bd->a[i] = (q->l20[k] - c.s[2]) & 0x3ff;
		bd->a[i] |= ((q->l18[k] - c.s[0]) & 0x3ff) << 16;
		bd->x[0xdf - i] = attr;
		bd->a[0xdf - i] = (q->l20[k] - c.s[2]) & 0x3ff;
		bd->a[0xdf - i] |= ((q->l18[k] - c.s[0]) & 0x3ff) << 16;
	}
}

void f_28fe8(struct board *bd)
{
	struct pal *q;
	short k;
	register unsigned long attr;
	long amp;
	union { long l[2]; short s[4]; } c;
	short *w;
	register int i;
	int ph;
	int sc;

	q = (struct pal *)((char *)g_60ad228 + (short)(bd->set[bd->sel] * 104));
	w = &bd->w38;
	k = q->n;
	ph = *w >> 8;
	amp = w[1] >> 8;
	sc = w[2];
	attr = (bd->w0a << 24) | (bd->w0c << 15) | (bd->w0e << 8) | q->w0c[k];
	for (i = 0; i < 224; i += 2) {
		c.l[1] = fixmul(f_2be48((unsigned char)((i / 2 * sc >> 9) + ph)), amp << 16);
		bd->x[i] = attr;
		bd->a[i] = (q->l20[k] - c.s[2]) & 0x3ff;
		bd->a[i] |= ((q->l18[k] - c.s[2]) & 0x3ff) << 16;
		bd->x[0xdf - i] = attr;
		bd->a[0xdf - i] = (q->l20[k] - c.s[2]) & 0x3ff;
		bd->a[0xdf - i] |= ((q->l18[k] - c.s[2]) & 0x3ff) << 16;
	}
}

void f_2923a(struct board *bd)
{
	struct pal *q;
	short k;
	register unsigned long attr;
	long amp;
	union { long l[2]; short s[4]; } c;
	short *w;
	register int i;
	int ph;
	int sc;

	q = (struct pal *)((char *)g_60ad228 + (short)(bd->set[bd->sel] * 104));
	w = &bd->w38;
	k = q->n;
	ph = *w >> 8;
	amp = w[1] >> 8;
	sc = w[2];
	attr = (bd->w0a << 24) | (bd->w0c << 15) | q->w0c[k];
	for (i = 0; i < 224; i += 2) {
		c.l[1] = fixmul(f_2be48((unsigned char)((i / 2 * sc >> 9) + ph)), amp << 16);
		bd->x[i] = (((bd->w0e + 48) & 63) << 8) | attr;
		bd->a[i] = (q->l20[k] - c.s[2]) & 0x3ff;
		bd->a[i] |= ((q->l18[k] - c.s[2]) & 0x3ff) << 16;
		bd->x[0xdf - i] = (((bd->w0e + 52) & 63) << 8) | attr;
		bd->a[0xdf - i] = (q->l20[k] + c.s[2]) & 0x3ff;
		bd->a[0xdf - i] |= ((q->l18[k] + c.s[2]) & 0x3ff) << 16;
	}
}

void f_294b6(struct boardb *bd0)
{
	struct boardb *bd;
	struct pal *q;
	short k;
	unsigned long attr;
	long amp;
	int ph;
	int off;
	long pad[1];
	union { long l; short w; unsigned char b[4]; } c1;
	union { long l; short w; unsigned char b[4]; } c2;
	 unsigned i;

	bd = bd0;
	q = (struct pal *)((char *)g_60ad228 + (short)(bd->set[bd->sel] * 104));
	k = q->n;
	ph = bd->wob[0] >> 8;
	amp = bd->wob[1] >> 8;
	off = bd->wob[2] >> 8;
	attr = (bd->w0a << 24) | (bd->w0c << 15) | (bd->w0e << 8) | q->w0c[k];
	for (i = 0; i < 224; i++) {
		c1.l = fixmul(f_2be88(ph + i), amp << 16);
		c2.l = fixmul(f_2be48((i >> 1) + 0x88), 0xff0000);
		bd->x[i] = ((c2.w + 0xff) << 16) | attr;
		bd->a[i] = (q->l20[k] & 0x3ff) | (((q->l18[k] + off - c1.w) & 0x3ff) << 16);
	}
}

void f_296b6(struct boardb *bd)
{
	struct pal *q;
	short k;
	unsigned long attr;
	long pad[1];
	union { long l; short w; unsigned char b[4]; } c2;
	union { long l; short w; unsigned char b[4]; } c1;
	unsigned i;

	q = (struct pal *)((char *)g_60ad228 + (short)(bd->set[bd->sel] * 104));
	k = q->n;
	attr = (bd->w0a << 24) | (bd->w0c << 15) | (bd->w0e << 8) | q->w0c[k];
	for (i = 0; i < 224; i++) {
		c1.l = fixmul(f_2be48(i / 2 + 0x88), 0x3f0000);
		bd->x[i] = ((unsigned char)(c1.w + 63) << 16) | attr;
		if (!bd->wob[0]) {
			c2.l = fixmul(f_2be48((unsigned char)((g_60b13b4 >> 1) + i)), 0x200000);
			bd->a[i] = ((q->l20[k] - d_3b8e4[(unsigned char)(c1.w + 63)]) & 0x3ff) | (((q->l18[k] - c2.w) & 0x3ff) << 16);
		} else {
			c2.l = fixmul(f_2be48((unsigned char)((g_60b13b4 >> 1) - i)), 0x200000);
			bd->a[i] = ((q->l20[k] - d_3b8e4[(unsigned char)(c1.w + 63)]) & 0x3ff) | (((q->l18[k] - c2.w) & 0x3ff) << 16);
		}
	}
	g_60b13b4++;
	if (0) c1.w += 0;
}

