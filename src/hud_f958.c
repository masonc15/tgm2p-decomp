/* rom: 0xf958 len: 0x480 func: f_f958 flags: -macsave=1 -optimize=1 -speed */
/* Byte-exact prefix (f_f958..f_fc8c) of the HUD source file 0xf958-0x11518.  The rest of that file
 * (f_fdd8 onward) shares its literal pools with these and is still in progress; this prefix's own
 * pool ends at 0xfdd8.  It replaces src/vm_fbdc.c and src/vm_fc8c.c, which held f_fbdc and f_fc8c.
 * Data references use their true addresses: d_a6cd4 etc. are ROM data, g_6060034 is a short array
 * (element 2 is 0x6060038), and f_1159c/f_11680 are outside the file. */
struct pl {
	char pad0[0x30c];
	unsigned short w30c;       /* 0x30c */
	unsigned char b30e;        /* 0x30e */
	char pad1[0x318 - 0x30f];
	short w318, w31a, w31c, w31e, w320;
	unsigned short w322;
	char pad2[0x339 - 0x324];
	unsigned char b339;        /* 0x339 */
	char pad3[0x358 - 0x33a];
	unsigned long l358;        /* 0x358 */
	char pad4[0x37e - 0x35c];
	unsigned char b37e;        /* 0x37e */
	char pad5[0x3b4 - 0x37f];
};
extern long g_6060008;
extern short g_6060034[];
extern short d_36314[];
extern long d_3ade4[], d_3ad44[];
extern char d_a6cd4[], d_a6ce0[], d_a6cec[], d_a71b4[];
extern void f_11680(long, short, short, short, short, short, short, short);
extern void f_1159c(long, short, short, unsigned char, short);
extern void f_11254(long, short, int, char, short, short, long, unsigned char);

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

void f_fc8c(unsigned long t, long x, long y)
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
