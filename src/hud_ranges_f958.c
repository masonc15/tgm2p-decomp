/* rom: 0xf958 len: 0x1bc0 func: f_f958 context-rom: 0xf958 flags: -macsave=1 -optimize=1 -speed */
/* Five strict retained ranges: 103d0-1050e, 105ba-1065e, 1065e-10788,
 * 10942-10b28,
 * 10d94-11254. All other functions are genuine unexported compiler context. */
/* Genuine full HUD/VM compiler context (archived agent A): the HUD/VM source file 0xf958-0x11518.  f_10de6 calls f_f958..f_100a4 with
 * non-hoisted jsr through the pool, so they are in the same source file as the HUD code. */
struct frame { char pad[12]; };
struct pl {
	char pad0[0xdf];
	unsigned char bdf;         /* 0xdf */
	char pad1[0x2f4 - 0xe0];
	struct pl *owner;          /* 0x2f4 */
	char pad1b[0x308 - 0x2f8];
	unsigned long l308;        /* 0x308 */
	unsigned short w30c;       /* 0x30c */
	unsigned char b30e;        /* 0x30e */
	char pad2[0x314 - 0x30f];
	short pos[2];              /* 0x314 */
	short w318, w31a, w31c, w31e, w320;
	unsigned short w322;
	char pad3[0x330 - 0x324];
	long l330;                 /* 0x330 */
	char pad4[0x339 - 0x334];
	unsigned char b339;        /* 0x339 */
	char pad5[0x350 - 0x33a];
	unsigned long l350;        /* 0x350 */
	char pad6[0x358 - 0x354];
	unsigned long l358;        /* 0x358 */
	char pad7[0x379 - 0x35c];
	unsigned char b379;        /* 0x379 */
	char pad8[0x37e - 0x37a];
	unsigned char b37e;        /* 0x37e */
	char b37f;
	char b380;                 /* 0x380 */
	char pad9[0x38a - 0x381];
	unsigned short w38a;       /* 0x38a */
	unsigned short w38c;       /* 0x38c */
	unsigned short w38e;       /* 0x38e */
	unsigned short w390;       /* 0x390 */
	char padA[0x3b4 - 0x392];
};
struct ent {
	char pad0;
	char b1;
	char pad1[0x80c - 2];
	struct pl *owner;          /* 0x80c */
	long pad2;
	struct ent *next;          /* 0x814 */
};
struct counter {
	char pad0[7];
	unsigned char count;       /* 0x7 */
	unsigned char player;      /* 0x8 */
};
struct pair { short a, b; };
extern long g_6060008, g_606487c, g_6064880;
extern short g_6060038, g_6060022, g_6060040, g_606003e;
extern short g_6060034[];
struct gs { short pad[5]; short w10, w12, w14, w16; };
extern struct gs gs_;
extern unsigned short g_6060000;
extern unsigned char g_6064888[], g_6079374[];
extern char g_6064760, g_6064761, g_6064762, g_6064763, g_6064764;
extern char g_6033620[], g_6033628[], g_6033630[], g_6033638[];
extern struct counter g_6064884;
extern struct pl g_6064898[];
extern short d_36314[], d_38f58[], d_3ae6c[];
extern struct pair d_36304[];
extern long d_3ade4[], d_3ad6c[];
extern long d_3ad44[];
extern struct frame *d_3adbc[], *d_3ae38[], *d_3ae58[];
extern unsigned short d_3ae4c[];
extern char d_a6cd4[], d_a6ce0[], d_a6cec[], d_a71b4[];
extern char d_a79ac[], d_a78f8[], d_a78ec[], d_a78e0[], d_a79b8[];
extern struct frame d_aa934[], d_aac34[], d_aacac[], d_a6d4c;
extern unsigned char f_8b7c(void);
extern char f_18ed4(unsigned short);
extern int f_2da90(long, long);
extern int f_e39c(char *);
extern void f_e490(int, int, char *, short, char);
extern void f_185f0(struct frame *, short, short, char, short);
extern void f_11680(long, short, short, short, short, short, short, short);
extern void f_1159c(long, short, short, unsigned char, short);
extern void f_214ec(struct pl *);
extern void f_f958(struct pl *, short);
extern void f_fa78(struct pl *);
extern void f_fac8(struct pl *, short, short);
extern void f_fbdc(struct pl *, unsigned short, long, long, char);
extern void f_fc8c(unsigned long, short, long);
extern void f_fdd8(struct pl *, long);
extern void f_fef6(struct pl *);
extern void f_ffee(struct pl *);
extern void f_100a4(unsigned char);
extern void f_101b8(struct pl *);
extern void f_10942(struct pl *);
extern void f_10d94(struct pl *, char *);
extern void f_11254(long, short, int, char, short, short, long, unsigned char);
#define FIELD(p) ((struct pl *)((char *)g_6064898 + (short)((p) * 0x3b4)))
#define FB(p) ((struct pl *)((char *)fb + (short)((p) * 0x3b4)))

void f_f958(struct pl *p, short b)
{
	char e;
	short dy, dx, s;

	if (p->w30c & 0x200) {
		p->w318 += p->w31a;
		if (p->w318 < 0) {
			p->w31c = 0;
			p->w31a = 0;
			p->w318 = 0;
		} else
			p->w31a -= p->w31c;
		if (!(p->b339 & 16)) {
			p->b339 |= 16;
			p->w318 = 0;
			p->w31a = 0x800;
			p->w31c = 0x100;
		} else if (!(g_6060008 & 31) && p->b37e > 17) {
			short v = p->b37e - 17;

			p->w318 = 0;
			p->w31a = v << 9;
			p->w31c = v << 5;
		}
		dy = -((e = *(char *)&p->w318) * 18) / 64;
		dx = -(e * 6) / 64;
		f_11680((long)d_a6cd4, dx + 26, b + dy, 0, 61, s, s = e + 63, 0);
		return;
	}
	f_1159c((long)d_a6cd4, 26, b, 0, 61);
}

void f_fa78(struct pl *p)
{
	short v;

	if (p->b30e == 0)
		v = 108;
	else
		v = 0x10c;
	f_1159c((long)d_a6ce0, 0x82, v, 0, 40);
}

void f_fac8(struct pl *p, short b, short c)
{
	char e;
	short dy, dx, s;

	if (p->w30c & 8) if (p->w322 >= d_36314[g_6060034[2]] - 20) {
		p->w31e += p->w320;
		if (p->w31e < 0) {
			p->w31e = 0;
			p->w320 = 0;
		} else
			p->w320 -= 64;
		if (!(g_6060008 & 31)) {
			p->w31e = 0;
			p->w320 = 0x400;
		}
		dy = -((e = *(char *)&p->w31e) * 32) / 128;
		dx = -(e * 8) / 128;
		f_11680((long)d_a6cec, b + dx, c + dy, 0, 110, s, s = e + 63, 0);
		return;
	}
	f_1159c((long)d_a6cec, b, c, 0, 40);
}

void f_fbdc(struct pl *f, unsigned short b, long c, long d, char e)
{
	unsigned long n;

	f_11254(f->w322, c, d, e, 40, 3, 0, 2);
	f_11254(b, c + 15, d, e, 40, 3, 0, 2);
	n = f->l358 >> 15;
	if (n > 20)
		n = 20;
	f_1159c(d_3ade4[n], c + 11, d, e, 40);
}

void f_fc8c(unsigned long t, short x, long y)
{
	unsigned char m, s, c;

	m = t / 3600;
	t -= m * 3600;
	s = t / 60;
	t -= s * 60;
	c = t * 100 / 60;
	f_1159c(d_3ad44[m / 10], 0xd7, x, y, 40);
	f_1159c(d_3ad44[m % 10], 0xd7, x + 16, y, 40);
	f_1159c((long)d_a71b4, 0xd7, x + 28, y, 40);
	f_1159c(d_3ad44[s / 10], 0xd7, x + 40, y, 40);
	f_1159c(d_3ad44[s % 10], 0xd7, x + 56, y, 40);
	/* written differently from the first colon so SHC doesn't CSE the address */
	f_1159c((long)&d_a71b4[0], 0xd7, x + 68, y, 40);
	f_1159c(d_3ad44[c / 10], 0xd7, x + 80, y, 40);
	f_1159c(d_3ad44[c % 10], 0xd7, x + 96, y, 40);
}

void f_fdd8(struct pl *p, long y)
{
	long x = p->pos[0] + 50;

	if (p->w390 > 63) {
		short z = p->w390;
		short d = -(((z - 63) << 10) / 63 * 16);
		short dx = d >> 10;
		short dy = d >> 10;
		if (0) z = y;
		if (0) x = dx;
		if (0) dx = d;
		if (0) dy = 63;

		f_11680(d_3ad6c[p->w38a], dy + 35, x + dx, y, 110, z, z, 0);
		f_1159c(d_3ad6c[p->w38e], 35, x, y, 40);
		if (g_6060000 < 40)
			p->w390 -= 4;
		if (p->w390 <= 63) {
			p->w390 = 63;
			p->w38e = p->w38a;
		}
	} else {
		f_1159c(d_3ad6c[p->w38a], 35, x, y, 40);
		if (p->w38e != p->w38a)
			p->w390 = 0x80;
	}
}

void f_fef6(struct pl *p)
{
	long col;
	long fr;
	long x;
	short n;
	short k;
	short v;

	fr = 13;
	col = (p->b30e == 0) ? 111 : 0xc5;
	if (p->b37e == 20 && (g_6060008 & 3))
		fr = 0x99;
	v = p->b37e;
	n = v * 74 / 20;
	if (0) g_6060000 = n;
	for (k = n / 16, x = 62; k > 0; k--) {
		f_1159c((long)d_a79ac, x + 80, col, fr, 40);
		x -= 16;
		n -= 16;
	}
	if (n > 0) {
		n--;
		f_1159c((long)(d_a78f8 + n * 12), x + 80, col, fr, 40);
	}
	f_1159c((long)d_a78ec, 77, col, 0x94, 39);
	f_1159c((long)d_a78e0, 80, col, 12, 40);
}

void f_ffee(struct pl *p)
{
	long col;
	short i, n;

	col = (p->b30e == 0) ? 111 : 0xc5;
	n = f_8b7c();
	for (i = 0; i < n; i++) {
		if (g_6064888[p->b30e] > i)
			f_1159c(0xa79b8, i * 14 + 35, col, 15, 40);
		else
			f_1159c(0xa79b8, i * 14 + 35, col, 14, 40);
	}
}

void f_100a4(unsigned char on_field)
{
	unsigned char c, p;
	short n, div, d;
	int y;
	int x;
	struct pl *fb;
	c = g_6064884.count;
	if (c > 0) {
		if (p = g_6064884.player, on_field != 0) {
			fb = g_6064898;
			x = !p ? FB(p)->pos[0] + 6 : FB(p)->pos[0] - 50;
			y = FB(p)->pos[1] + 2;
		} else {
			if (p == 0)
				x = 108;
			else
				x = 268;
			y = 118;
		}
		if (0) y = 10; /* dead code: steers SHC register ranking */
		for (div = 10, n = c; div; div /= 10) {
			d = n / div;
			n -= d * div;
			if (div != 10 || d)
				f_1159c((long)&d_aac34[d], y, x, 0, 40);
			x = x + 8;
		}
		f_1159c((long)d_aacac, y, x + 4, 0, 40);
	}
}

void f_101b8(struct pl *p)
{
	if (g_6079374[p->b30e]) {
		f_1159c((long)&d_aa934[g_6079374[p->b30e]], p->pos[1] - 190, p->pos[0] + 20, 4, 61);
	}
	if (p->b380)
		g_6079374[p->b30e] = p->b380;
}

void f_10228(struct ent *p)
{
	struct ent *t, *n;
	struct pl *q;
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
			if (0) f_1159c(0, y, 0, 0, 0);
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
				f_1159c((long)&d_aa934[v], y, x, 4, 40);
		}
	}
}

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


	if (g_6060034[6] == 2) {
		f_185f0(((struct frame *)0xa6c8c), x, y, 0, 110);
	} else if (g_6060034[5] == 0) {

		if (f_2da90(0, 0)) {
			f_185f0(((struct frame *)0xa6c8c), x, y, 0, 110);
		} else {
			if (g_6064760 || g_6064761)
				f_185f0(((struct frame *)0xa6d04), x, y, 0, 110);
			else
				f_185f0(((struct frame *)0xa6d4c), x, y, 0, 110);
		}
	} else {

		if (f_2da90(0, 0))
			f_185f0(((struct frame *)0xa6c8c), x, 80, 0, 110);
		else if (g_6064760)
			f_185f0(((struct frame *)0xa6d04), x, 80, 0, 110);
		else
			f_185f0(((struct frame *)0xa6d4c), x, 80, 0, 110);
		y = 0xf0;
		if (f_2da90(1, 0)) {
			f_185f0(((struct frame *)0xa6c8c), x, y, 0, 110);
		} else if (g_6064761)
			f_185f0(((struct frame *)0xa6d04), x, y, 0, 110);
		else
			f_185f0(((struct frame *)0xa6d4c), x, y, 0, 110);
	}
}

void f_1050e(struct pl *p)
{
	int a, b, v;

	if (!(g_6060008 & 0x60))
		return;
	if (f_2da90(p->b30e, 0) || g_6060034[6] == 2) {
		if (p->b30e == 0)
			f_1159c((long)((struct frame *)0xa6ecc), 100, p->pos[0], 0, 125);
		else
			f_1159c((long)((struct frame *)0xa6ed8), 100, p->pos[0], 0, 125);
		return;
	}
	a = g_6064760;
	b = g_6064761;
	if (g_6060034[5] == 0)
		v = a + b;
	else if (!p->b30e)
		v = a;
	else
		v = b;
	if ((unsigned char)v)
		f_1159c((long)((struct frame *)0xa6f20), 100, p->pos[0], 0, 125);
	else
		f_1159c((long)((struct frame *)0xa6efc), 100, p->pos[0], 0, 125);
}

void f_105ba(long arg0, short x)
{
	if (g_6060034[5] == 0) {
		f_185f0(((struct frame *)0xa6de8), x, 0x93, 0, 110);
		return;
	}
	if (!(arg0 & 1))
		f_185f0(((struct frame *)0xa6de8), x, 10, 0, 110);
	if (!(arg0 & 2))
		f_185f0(((struct frame *)0xa6de8), x, 0x11d, 0, 110);
}

void f_1065e(unsigned char a, short b, short c)
{
	f_185f0(d_3adbc[(unsigned char)a], c, b, 0, 110);
	f_185f0((struct frame *)0xa6df4, c, b + 10, 0, 110);
}

void f_106a6(unsigned char a, short b, short c)
{
	f_185f0(d_3adbc[(unsigned char)a], c, b, 0, 110);
	f_185f0((struct frame *)0xa6ddc, c, b + 10, 0, 110);
}

void f_106ee(unsigned char a, char b, short c, short d, short e)
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
	unsigned char a, b, c, d;
	unsigned char e;
	struct gs *pp;
	short k;


	mask = 0;
	if (g_6064898[0].l308 & 0x1000)
		mask = 1;
	if (g_6064898[1].l308 & 0x1000)
		mask |= 2;
	if (g_606487c != 0)
		mask = 0;
		if (0) g_6064763 = c;
	if (mask == 3)
		return;
	if ((pp = (struct gs *)g_6060034)->w12 == 2) {
		if (0) c = 0xe4;
		f_105ba(mask, 0xe4);
		return;
	}
	k = 0xfb;
	c = g_6064762;
	a = g_6064760;
	b = g_6064761;
	if (pp->w10 == 0) {
		if (c != 0) {
			f_106a6(c, 0x10b, 0xe4);
			return;
		}
		if (a && g_6064761) {
			f_106ee(a, pp->w14, 0xda, 0xe4, 0);
			f_106ee(g_6064761, g_6060034[8], k, 0xe4, 1);
		} else if (a != 0) {
			f_106ee(a, pp->w14, k, 0xe4, 1);
		} else if (b != 0)
			f_106ee(b, pp->w16, k, 0xe4, 1);
			if (0) g_6060034[8] = 0x10b;
			if (0) d = d;
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
		if (d != 0)
			f_106a6(d, 0x10b, 0xe4);
		else {
			if ((d = g_6064761) != 0)
				f_106ee(d, g_6060034[7], k, 0xe4, 1);
		}
	}
	e = g_6064764;
	if (e)
		f_1065e(e, 0x87, 0xe4);
}

void f_10942(struct pl *p)
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

void f_10b28(struct pl *p)
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

void f_10d94(struct pl *p, char *out)
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

void f_10de6(void)
{
	char buf[3];
	struct pl *p;
	short i;

	if (g_6064880 & 3) {
		f_11680(0xaac10, 12, g_6064898[0].pos[0] - 44, 208, 60, 63, 63, 1);
		f_11680(0xaac10, 12, g_6064898[1].pos[0] - 44, 208, 60, 63, 63, 1);
	} else if (g_6064880 & 4) {
		f_11680(0xaac1c, 12, 98, 208, 60, 63, 63, 1);
	}
	if (g_6064880 & 4) {
		p = g_6064898;
		f_f958(p, 92);
		f_fc8c(p->l350, 104, 0);
		for (i = 0; i < 2; i++, p++) {
			f_10d94(p, &buf[0]);
			f_fac8(p, 169, i == 0 ? 62 : 226);
			f_fbdc(p, d_38f58[2], 182, i == 0 ? 69 : 226, buf[2]);
		}
	} else if (g_6064880 & 1) {
		p = g_6064898;
		for (i = 0; i < 2; i++, p++) {
			f_10d94(p, &buf[0]);
			f_f958(p, i == 0 ? 12 : 172);
			f_fa78(p);
			f_fac8(p, 169, i == 0 ? 108 : 268);
			f_fc8c(p->l350, p->pos[0] - p->bdf / 2 * 8, buf[1]);
			f_101b8(p);
			if (p->w30c & 2)
				f_fdd8(p, buf[0]);
			if ((p->w30c & 0x1000) && p->w38a >= 18)
				f_fdd8(p, buf[0]);
			if (p->w30c & 0x1002)
				f_214ec(p);
			f_11254(p->l330, 145, p->b30e == 0 ? 108 : 268, buf[2], 40, 6, 0, 1);
			if (p->w30c & 0x1082)
				f_fbdc(p, d_38f58[p->w38c], 182, i == 0 ? 108 : 268, buf[2]);
			else
				f_fbdc(p, d_38f58[2], 182, i == 0 ? 108 : 268, buf[2]);
		}
		f_100a4(0);
	} else if (g_6064880 & 2) {
	p = g_6064898;
	for (i = 0; i < 2; i++, p++) {
		f_10d94(p, &buf[0]);
		f_f958(p, i == 0 ? 16 : 212);
		if (!(p->w30c & 0x900))
			f_fef6(p);
		f_ffee(p);
		f_fac8(p, 167, i == 0 ? 111 : 178);
		if (g_606487c)
			f_fbdc(p, d_36314[2], 182, i == 0 ? 111 : 185, buf[2]);
		else
			f_fbdc(p, d_36314[g_6060034[2]], 182, i == 0 ? 111 : 185, buf[2]);
		f_101b8(p);
	}
	f_100a4(1);
	p = g_6064898;
	f_10942(p);
	if (p->l350 < 1200) {
		if ((g_6060008 & 4) || p->l350 == 0)
			f_fc8c(p->l350, 104, 156);
		else
			f_fc8c(p->l350, 104, buf[1]);
	} else
		f_fc8c(p->l350, 104, buf[1]);
	}
}

void f_11254(long val, short a, int x, char c, short e, short digits, long z, unsigned char mode)
{
	long div;
	int d;
	int n;
	long fr;

	n = digits;
	div = 1;
	for (digits--; digits; digits--)
		div *= 10;
	while (n) {
		d = val / div;
		val -= div * d;
		if (d > 9)
			d %= 10;
		if (d == 0) {
			if (z || n == 1)
				fr = (long)d_3adbc[0];
			else
				fr = 0;
		} else {
			fr = (long)d_3adbc[d];
			z = 1;
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

void f_11336(long val, short a, int x, short digits, long z, unsigned char mode)
{
	long div;
	int d;
	int n;
	struct frame *fr;

	n = digits;
	div = 1;
	for (digits--; digits; digits--)
		div *= 10;
	while (n) {
		d = val / div;
		val -= div * d;
		if (d > 9)
			d %= 10;
		if (d == 0) {
			if (z || n == 1)
				fr = d_3adbc[0];
			else
				fr = 0;
		} else {
			fr = d_3adbc[d];
			z = 1;
		}
		if (fr) {
			f_185f0(fr, a, x, 0, 40);
			x += 8;
		} else if (mode == 2)
			x += 8;
		n--;
		div /= 10;
	}
}

void f_11424(long val, short a, int x, char c, short e, short digits, long z, unsigned char mode)
{
	long div;
	int d;
	int n;
	long fr;

	n = digits;
	div = 1;
	for (digits--; digits; digits--)
		div *= 10;
	while (n) {
		d = val / div;
		val -= div * d;
		if (d > 9)
			d %= 10;
		if (d == 0) {
			if (z || n == 1)
				fr = d_3ad44[0];
			else
				fr = 0;
		} else {
			fr = d_3ad44[d];
			z = 1;
		}
		if (fr) {
			f_1159c(fr, a, x, c, e);
			x += 16;
		} else if (mode == 2)
			x += 16;
		n--;
		div /= 10;
	}
}

