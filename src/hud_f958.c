/* rom: 0xf958 len: 0x74c func: f_f958 flags: -macsave=1 -optimize=1 -speed */
/* Byte-exact first part of the HUD source file 0xf958-0x11518: f_f958..f_ffee, 0xf958-0x100a4.  Replaces
 * src/hud_f958.c (0xf958-0xfdd8) and the asm for 0xfdd8-0x100a4.  In f_fdd8 the dead `if (0) a = b;`
 * statements emit nothing but count as references when SHC ranks which values get r8-r14.
 * f_fc8c's x is `short`: that is what makes f_10de6's `p->pos[0] - p->bdf / 2 * 8` argument evaluate the
 * divide first (it is byte-identical here either way). */
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
