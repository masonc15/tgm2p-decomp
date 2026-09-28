/* rom: 0x248a8 len: 0x9e4 func: f_248a8 flags: -macsave=1 -optimize=1 -speed */
/* NOT FINAL, 0x248a8-0x2528c. funcscore 98.7%: every function is 100%
 * except f_251cc (61%, r4/r5/r7 permuted between k, s and the 0x24003800
 * base; the code is otherwise identical). Fixes found this round: f_248fe
 * needs `j = 0xe0;` as its own statement, f_249c4 drops the trailing
 * `break;` of its last case, f_24b68/f_24e9c need `i = n = ...`, ternary
 * sign extension, `x += o->x;` before masking, the struct copy written as
 * `*(struct spr *)(...) = *(struct spr *)&g_6061932;`, and `n = i;
 * f += n - 1; for (i = 0; ...)`, f_2521e is `register int i` with 8
 * stores per iteration (SHC unrolls it by 2). */
struct obj {
	long p;                    /* 0x00 */
	short x;                   /* 0x04 */
	char pad06[2];
	short y;                   /* 0x08 */
	char pad0a[0x14 - 0xa];
	unsigned char a14;         /* 0x14 */
	unsigned char a15;         /* 0x15 */
	short w16;                 /* 0x16 */
	unsigned short w18;        /* 0x18 */
	unsigned short w1a;        /* 0x1a */
	unsigned char b1c;         /* 0x1c */
	char pad1d[0x22 - 0x1d];
	unsigned short w22;        /* 0x22 */
	char pad24[0x28 - 0x24];
	short w28;                 /* 0x28 */
	unsigned char b2a;         /* 0x2a */
	unsigned char b2b;         /* 0x2b */
	unsigned short w2c;        /* 0x2c */
	unsigned char b2e;         /* 0x2e */
	unsigned char b2f;         /* 0x2f */
	short w30;                 /* 0x30 */
	short w32;                 /* 0x32 */
	short w34;                 /* 0x34 */
	short w36;                 /* 0x36 */
	short w38;                 /* 0x38 */
	char pad3a[0x40 - 0x3a];
};
extern short g_60ad224;
extern short g_606106c[];
extern struct obj g_606006c[];
extern long g_60618f0[];
extern short g_60618ec;
extern short g_60611ec[];
extern short g_60610ec[];
extern short g_606193e, g_6061944, g_6061946;
extern unsigned short g_6060000;
extern unsigned short g_6060002;
extern void f_2e18c(int, int);
struct part {
	short x;                   /* 0x0 */
	short y;                   /* 0x2, top 6 bits: parts in this frame */
	unsigned char b4;
	unsigned char b5;
	unsigned char b6;
	unsigned char b7;
	unsigned char b8;
	unsigned char b9;
	short wA;
};
struct spr {
	short x;
	short y;
	unsigned char a4, a5, a6, a7, a8, a9;
	short aA;
	short aC;
	short aE;
};
extern struct spr g_6061932;
extern short g_60356c8[];
void f_24b68(struct obj *o);
void f_24e9c(struct obj *o);

void f_248a8(short id, unsigned short n)
{
	short i;
	short base;

	base = g_60618ec;
	for (i = 0; i < n - 1; i++)
		g_60611ec[base + i] = base + i + 1;
	g_60611ec[base + i] = g_60610ec[id];
	g_60610ec[id] = base;
}

void f_248f0(unsigned char id)
{
	short *p;

	p = &g_60610ec[id];
	*p = 0;
}

void f_248fe(void)
{
	int i, j;

	j = 0xe0;
	for (i = 0; i < 8; i++, j++)
		((char *)0x2405ff00)[j] = 0;
}

void f_2491c(void)
{
	short i;
	struct obj *o;

	f_2e18c(1, 0);
	f_248fe();
	*(char *)0x2405ffe8 = 0x13;
	*(char *)0x2405ffe9 = 0x67;
	o = g_606006c;
	for (i = 0; i < 64; i++, o++) {
		o->b2b = 0;
		o->w2c = 0x8000;
		g_606106c[i] = i;
	}
	g_60ad224 = 0;
	g_606193e = 63;
	g_6061944 = 125;
	g_6061946 = 127;
}

void f_249c4(void)
{
	unsigned short n;
	short *ip;
	struct obj *o;

	for (n = 0, ip = g_606106c; n < g_60ad224; n++, ip++) {
		o = &g_606006c[*ip];
		switch (o->b2b) {
		case 1:
			if (g_6060000 < 30) {
				if (--o->w34 <= 0) {
					o->w34 = o->w36;
					if (o->w2c & 0x800) {
						if (o->w16 > o->w38)
							o->w16--;
						else if (o->w2c & 0x200)
							o->w16 = o->w28 - 1;
						else if (o->w2c & 0x400) {
							o->w16++;
							o->w2c &= 0xf7ff;
						} else {
							o->w16 = o->w38;
							o->b2b = 2;
						}
					} else {
						if (o->w16 < o->w28 - 1)
							o->w16++;
						else if (o->w2c & 0x200)
							o->w16 = o->w38;
						else if (o->w2c & 0x400) {
							o->w16--;
							o->w2c |= 0x800;
						} else {
							o->w16 = o->w28 - 1;
							o->b2b = 2;
						}
					}
				}
			}
		case 2:
			if (!(o->w2c & 0x8000)) {
				if (g_6060002) {
					if ((o->w2c & 0x20) || g_6060000 >= 40)
						f_24b68(o);
				} else {
					if ((o->w2c & 0x10) || g_6060000 >= 40)
						f_24b68(o);
				}
			}
			break;
		case 9:
			if (!(o->w2c & 0x8000))
				f_24e9c(o);
		}
	}
}

void f_24b68(struct obj *o)
{
	struct part *f;
	short i;
	long n;
	short vis;
	long fl2, fl1;
	int x, y;
	short k;
	struct spr *d;

	fl1 = 0;
	fl2 = 0;
	f = (struct part *)o->p;
	for (k = 0; k < o->w16; k++)
		f += (unsigned short)f->y >> 10;
	i = n = (unsigned short)f->y >> 10;
	vis = (o->a14 == 63 && o->a15 == 63) ? 0 : 1;
	f_248a8(o->b2a, n);
	if (o->w30)
		o->w30--;
	if (o->w32 > 0)
		o->w32--;
	else if (o->w32 < 0)
		o->w32++;
	if (!(o->w2c & 0x2000)) {
		if (o->w2c & 0x1000) {
			if (o->w32 == 0) {
				fl1 = 1;
				o->w32 = 8;
			}
			o->w2c &= 0xefff;
		} else if (o->w32 < 0) {
			if (o->w32 & 1)
				fl1 = 1;
		} else if (g_6060000 == 0 && (o->w2c & 3) && o->w30 == 0) {
			fl2 = 1;
			o->w30 = g_60356c8[o->w2c & 3];
		}
	}
	n = i;
	f += n - 1;
	for (i = 0; i < n; i++, f--) {
		if ((f->x & 0x8000) && (o->b2f & (g_6060002 + 1)))
			continue;
		x = f->x;
		x = (x & 0x200) ? (x | 0xfc00) : (x & 0x3ff);
		y = f->y;
		y = (y & 0x200) ? (y | 0xfc00) : (y & 0x3ff);
		if (o->w22 & 0x8000)
			x = -x - (((f->b4 + 1) & 15) << 4);
		if (o->w22 & 0x80)
			y = -y - (((f->b6 + 1) & 15) << 4);
		if (vis) {
		}
		x += o->x;
		y += o->y;
		g_6061932.x = x & 0x3ff;
		g_6061932.y = y & 0x3ff;
		g_6061932.a4 = f->b4;
		g_6061932.a5 = o->a14;
		g_6061932.a6 = (f->b6 & 0xcf) | ((o->w18 << 4) & 0x30);
		if (o->w22 & 0x8000) {
			if (g_6061932.a4 & 0x80)
				g_6061932.a4 &= 0x7f;
			else
				g_6061932.a4 |= 0x80;
		}
		if (o->w22 & 0x80) {
			if (g_6061932.a6 & 0x80)
				g_6061932.a6 &= 0x7f;
			else
				g_6061932.a6 |= 0x80;
		}
		g_6061932.a7 = o->a15;
		if (fl1) {
			if (f->b4 & 0x20)
				g_6061932.a8 = 16;
			else
				g_6061932.a8 = o->w1a ? o->w1a : f->b8;
		} else if (fl2) {
			if (f->b4 & 0x20)
				g_6061932.a8 = 32;
			else
				g_6061932.a8 = o->w1a ? o->w1a : f->b8;
		} else
			g_6061932.a8 = o->w1a ? o->w1a : f->b8;
		g_6061932.a9 = (o->b1c & 0x70) | (f->b9 & 0x8f);
		g_6061932.aA = f->wA;
		*(struct spr *)(0x24000000 + (unsigned short)g_60618ec * 16) = *(struct spr *)&g_6061932;
		g_60618ec++;
	}
}

void f_24e9c(struct obj *o)
{
	struct part *f;
	short i;
	long n;
	short vis;
	long fl2, fl1;
	int x, y;
	short k;
	struct spr *d;

	fl1 = 0;
	fl2 = 0;
	f = (struct part *)o->p;
	for (k = 0; k < o->w16; k++)
		f += (unsigned short)f->y >> 10;
	i = n = (unsigned short)f->y >> 10;
	vis = (o->a14 == 63 && o->a15 == 63) ? 0 : 1;
	f_248a8(o->b2a, n);
	if (o->w30)
		o->w30--;
	if (o->w32 > 0)
		o->w32--;
	else if (o->w32 < 0)
		o->w32++;
	if (!(o->w2c & 0x2000)) {
		if (o->w2c & 0x1000) {
			if (o->w32 == 0) {
				fl1 = 1;
				o->w32 = 8;
			}
			o->w2c &= 0xefff;
		} else if (o->w32 < 0) {
			if (o->w32 & 1)
				fl1 = 1;
		} else if (g_6060000 == 0 && (o->w2c & 3) && o->w30 == 0) {
			fl2 = 1;
			o->w30 = g_60356c8[o->w2c & 3];
		}
	}
	n = i;
	f += n - 1;
	for (i = 0; i < n; i++, f--) {
		if ((f->x & 0x8000) && (o->b2f & (g_6060002 + 1)))
			continue;
		x = f->x;
		x = (x & 0x200) ? (x | 0xfc00) : (x & 0x3ff);
		y = f->y;
		y = (y & 0x200) ? (y | 0xfc00) : (y & 0x3ff);
		if (o->w22 & 0x8000)
			x = -x - (((f->b4 + 1) & 15) << 4);
		if (o->w22 & 0x80)
			y = -y - (((f->b6 + 1) & 15) << 4);
		if (vis) {
		}
		x += o->x;
		y += o->y;
		g_6061932.x = x & 0x3ff;
		g_6061932.y = y & 0x3ff;
		g_6061932.a4 = f->b4;
		g_6061932.a5 = o->a14;
		g_6061932.a6 = (f->b6 & 0xcf) | ((o->w18 << 4) & 0x30);
		if (o->w22 & 0x8000) {
			if (g_6061932.a4 & 0x80)
				g_6061932.a4 &= 0x7f;
			else
				g_6061932.a4 |= 0x80;
		}
		if (o->w22 & 0x80) {
			if (g_6061932.a6 & 0x80)
				g_6061932.a6 &= 0x7f;
			else
				g_6061932.a6 |= 0x80;
		}
		g_6061932.a7 = o->a15;
		if (fl1) {
			if (f->b4 & 0x20)
				g_6061932.a8 = 16;
			else
				g_6061932.a8 = o->w1a ? o->w1a : f->b8;
		} else if (fl2) {
			if (f->b4 & 0x20)
				g_6061932.a8 = 32;
			else
				g_6061932.a8 = o->w1a ? o->w1a : f->b8;
		} else
			g_6061932.a8 = o->w1a ? o->w1a : f->b8;
		g_6061932.a9 = (o->b1c & 0x70) | (f->b9 & 0x8f);
		g_6061932.aA = f->wA;
		*(struct spr *)(0x24000000 + (unsigned short)g_60618ec * 16) = *(struct spr *)&g_6061932;
		g_60618ec++;
	}
}

void f_251cc(void)
{
	short i, k, s;
	short *l;

	((short *)0x24003800)[0] = 0;
	((short *)0x24003800)[1] = 1;
	k = 2;
	l = g_60610ec;
	for (i = 0; i < 127; i++) {
		for (s = *l++; s; s = g_60611ec[s])
			((short *)0x24003800)[k++] = s;
	}
	((short *)0x24003800)[--k] |= 0x4000;
}

void f_2521e(void)
{
	register int i;
	short *p;

	p = g_60610ec;
	for (i = 0; i < 0x80; i += 8) {
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
	}
}
