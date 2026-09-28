/* rom: 0xfdd8 len: 0x2cc func: f_fdd8 flags: -macsave=1 -optimize=1 -speed */
struct fld {
	char pad0[0x30e];
	unsigned char b30e;        /* 0x30e */
	char pad1[0x314 - 0x30f];
	short x314;                /* 0x314 */
	char pad2[0x37e - 0x316];
	unsigned char b37e;        /* 0x37e */
	char pad3[0x38a - 0x37f];
	unsigned short x38a;       /* 0x38a */
	char pad4[0x38e - 0x38c];
	unsigned short x38e;       /* 0x38e */
	unsigned short x390;       /* 0x390 */
};
extern unsigned short g_6060000;
extern long g_6060008;
extern unsigned char g_6064888[];
extern long d_3ad6c[];
extern char d_a79ac[], d_a78f8[], d_a78ec[], d_a78e0[], d_a79b8[];
extern void f_11680(long, short, short, short, short, short, short, short);
extern void f_1159c(long, long, long, long, long);
extern unsigned char f_8b7c(void);

void f_fdd8(register struct fld *p, register long y)
{
	register long x = p->x314 + 50;

	if (p->x390 > 63) {
		register short z = p->x390;
		short d = -(((z - 63) << 10) / 63 * 16);
		register short dx = d >> 10;
		register short dy = d >> 10;

		f_11680(d_3ad6c[p->x38a], dy + 35, x + dx, y, 110, z, z, 0);
		f_1159c((&d_3ad6c[0])[p->x38e], 35, x, y, 40);
		if (g_6060000 < 40)
			p->x390 -= 4;
		if (p->x390 <= 63) {
			p->x390 = 63;
			p->x38e = p->x38a;
		}
	} else {
		f_1159c(d_3ad6c[p->x38a], 35, x, y, 40);
		if (p->x38e != p->x38a)
			p->x390 = 0x80;
	}
}

void f_fef6(struct fld *p)
{
	long col;
	long fr;
	long x;
	register short n;
	short k;
	short v;

	fr = 13;
	if (!p->b30e)
		col = 111;
	else
		col = 0xc5;
	if (p->b37e == 20 && (g_6060008 & 3))
		fr = 0x99;
	v = p->b37e;
	n = v * 74 / 20;
	x = 62;
	for (k = n / 16; k > 0; k--) {
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

void f_ffee(struct fld *p)
{
	long col;
	short i, n;

	if (!p->b30e)
		col = 111;
	else
		col = 0xc5;
	n = f_8b7c();
	for (i = 0; i < n; i++) {
		if (g_6064888[p->b30e] > i)
			f_1159c((long)d_a79b8, i * 14 + 35, col, 15, 40);
		else
			f_1159c((long)d_a79b8, i * 14 + 35, col, 14, 40);
	}
}
