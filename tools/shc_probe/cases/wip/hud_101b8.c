/* rom: 0x101b8 len: 0x1360 func: f_101b8 flags: -macsave=1 -optimize=1 -speed */
/* WIP (agent A), NOT a finished unit.  0x101b8-0x11518 is one source file (one shared literal pool,
 * bsr/bra calls between its functions).  Status of each function by funcscore/alignscore:
 *   f_101b8 100%   f_10228 93.5% (x/y registers r9/r10 swapped)   f_103d0 ~72% (aligned; hoisting ranks)
 *   f_1050e ~86% (temps)   f_105ba ~68%   f_1065e/f_106a6/f_106ee 100% when f_103d0 is absent (state)
 *   f_10788 ~79%   f_10942 100% code   f_10b28 100% code   f_10d94 100%   f_10de6 not written
 *   f_11254 ~80%   f_11336 and f_11424 not written (digit-drawing siblings of f_11254)
 * The header above only makes funcscore work; use the per-function scores, not the total.
 */
struct frame { char pad[12]; };
struct point { short x, y; };
struct fld {
	char pad0[0x30e];
	unsigned char b30e;        /* 0x30e */
	char pad1[0x314 - 0x30f];
	short pos[2];              /* 0x314 */
	char pad2[0x379 - 0x318];
	unsigned char b379;        /* 0x379 */
	char pad3[0x380 - 0x37a];
	char b380;                 /* 0x380 */
};
struct ent {
	char pad0;
	char b1;
	char pad1[0x80c - 2];
	struct fld *owner;         /* 0x80c */
	struct ent *next;          /* 0x814 */
};
extern unsigned char g_6079374[];
extern struct frame d_aa934[];
extern void f_1159c(struct frame *, short, short, short, short);
extern char f_18ed4(unsigned short);

void f_101b8(struct fld *p)
{
	if (g_6079374[p->b30e]) {
		f_1159c(&d_aa934[g_6079374[p->b30e]], p->pos[1] - 190, p->pos[0] + 20, 4, 61);
	}
	if (p->b380)
		g_6079374[p->b30e] = p->b380;
}

void f_10228(struct ent *p)
{
	struct ent *t, *n;
	struct fld *q;
	char a[2], b[2], c[2];
	char i;
	int r;
	unsigned char id;
	char k, v;
	int y, x;
	char s;

	a[0] = a[1] = 0;
	b[0] = b[1] = 0;
	c[0] = c[1] = 0;
	t = p;
	if ((r = f_18ed4(t->b1)) == 0)
		return;
	q = t->owner;
	a[q->b30e]++;
	b[q->b30e] = r;
	n = p->next;
	for (i = 0; i < 18; i++) {
		if (n == 0)
			break;
		t = n;
		n = t->next;
		r = f_18ed4(t->b1);
		if (r != 0) {
			q = t->owner;
			k = r - 1;
			id = q->b30e;
			k ^= id;
			a[id]++;
			b[q->b30e] = r;
			if (a[0] == 1 && a[1] == 1 && b[0] == b[1])
				continue;
			if (a[q->b30e] == 1 && q->b379 == t->b1)
				continue;
			c[k]++;
			v = t->b1;
			s = r;
			switch (k + s) {
			case 1:
			case 3:
			case 5:
				y = q->pos[1] + 5;
				x = q->pos[0] - 60;
				break;
			case 2:
			case 6:
				y = q->pos[1] + 5;
				x = q->pos[0] + 30;
				break;
			}
			if (v)
				f_1159c(&d_aa934[v], y, x, 4, 40);
		}
	}
}

struct gs { char pad[10]; short w10; short w12; short w14; short w16; };
struct pl { char pad[0x308]; long f308; char pad2[0x3b4 - 0x30c]; };
extern struct pl g_6064898[];
extern char g_6064762, g_6064763, g_6064764;
extern long g_6060008;
extern short g_6060040, g_606003e;
extern char g_6064760, g_6064761;
extern struct frame d_a6de8[], d_a6ecc[], d_a6ed8[], d_a6efc[], d_a6f20[];
extern int f_2da90(long, long);
extern struct frame d_a6d4c;
extern long g_606487c;
extern struct gs g_6060034;
extern void f_185f0(struct frame *, short, short, char, short);

void f_103d0(void)
{
	int x, y;

	if (!(g_6060008 & 0x60))
		return;
	if (g_606487c)
		x = 0xaa;
	else
		x = 0xc8;
	y = 0xa0;

	if (g_6060034.w12 == 2) {
		f_185f0(((struct frame *)0xa6c8c), x, y, 0, 110);
	} else if (g_6060034.w10 == 0) {
		if (f_2da90(0, 0) == 0) {
			if (g_6064760 || g_6064761)
				f_185f0(((struct frame *)0xa6d04), x, y, 0, 110);
			else
				f_185f0((&d_a6d4c), x, y, 0, 110);
		} else {
			f_185f0(((struct frame *)0xa6c8c), x, y, 0, 110);
		}
	} else {
		if (f_2da90(0, 0))
			f_185f0(((struct frame *)0xa6c8c), x, 80, 0, 110);
		else if (g_6064760)
			f_185f0(((struct frame *)0xa6d04), x, 80, 0, 110);
		else
			f_185f0((&d_a6d4c), x, 80, 0, 110);
		y = 0xf0;
		if (f_2da90(1, 0)) {
			f_185f0(((struct frame *)0xa6c8c), x, y, 0, 110);
		} else if (g_6064761)
			f_185f0(((struct frame *)0xa6d04), x, y, 0, 110);
		else
			f_185f0((&d_a6d4c), x, y, 0, 110);
	}
}

void f_1050e(struct fld *p)
{
	int a, b, v;

	if (!(g_6060008 & 0x60))
		return;
	if (f_2da90(p->b30e, 0) || g_6060040 == 2) {
		if (p->b30e == 0)
			f_185f0(((struct frame *)0xa6ecc), 100, p->pos[0], 0, 125);
		else
			f_185f0(((struct frame *)0xa6ed8), 100, p->pos[0], 0, 125);
		return;
	}
	a = g_6064760;
	b = g_6064761;
	if (g_606003e == 0)
		v = a + b;
	else if (p->b30e == 0)
		v = a;
	else
		v = b;
	if ((unsigned char)v)
		f_185f0(((struct frame *)0xa6f20), 100, p->pos[0], 0, 125);
	else
		f_185f0(((struct frame *)0xa6efc), 100, p->pos[0], 0, 125);
}

void f_105ba(long arg0, short x)
{
	if (g_606003e == 0) {
		f_185f0(((struct frame *)0xa6de8), x, 0x93, 0, 110);
		return;
	}
	if (!(arg0 & 1))
		f_185f0(((struct frame *)0xa6de8), x, 10, 0, 110);
	if (!(arg0 & 2))
		f_185f0(((struct frame *)0xa6de8), x, 0x11d, 0, 110);
}

extern struct frame *d_3adbc[], *d_3ae38[];

void f_1065e(char a, short b, short c)
{
	f_185f0(d_3adbc[(unsigned char)a], c, b, 0, 110);
	f_185f0((struct frame *)0xa6df4, c, b + 10, 0, 110);
}

void f_106a6(char a, short b, short c)
{
	f_185f0(d_3adbc[(unsigned char)a], c, b, 0, 110);
	f_185f0((struct frame *)0xa6ddc, c, b + 10, 0, 110);
}

void f_106ee(char a, char b, short c, short d, short e)
{
	b--;
	f_185f0(d_3adbc[(unsigned char)a], d, c, 0, 110);
	f_185f0(d_3ae38[(unsigned char)b], d, c, 0, 110);
	if (e)
		f_185f0((struct frame *)0xa6ddc, d, c + 26, 0, 110);
	else
		f_185f0((struct frame *)0xa6e00, d, c + 27, 0, 110);
}

void f_10788(void)
{
	int mask;
	char a, b, c, d;
	struct gs *pp;

	pp = &g_6060034;
	mask = 0;
	if (g_6064898[0].f308 & 0x1000)
		mask = 1;
	if (g_6064898[1].f308 & 0x1000)
		mask |= 2;
	if (g_606487c)
		mask = 0;
	if (mask == 3)
		return;
	if (g_6060034.w12 == 2) {
		f_105ba(mask, 0xe4);
		return;
	}
	c = g_6064762;
	a = g_6064760;
	b = g_6064761;
	if (pp->w10 == 0) {
		if (c) {
			f_106a6(c, 0x10b, 0xe4);
			return;
		}
		if (a && g_6064761) {
			f_106ee(a, pp->w14, 0xda, 0xe4, 0);
			f_106ee(g_6064761, g_6060034.w16, 0xfb, 0xe4, 1);
			return;
		}
		if (a) {
			f_106ee(a, pp->w14, 0xfb, 0xe4, 1);
			return;
		}
		if (b)
			f_106ee(b, pp->w16, 0xfb, 0xe4, 1);
		return;
	}
	if (!(mask & 1)) {
		if (c)
			f_106a6(c, 10, 0xe4);
		else if (a)
			f_106ee(a, pp->w14, 10, 0xe4, 1);
	}
	if (!(mask & 2)) {
		d = g_6064763;
		if (d)
			f_106a6(d, 0x10b, 0xe4);
		else if (g_6064761)
			f_106ee(g_6064761, g_6060034.w14, 0xfb, 0xe4, 1);
	}
	if (g_6064764)
		f_1065e(g_6064764, 0x87, 0xe4);
}

struct hud {
	char pad0[0x308];
	unsigned long l308;        /* 0x308 */
	unsigned short w30c;       /* 0x30c */
	unsigned char b30e;        /* 0x30e */
};
struct pair { short a, b; };
extern short g_6060022;
extern struct pair d_36304[];
extern unsigned short d_3ae4c[];
extern struct frame *d_3ae58[];
extern short d_3ae6c[];

void f_10942(struct hud *p)
{
	short x, y, i;
	unsigned char z;
	unsigned short m;

	if (p->w30c & 8) {
		x = 80;
		y = 160;
		z = 40;
	} else if (g_6060022 == 0) {
		x = 180;
		y = d_36304[p->b30e + 2].a;
		z = 40;
	} else {
		x = 170;
		y = d_36304[p->b30e].a;
		z = 110;
	}
	if (p->w30c & 4)
		return;
	m = p->w30c;
	if ((p->w30c & 0x80) && !(p->l308 & 0x10))
		m &= 0xffbf;
	if ((p->w30c & 0x1000) && !(p->l308 & 0x10))
		m &= 0xfb9f;
	for (i = 0; i < 5; i++) {
		if (d_3ae4c[i] & m) {
			f_185f0(d_3ae58[i], x, y - d_3ae6c[i], 0, z);
			x += 8;
		}
	}
}

struct hud2 {
	char pad0[0x2f4];
	struct hud *owner;         /* 0x2f4 */
	char pad1[0x30e - 0x2f8];
	unsigned char b30e;        /* 0x30e */
};
extern int f_e39c(char *);
extern void f_e490(int, int, char *, short, char);
extern char g_6033620[], g_6033628[], g_6033630[], g_6033638[];

void f_10b28(struct hud2 *p)
{
	short x, y, i;
	unsigned short m;

	x = 170;
	y = d_36304[p->b30e].a;
	if (p->owner->w30c & 1)
		f_e490((unsigned short)y - f_e39c(g_6033620) / 2, 155, g_6033620, 15, 0);
	if (p->owner->w30c & 2)
		f_e490((unsigned short)y - f_e39c(g_6033628) / 2, 155, g_6033628, 15, 0);
	if (p->owner->w30c & 0x80)
		f_e490((unsigned short)y - f_e39c(g_6033630) / 2, 155, g_6033630, 15, 0);
	if (p->owner->w30c & 0x1000)
		f_e490((unsigned short)y - f_e39c(g_6033638) / 2, 155, g_6033638, 15, 0);
	m = p->owner->w30c;
	if ((p->owner->w30c & 0x80) && !(p->owner->l308 & 0x10))
		m &= 0xfdbf;
	if ((p->owner->w30c & 0x1000) && !(p->owner->l308 & 0x10))
		m &= 0xf99f;
	for (i = 0; i < 5; i++) {
		if (d_3ae4c[i] & m) {
			f_185f0(d_3ae58[i], x, y - d_3ae6c[i], 0, 110);
			x += 8;
		}
	}
}

struct plf {
	char pad0[0x308];
	unsigned long l308;        /* 0x308 */
	char pad1[0x358 - 0x30c];
	unsigned long l358;        /* 0x358 */
};
extern long g_6060008;

void f_10d94(struct plf *p, char *out)
{
	unsigned char f = 0;

	if (p->l358 >= 0x140000 && (g_6060008 & 3))
		f = 1;
	if (p->l358 >= 0x140000 && !(p->l308 & 0x1000))
		f = 1;
	if (f) {
		out[0] = 7;
		out[1] = 8;
		out[2] = 9;
	} else {
		out[0] = out[1] = out[2] = 0;
	}
}

extern struct frame *d_3adbc[];

void f_11254(long val, short a, int x, char c, short e, short digits, long z, char mode)
{
	long div;
	int d, n;
	struct frame *fr;
	short k;
	int zz;

	zz = z;
	n = digits;
	div = 1;
	for (k = digits - 1; k; k--)
		div *= 10;
	while (n) {
		d = val / div;
		val -= div * d;
		if (d > 9)
			d = d % 10;
		if (d == 0) {
			if (zz || n == 1)
				fr = d_3adbc[0];
			else
				fr = 0;
		} else {
			zz = 1;
			fr = d_3adbc[d];
		}
		if (fr) {
			f_1159c(fr, a, x, c, e);
			x += 8;
		} else if (mode == 2)
			x += 8;
		n--;
		div /= 10;
	}
}
