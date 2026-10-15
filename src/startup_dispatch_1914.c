/* rom: 0x1914 len: 0x378 func: f_1914 context-rom: 0x1278 guarded-switches: 1 flags: -macsave=1 -optimize=1 -speed */
/* Retain only the complete startup dispatcher 0x1914-0x1c8c.
 * Surrounding genuine functions supply native branch and literal context.
 * The guarded 13-entry switch is checked against original compiler ELF,
 * its complete control-flow graph and the reference ROM. Context is excluded.
 */
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

void f_1278(void)
{
	unsigned char i;

	for (i = 0; i <= 1; i++) {
		g_6064898[i].b30e = i;
		g_6064898[i].u308.l308 = 0x10000010;
		g_6064898[i].w30c = 2;
		f_1340(i);
	}
}

void f_12e2(struct field *f, unsigned char a)
{
	short id = f->b30e;
	struct pos *p;

	if (g_6064880 & 4) {
		f->w314 = 160;
		f->w316 = d_36304[id].y;
	} else {
		if ((g_6064880 & 2) && a) {
			f->w314 = d_36304[id + 2].x;
			p = &d_36304[id + 2];
		} else {
			p = &d_36304[id];
			f->w314 = p->x;
		}
		f->w316 = p->y;
	}
}

#pragma inline(set_flag)
static void set_flag(unsigned char n, unsigned char v)
{
	g_6079374[n] = v;
}

void f_1340(unsigned char id)
{
	struct field *f = &g_6064898[id];
	struct cell *c;
	unsigned char i, j;
	unsigned char p;
	unsigned char ok;
	int k;

	f->other = &g_6064898[id == 0];
	if (f->w30c & 8)
		k = id == 0 ? 1 : 0;
	else
		k = id == 0 ? 0 : 1;
	f->p2f8 = &g_6064898[k];
	for (i = 0; i < 4; i++)
		f->b2fc[i] = 0;
	for (i = 0; i < 4; i++)
		f->w300[i] = 0;
	f->u308.l308 &= 0xbfeffe1f;
	f->u308.l308 &= 0xfff9bfff;
	f->u308.l308 |= 0x18000;
	f->b347 = 30;
	f->b348 = 30;
	f->b349 = 0;
	if (g_6064880 & 2)
		f->l350 = d_36314[g_6060038] / 100 * 0x12c0;
	else
		f->l350 = 0;
	f->w322 = f->w324 = f->w326 = f->l384 = f->b388 = 0;
	f->l358 = 0;
	f->b346 = 0;
	f->w328 = 1;
	f->b399 = 0;
	f->b398 = 0;
	f->b39a = 0;
	f->b39f = 0;
	f->b39e = 0;
	f->b3a0 = 17;
	f->b3a1 = 0;
	f->u35c.w35c = 10;
	f->w35e = 2;
	f->b362 = 0;
	f->l368 = 0x15ffff;
	if (g_6064880 & 4)
		f->l364 = f->b30e == 0 ? 0x30000 : 0xa0000;
	else
		f->l364 = 0x40000;
	f->l370 = 0x7ffff;
	f->l374 = 0x7ffff;
	f->b37e = f->w37c = f->b378 = 0;
	f->b379 = 0;
	f->b37f = 0;
	f->b380 = 0;
	g_6079374[id] = 0;
	f->w38c = 0;
	f->w390 = 0;
	f->w38e = 0;
	f->w38a = 0;
	f->w318 = f->w31a = f->w31c = f->w31e = f->w320 = f->l330 = f->l334 = f->b339 = f->w344
		= f->b340 = f->c341 = f->c342 = f->w396 = f->w392 = f->w394 = f->w32a = f->c343 = 0;
	f->b338 = 48;
	for (i = 0; i < 6; i++)
		f->b33a[i] = 0;
	if ((g_6064880 & 4) && g_606487c == 0)
		g_60657a0 += f_2beca(0x4a8) + 1;
	if ((g_6064880 & 1) && g_606487c == 0)
		g_60657a0 += f_2beca(0x4a8) + 1;
	f->l310 = g_60657a0;
	p = f_2f3c(&f->l310, 1) % 7;
	ok = 1;
	if (p == 1 || p == 2 || p == 5) {
		do {
			p = f_2f3c(&f->l310, 1) % 7;
			if (p != 1 && p != 2 && p != 5)
				ok = 0;
		} while (ok);
	}
	f->w360 = p + 2;
	for (i = 0; i < 2; i++)
		f->b36c[i] = 1;
	for (; i < 4; i++)
		f->b36c[i] = 2;
	f->b36c[0] = f->w360 - 2;
	if (g_6064880 & 4) {
		f->cells = g_6078608[0];
		f->bdc = 20;
		f->bdd = 14;
		f->bde = 22;
		f->bdf = 16;
	} else {
		f->cells = g_6078608[f->b30e];
		f->bdc = 20;
		f->bdd = 10;
		f->bde = 22;
		f->bdf = 12;
	}
	for (i = 0; i < 8; i++)
		for (j = 0; j < 12; j++)
			f->w4[i][j] = 0;
	c = f->cells;
	if (!(4 & g_6064880) || id != 1) {
		for (i = 0; i < f->bde; i++) {
			for (j = 0; j < f->bdf; j++) {
				if (i == 0)
					c[i * f->bdf + j].w0 = 1;
				else if (j == 0 || j == f->bdf - 1)
					c[i * f->bdf + j].w0 = 1;
				else
					c[i * f->bdf + j].w0 = 0;
				c[i * f->bdf + j].b3 = 0;
			}
		}
	}
	f_12e2(f, 1);
	f->e0[0] = f->e0[1] = 0;
	f_15f10(f, 0);
	f_2d06(f);
	f_1175c(f);
	f_21454(f, 67, f->w314 + 49);
	set_flag(f->b30e, 0);
	if (f->w30c & 0x40)
		f->w360 |= 0x200;
}

void f_1896(struct field *f)
{
	short i;

	for (i = 0; i < 4; i++)
		f->b2fc[i] = 0;
	for (i = 0; i < 4; i++)
		f->w300[i] = 0;
}

void f_18ca(struct field *f)
{
	f_1896(f);
	f->u308.l308 |= 0x58000;
	f_1175c(f);
}

void f_1914(struct field *f)
{
	int flag = 0;
	unsigned char os, st;

	if (f->u308.l308 & 0x80000000)
		return;
	if (!(g_606475a[f->b30e] & 0x40))
		g_6079288[f->b30e] |= 1;
	os = f->other->u35c.w35c;
	st = f->u35c.w35c;
	if (st != 7 && st != 11 && !(f->u308.l308 & 0x20) && !(f->w30c & 8)) {
		if (g_606475e[f->b30e] & 1) f->b39a = f->b39a ? 0 : 1;
	}
	switch (st) {
	case 10:
		flag = 1;
		f_7c08(f);
		break;
	case 12:
		f_7f38(f);
		break;
	case 1:
		if (g_6064880 & 4) {
			f_50e0(f);
			if (os != 1 && os != 8)
				f_44d4(f->other, 1);
		}
		f_61d0(f);
		if ((g_6064880 & 4) && !(f->u308.l308 & 0x40) && !(f->other->u308.l308 & 0x40))
			f_5168(f);
		break;
	case 2:
		f->w39c++;
		if (g_6064880 & 4) {
			f_50e0(f);
			if (os != 1 && os != 8)
				f_44d4(f->other, 1);
		}
		f_4044(f);
		if ((g_6064880 & 4) && !(f->u308.l308 & 0x40) && !(f->other->u308.l308 & 0x40))
			f_5168(f);
		break;
	case 3:
		if (g_6064880 & 2)
			f_50e0(f);
		f->u308.l308 &= ~0x20000;
		f_44d4(f, 0);
		f_57d4(f);
		break;
	case 4:
		f_5b80(f);
		break;
	case 5:
		f_7264(f);
		break;
	case 6:
		f_67f4(f);
		break;
	case 7:
		flag = 1;
		f_7324(f);
		break;
	case 8:
		if (g_6064880 & 4) {
			f_50e0(f);
			if (os != 1 && os != 8)
				f_44d4(f->other, 1);
		}
		f_667c(f);
		if ((g_6064880 & 4) && !(f->u308.l308 & 0x40) && !(f->other->u308.l308 & 0x40))
			f_5168(f);
		break;
	case 9:
		flag = 1;
		f_7956(f);
		break;
	case 11:
		flag = 1;
		f_1421c(f);
		break;
	case 13:
		flag = 1;
		f_8430(f);
		break;
	}
	if (f->u308.l308 & 0x40)
		flag = 1;
	if (g_6064880 & 4) {
		if ((f->u308.l308 & 0x20) && (f->other->u308.l308 & 0x20))
			flag = 1;
	} else if (f->u308.l308 & 0x20) {
		flag = 1;
	} else if (!(d_38f6a[0] > f->w322)) {
		flag = 1;
	}
	if (!flag) {
		if (g_6064880 & 2) {
			if (f->l350)
				f->l350--;
		} else {
			f->l350++;
		}
		if (f->p2f8->b379 != 9)
			f_2e4c(f);
		if (f->l350 > 0x5879f)
			f->l350 = 0x5879f;
		if (f->l350 > 0xd2f0)
			f->w30c |= 0x2000;
	}
	f->l34c++;
}

#pragma inline(start_field)
static void start_field(struct field *f, int zero)
{
	f->u308.l308 = 0x1001;
	f->w30c &= 0xfff0;
	f->w30c |= 1;
	f->l34c = zero;
	f_1340(f->b30e);
	if (f->other->u308.l308 & 0x10000000)
		g_6064884.b8 = f->b30e;
	if (g_6064884.b8 == f->b30e)
		g_6064884.b7 = zero;
	g_6064884.b4[f->b30e] = zero;
}

void f_1c8c(struct field *f)
{
	unsigned short x = f->w314;

	if (f->other->u308.l308 & 1) {
		if (g_6060008 & 0x60)
			f_1159c(d_aaa24, 120, f->w314, 0, 110);
		return;
	}
	f_1050e(f);
	if (f->other->b39a) {
		f_e490(x - f_e39c(g_6032830) / 2, 65, g_6032830, 15, 0);
		f_e490(x - f_e39c(g_6032838) / 2, 77, g_6032838, 15, 0);
	}
	if (f_2da90(f->b30e, 0))
		f_2d54(f);
	else
		f->w30c &= 0xfb9f;
	if (g_606475e[f->b30e] & 1) {
		if (f_2da90(f->b30e, 0)) {
			f_2db32(f->b30e, 0);
			start_field(f, 0);
		}
	}
	f_10942(f);
}

/* RETAINED C RANGE: f_1e64. */
void f_1e64(struct field *f)
{
	if (++f->w300[0] > 3600 || (g_6064880 & 0x40) || (f->other->u308.l308 & 0x40)) {
		f->u308.l308 = 0x1004;
		f_1340(f->b30e);
		f_2f7a(f);
	}
	if (!g_606488a && (!(f->u308.l308 & 2) || !(f->other->u308.l308 & 2)) && (f->w300[0] & 0x60))
		f_1159c(d_aaa24, 120, f->w314, 0, 110);
}

/* CONTEXT ONLY: not linked or counted. */
void f_1ede(struct field *f)
{
	short i;

	g_6064880 &= ~0x1000;
	if (f->w300[0] == 0) {
		if (f->b30e == 0)
			g_6064880 |= 0x10;
		else
			g_6064880 |= 0x20;
		f->u308.l308 = 0x1002;
		f_18ca(f);
		f_2e6fc(27);
		g_606488c = f->other->b30e;
	} else if (f->w300[0] == 1) {
		f->b2fc[0] = 10;
		f->w300[0] = 0;
		f->w300[1] = 0x293;
		for (i = 0; i < 4; i++)
			g_6079274[f->b30e][i] = -(i * 30);
		g_606568a.a++;
		f_2e6fc(27);
	} else if (f->w300[0] == 2) {
	}
}

/* RETAINED C RANGE: f_1fce and its literal pool. */
void f_1fce(struct field *f, short n)
{
	f->w30c &= 0xef78;
	switch (n) {
	case 0:
		f->w30c |= 1;
		break;
	case 1:
		f->w30c |= 2;
		break;
	case 2:
		f->w30c |= 0x80;
		break;
	case 3:
		f->w30c |= 0x1000;
		break;
	case 4:
		f->w30c |= 4;
		break;
	}
}

/* CONTEXT ONLY: not linked or counted. */
void f_2058(struct field *f)
{
	g_6064884.b0 = 0;
	g_6064880 |= 0x20000;
	g_6064898[0].w360 &= 15;
	g_6064898[1].w360 &= 15;
	g_6064898[0].u308.l308 = 0x1004;
	g_6064898[1].u308.l308 = 0x1004;
	f->u308.l308 |= 0x80000000;
	f->other->u308.l308 |= 0x80000000;
	f_18ca(f);
	f_18ca(f->other);
	g_606568a.c++;
	f_2e6fc(27);
}

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
		f_2af5e(x, 16, 0x64710);
		f_2af5e(15, 1, 0x68ad0);
		f_2af5e(14, 1, 0x68b10);
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
			f_2af5e(x, 16, 0x64710);
			break;
		case 1:
			f_2af5e(x, 16, 0x63f10);
			break;
		case 2:
			f_2af5e(x, 16, 0x65310);
			break;
		case 3:
			f_2af5e(x, 16, 0x64b10);
			break;
		case 4:
			f_2af5e(x, 16, 0x63f10);
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
