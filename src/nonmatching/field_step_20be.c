/* rom: 0x20be len: 0xd8e func: f_20be flags: -macsave=1 -optimize=1 -speed */
/* Non-matching: functionally equivalent C for [0x20be, 0x2e4c) (f_20be, f_29ea, f_2a92, f_2c0a, f_2d06, f_2d54),
 * linked only by the shiftable build (splits_nonmatching.txt). Not matching coverage.
 * From research/2026-10-16-20be-tail/v4.c. */
extern char d_63f10[];
extern char d_64710[];
extern char d_64b10[];
extern char d_65310[];
extern char d_68ad0[];
extern char d_68b10[];
struct cell {
	short w0;
	char b2;
	char b3;
	short w4;
};
struct pos {
	short x, y;
};
struct field {
	struct cell *cells;        /* 0x000 */
	short w4[8][12];           /* 0x004 */
	char pad0[0xdc - 0xc4];
	unsigned char bdc;         /* 0x0dc */
	unsigned char bdd;
	unsigned char bde;
	unsigned char bdf;
	short e0[2];               /* 0x0e0 */
	char pad1[0x2f4 - 0xe4];
	struct field *other;       /* 0x2f4 */
	struct field *p2f8;        /* 0x2f8 */
	unsigned char b2fc[4];     /* 0x2fc */
	short w300[4];             /* 0x300 */
	union {
		unsigned long l308;    /* 0x308 */
		unsigned char b[4];
	} u308;
	unsigned short w30c;       /* 0x30c */
	unsigned char b30e;        /* 0x30e */
	char pad30f;
	unsigned long l310;        /* 0x310 */
	short w314;                /* 0x314 */
	short w316;                /* 0x316 */
	short w318;
	short w31a;
	short w31c;
	short w31e;
	short w320;
	unsigned short w322;
	short w324;
	short w326;
	short w328;
	short w32a;                /* 0x32a */
	char pad32c[4];
	long l330;                 /* 0x330 */
	long l334;                 /* 0x334 */
	unsigned char b338;        /* 0x338 */
	unsigned char b339;
	unsigned char b33a[6];     /* 0x33a */
	unsigned char b340;        /* 0x340 */
	signed char c341;
	signed char c342;
	signed char c343;
	short w344;                /* 0x344 */
	unsigned char b346;
	unsigned char b347;
	unsigned char b348;
	unsigned char b349;
	char pad34a[2];
	long l34c;                 /* 0x34c */
	unsigned long l350;        /* 0x350 */
	char pad354[4];
	long l358;                 /* 0x358 */
	union {
		short w35c;            /* 0x35c */
		struct {
			unsigned char b35c;
			unsigned char b35d;
		} b;
	} u35c;
	short w35e;
	short w360;                /* 0x360 */
	unsigned char b362;
	char pad363;
	long l364;                 /* 0x364 */
	long l368;                 /* 0x368 */
	unsigned char b36c[4];     /* 0x36c */
	long l370;                 /* 0x370 */
	long l374;                 /* 0x374 */
	unsigned char b378;        /* 0x378 */
	unsigned char b379;
	char pad37a[2];
	short w37c;                /* 0x37c */
	unsigned char b37e;
	unsigned char b37f;
	unsigned char b380;        /* 0x380 */
	char pad381[3];
	long l384;                 /* 0x384 */
	unsigned char b388;        /* 0x388 */
	char pad389;
	short w38a;
	short w38c;
	short w38e;
	short w390;
	short w392;
	short w394;
	short w396;
	unsigned char b398;        /* 0x398 */
	unsigned char b399;
	unsigned char b39a;
	char pad39b;
	short w39c;                /* 0x39c */
	unsigned char b39e;
	unsigned char b39f;
	unsigned char b3a0;        /* 0x3a0 */
	unsigned char b3a1;
	char pad3a2[0x3b4 - 0x3a2];
};

extern struct field g_6064898[2];
extern unsigned long g_6064880;
extern long g_606487c;
extern long g_60657a0;
extern short g_6060038;
extern unsigned char g_6079374[];
extern struct cell g_6078608[2][264];
extern struct pos d_36304[4];
extern short d_36314[];
extern long f_2beca(int);
extern unsigned short f_2f3c(unsigned long *, unsigned short);
extern void f_15f10(struct field *, int);
extern void f_2d06(struct field *);
extern void f_1175c(struct field *);
extern void f_21454(struct field *, int, int);
extern unsigned char g_606475a[];
extern unsigned char g_606475e[];
extern unsigned char g_6079288[];
extern short d_38f6a[];
extern unsigned long g_6060008;
struct gs {
	unsigned char b0;
	char pad1[3];
	unsigned char b4[2];
	char pad6;
	unsigned char b7;
	unsigned char b8;
};
extern struct gs g_6064884;
extern char g_6032830[];
extern char g_6032838[];
extern char d_aaa24[];
extern void f_7c08(struct field *);
extern void f_7f38(struct field *);
extern void f_50e0(struct field *);
extern void f_44d4(struct field *, int);
extern void f_61d0(struct field *);
extern void f_5168(struct field *);
extern void f_4044(struct field *);
extern void f_57d4(struct field *);
extern void f_5b80(struct field *);
extern void f_7264(struct field *);
extern void f_67f4(struct field *);
extern void f_7324(struct field *);
extern void f_667c(struct field *);
extern void f_7956(struct field *);
extern void f_1421c(struct field *);
extern void f_8430(struct field *);
extern void f_2e4c(struct field *);
extern void f_1050e(struct field *);
extern int f_e39c(char *);
extern void f_e490(int, int, char *, int, int);
extern int f_2da90(int, int);
extern void f_2db32(int, int);
extern int f_2d54(struct field *);
extern void f_10942(struct field *);
extern void f_1159c(void *, int, int, int, int);
extern unsigned char g_606488a;
extern unsigned char g_606488c;
struct cnt { short a, c; };
extern struct cnt g_606568a;
extern signed char g_6079274[][4];
extern void f_2f7a(struct field *);
extern void f_2e6fc(int);
extern void f_10b28(struct field *);
extern void f_2af5e(int, int, long);
extern void f_11680(void *, int, short, short, short, short, short, short);
extern char d_a7028[];
extern char d_a7034[];
extern char *d_36334[];
extern char g_6032844[], g_603284c[], g_6032854[], g_603285c[], g_6032868[];
extern unsigned char g_6066198;
extern unsigned short g_6060000;
extern void f_21bc8(struct field *);
extern void f_191a4(void);
extern void f_10788(void);
extern void f_15fae(struct field *, int, unsigned long);
extern void f_16938(struct field *);
extern unsigned char g_607927c[];
extern unsigned short *d_363a8[];
extern short d_363bc[];
extern unsigned char d_363c6[];
unsigned char f_29ea(struct field *f);
void f_2efc(struct field *f);

void f_1340(unsigned char id);

void f_1278(void);

void f_12e2(struct field *f, unsigned char a);

#pragma inline(set_flag)
static void set_flag(unsigned char n, unsigned char v)
{
	g_6079374[n] = v;
}

void f_1340(unsigned char id);

void f_1896(struct field *f);

#pragma inline(nop0)
static void nop0(void)
{
}

void f_18ca(struct field *f);

void f_1914(struct field *f);

void f_1c8c(struct field *f);

void f_1e64(struct field *f);

/* CONTEXT ONLY: not linked or counted. */
void f_1ede(struct field *f);

void f_1fce(struct field *f, short n);

/* CONTEXT ONLY: not linked or counted. */
void f_2058(struct field *f);

void f_20be(struct field *f)
{
	short x;
	short p;
	short o;
	short c;
	short i;
	short k;
	short s;
	short j;

	x = f->b30e == 0 ? 16 : 32;
	if (f->b2fc[0] != 30)
		f_10942(f);
	switch (f->b2fc[0]) {
	case 0:
		f->b2fc[0] = 10;
		f->w300[0] = 0;
		f->w300[1] = 0x293;
		for (i = 0; i < 4; i++)
			g_6079274[f->b30e][i] = -(i * 30);
		f->w300[0] = 0;
		f_1fce(f, f->w300[0]);
		f_2af5e(x, 16, ((long)d_64710));
		f_2af5e(15, 1, ((long)d_68ad0));
		f_2af5e(14, 1, ((long)d_68b10));
		if (f->other->b39a == 0) {
			if (f->other->u308.l308 & 0x1000) {
				if (!(f->other->u308.l308 & 1)) {
					s = f->other->u35c.b.b35d;
					if (s != 7 && s != 11 && !(f->other->u308.l308 & 0x20)) {
						f->b2fc[0] = 30;
						f->w300[0] = 1;
					}
					break;
				}
				if (f->other->b2fc[0]) {
					s = f->other->u35c.b.b35d;
					if (s != 7 && s != 11 && !(f->other->u308.l308 & 0x20)) {
						f->b2fc[0] = 30;
						f->w300[0] = 1;
					}
					break;
				}
			}
		}
	case 10:
		p = f->w314;
		for (i = 0; i < 4; i++) {
			if (g_6079274[f->b30e][i] < 63)
				g_6079274[f->b30e][i] += 8;
			else
				g_6079274[f->b30e][i] = 63;
		}
		f_11680(d_a7028, 70, p, 0, 125, 63, g_6079274[f->b30e][0], 0);
		f_e490(p - f_e39c(g_6032844) / 2, 90, g_6032844, f->w300[0] == 0 ? 15 : 14, 0);
		f_e490(p - f_e39c(g_603284c) / 2, 105, g_603284c, f->w300[0] == 1 ? 15 : 14, 0);
		f_e490(p - f_e39c(g_6032854) / 2, 122, g_6032854, f->w300[0] == 2 ? 15 : 14, 0);
		f_e490(p - f_e39c(g_603285c) / 2, 137, g_603285c, f->w300[0] == 3 ? 15 : 14, 0);
		if (f->other->u308.l308 & 0x10000000)
			f_e490(p - f_e39c(g_6032868) / 2, 154, g_6032868, f->w300[0] == 4 ? 15 : 14, 0);
		o = f->w300[0];
		k = f_29ea(f);
		if (k & 0x80) {
			if (f->w300[0] > 0)
				f->w300[0]--;
		} else if (k & 0x40) {
			if (f->other->u308.l308 & 0x10000000) {
				if (f->w300[0] < 4)
					f->w300[0]++;
			} else {
				if (f->w300[0] < 3)
					f->w300[0]++;
			}
		}
		if (f->other->u308.l308 & 0x10000000)
			f->w300[0] %= 5;
		else
			f->w300[0] %= 4;
		if (o != f->w300[0])
			f_2e6fc(13);
		f_1fce(f, f->w300[0]);
		switch (f->w300[0]) {
		case 0:
			f_2af5e(x, 16, ((long)d_64710));
			break;
		case 1:
			f_2af5e(x, 16, ((long)d_63f10));
			break;
		case 2:
			f_2af5e(x, 16, ((long)d_65310));
			break;
		case 3:
			f_2af5e(x, 16, ((long)d_64b10));
			break;
		case 4:
			f_2af5e(x, 16, ((long)d_63f10));
			break;
		}
		if (g_606475e[f->b30e] & 14) {
			if (f->w30c & 4) {
				f_2058(f);
			} else {
				f->b2fc[0] = 0xff;
				f_2e6fc(27);
				if (!(f->w30c & 0x1005))
					f_2efc(f);
			}
		} else if (--f->w300[1] == 0) {
			if (f->w30c & 4) {
				f_2058(f);
			} else {
				f->b2fc[0] = 0xff;
				f_2e6fc(27);
			}
		}
		break;
	case 20:
	case 21:
		break;
	case 30:
		p = f->w314;
		for (i = 0; i < 4; i++) {
			if (g_6079274[f->b30e][i] < 63)
				g_6079274[f->b30e][i] += 8;
			else
				g_6079274[f->b30e][i] = 63;
		}
		f_11680(d_a7034, 70, p, 0, 125, 63, g_6079274[f->b30e][0], 0);
		c = 100;
		for (j = 0; j < 2; j++) {
			if (g_6079274[f->b30e][j + 1] > 0)
				f_11680(d_36334[j] + (f->w300[0] == j ? 0 : 1) * 12, c, p, 0, 125, 63,
					g_6079274[f->b30e][j + 1], 0);
			c += 20;
		}
		o = f->w300[0];
		k = f_29ea(f);
		if (k & 0x80)
			f->w300[0] -= f->w300[0] > 0;
		else if (k & 0x40)
			f->w300[0] += f->w300[0] >= 1 ? 0 : 1;
		if (f->w300[0] == 0)
			f_10b28(f);
		if (o != f->w300[0])
			f_2e6fc(13);
		s = f->other->u35c.b.b35d;
		if ((f->other->u308.l308 & 0x10000000) | (s == 7) | (s == 11) || (f->other->u308.l308 & 0x20)) {
			g_6064880 &= ~0x1000;
			f->b2fc[0] = 10;
			f->w300[0] = 0;
			f->w300[1] = 0x293;
			break;
		}
		if (g_606475e[f->b30e] & 14)
			f_1ede(f);
		else if (--f->w300[1] == 0)
			f_1ede(f);
		break;
	case 0xff:
		if (f->w30c & 0x80) {
			f->w30c &= 0x6a0;
			f->w360 &= 15;
		}
		if (f->w30c & 0x1000) {
			f->w30c &= 0x1000;
			f->w360 &= 15;
		}
		f->u308.l308 = 0x1004;
		f_18ca(f);
		f_2f7a(f);
		g_6066198 |= f->b30e == 0 ? 2 : 4;
		break;
	}
}

unsigned char f_29ea(struct field *f)
{
	unsigned char idx = f->b30e;
	unsigned char dir = 0;

	if (g_606475a[idx] & 0x80) {
		if (g_606475e[idx] & 0x80) {
			f->b349 = 0;
			dir = 0x80;
		} else if (f->b349 < 14)
			f->b349++;
		else {
			f->b349 = 0;
			dir = 0x80;
		}
	}
	if (g_606475a[idx] & 0x40) {
		if (g_606475e[idx] & 0x40) {
			f->b349 = 0;
			dir = 0x40;
		} else if (f->b349 < 14)
			f->b349++;
		else {
			f->b349 = 0;
			dir = 0x40;
		}
	}
	return dir;
}

void f_2a92(void)
{
	unsigned char p;
	struct field *f;

	for (p = 0; p <= 1; p++) {
		f = &g_6064898[p];
		if (f->u308.l308 & 0x2000) {
			f_1340(p);
			f->u308.l308 &= ~0x2000;
			f->u308.l308 |= 0x40000;
			if (f->u308.l308 & 0x1000)
				f_2f7a(f);
		}
		if (!(f->u308.l308 & 0x63) &&
		    ((*(unsigned long *)0x06064880 & 0x80) | (*(unsigned long *)0x06064880 & 0x4000))) {
			f->u308.l308 |= 2;
			f->u308.l308 &= ~4;
			f->w300[0] = 0;
			if (g_606488a == 0) {
				g_6064880 |= 0x10000;
				f_21bc8(f);
			}
		}
		if (g_6060000 < 40) {
			f->b2fc[1] = f->u308.b[3];
			if (f->u308.l308 & 4)
				f_1914(f);
			else if (f->u308.l308 & 1)
				f_20be(f);
			else if (f->u308.l308 & 0x10)
				f_1c8c(f);
			else if (f->u308.l308 & 2)
				f_1e64(f);
		}
	}
	if (g_6064880 & 4) {
		if (g_6064898[0].l350 >= g_6064898[1].l350)
			g_6064898[1].l350 = g_6064898[0].l350;
		else
			g_6064898[0].l350 = g_6064898[1].l350;
	}
	f_191a4();
	f_10788();
}

void f_2c0a(void)
{
	unsigned char p;
	struct field *f;
	unsigned char flag;

	for (p = 0; p <= 1; p++) {
		f = &g_6064898[p];
		flag = f->w322 >= 100 ? 1 : 0;
		if ((f->w30c & 0x400) | (f->w30c & 8))
			flag = 0;
		f_15fae(f, 0, f->u308.l308 & 0x20000);
		f_15fae(f, 1, f->u308.l308 & 0x40000);
		if (!flag)
			f_15fae(f, 2, f->u308.l308 & 0x4000);
		if (g_6064880 & 4) {
			if (p == 1 && (f->u308.l308 & 0x10000))
				f_16938(f);
		} else if (f->u308.l308 & 0x10000)
			f_16938(f);
	}
}

void f_2d06(struct field *f)
{
	short i;

	for (i = 0; i < 3; i++)
		(g_607927c + (char)(f->b30e * 3))[i] = 0;
}

int f_2d54(struct field *f)
{
	unsigned char id = f->b30e;
	unsigned char *row = g_607927c + (char)(f->b30e * 3);
	unsigned short key = g_606475e[id];
	short i, j;
	int ret = 0;

	for (i = 0; i < 3; i++) {
		unsigned short *t = d_363a8[i];

		if (t[row[i]] == 0xffff)
			continue;
		if (key) {
			if (t[row[i]] == key)
				row[i]++;
			else
				row[i] = 0;
		}
		if (t[row[i]] == 0xffff) {
			f->w30c |= d_363bc[i];
			ret = 1;
			for (j = 0; j < 3; j++)
				if (i != j)
					row[j] = 0;
		}
	}
	return ret;
}

void f_2e4c(struct field *f);

void f_2efc(struct field *f);
