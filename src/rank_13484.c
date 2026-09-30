/* rom: 0x13484 len: 0x424 func: f_13484 flags: -macsave=1 -optimize=1 -speed */
/* Ranking / rank-screen helpers: prefix of the 0x13484 unit (f_13484..f_13848);
 * f_138a8 onwards is still WIP. */
struct field {
	char pad0[0x300];
	short w300;                /* 0x300 */
	char pad0a[0x304 - 0x302];
	short w304;                /* 0x304 */
	char pad0b[0x308 - 0x306];
	unsigned long flags308;    /* 0x308 */
	unsigned short mode;       /* 0x30c */
	unsigned char id;          /* 0x30e */
	char pad0c[0x322 - 0x30f];
	unsigned short w322;       /* 0x322 */
	char pad1[0x330 - 0x324];
	unsigned long l330;        /* 0x330 */
	char pad2[0x339 - 0x334];
	unsigned char b339;        /* 0x339 */
	unsigned char b33a[6];     /* 0x33a */
	char pad3[0x350 - 0x340];
	unsigned long l350;        /* 0x350 */
	unsigned long l354;        /* 0x354 */
	char pad4[0x38a - 0x358];
	unsigned short w38a;       /* 0x38a */
	char pad5[0x3b4 - 0x38c];
};
struct rank {
	long name;
	unsigned long v;
};
struct rank2 {
	long name;
	unsigned short a;
	unsigned short b;
};
struct ent {
	char name[4];
	char b4;
	char pad0[3];
	struct field *f;           /* 0x08 */
	short timer;               /* 0x0c */
	char pos;                  /* 0x0e */
	char cur;                  /* 0x0f */
	char rep;                  /* 0x10 */
	char wait;                 /* 0x11 */
	char pad1[2];
};
struct res {
	unsigned char state;       /* 0x00 */
	char pad0;
	short timer;               /* 0x02 */
	short w4;                  /* 0x04 */
	short w6;                  /* 0x06 */
	struct field *f;           /* 0x08 */
	long mask;                 /* 0x0c */
	unsigned char b16;         /* 0x10 */
	unsigned char b17;         /* 0x11 */
	unsigned char b18;         /* 0x12 */
	char pad1;
	struct ent e[2];           /* 0x14 */
};
struct save {
	char pad0[0x44];
	struct rank t[19];         /* 0x44 */
	struct rank2 s[3];         /* 0xdc */
	short u[3];                /* 0xf4 */
};
extern struct save g_6065650;
extern struct field g_6064898[2];
extern struct rank g_6079454[3];
extern struct rank g_6079474[3];
extern struct rank g_607948c[3];
extern struct rank2 g_60794a4[3];
extern unsigned long g_6079394[];
extern void f_2f68(struct field *, int);

short f_13484(struct field *f, struct rank *r, short n)
{
	short i;

	for (i = 0; i < n; i++, r++) {
		if (f->w38a > ((r->v >> 27) & 31))
			return i;
		if (f->w38a == ((r->v >> 27) & 31)) {
			if (r->v & 0x2000000) {
				if (!(f->b339 & 0x20))
					continue;
			} else {
				if (f->b339 & 0x20)
					return i;
			}
			if (r->v & 0x4000000) {
				if (f->w322 != 999)
					continue;
			} else {
				if (f->w322 == 999)
					return i;
			}
			if (f->l354 < (r->v & 0xfffff))
				return i;
		}
	}
	return -1;
}

short f_13532(struct field *f, struct rank *r, short n)
{
	short i;

	for (i = 0; i < n; i++, r++) {
		if (f->l330 > (r->v & 0xfffff))
			return i;
	}
	return -1;
}

short f_13578(unsigned short a, unsigned long b, struct rank *r, struct rank2 *s, short n)
{
	short i;

	for (i = 0; i < n; i++, r++) {
		if (a > s[i].a + s[i].b)
			return i;
		if (a == s[i].a + s[i].b) {
			if (b < (r->v & 0xfffff))
				return i;
		}
	}
	return -1;
}

int f_135d8(struct field *f, struct res *o)
{
	struct res tmp;
	int m;
	short k;
	short i;
	unsigned long *best;
	struct rank *p;

	m = 0;
	if (o == 0)
		o = &tmp;
	o->b16 = o->b17 = 3;
	if (f->mode & 4) {
		k = f_13578(g_6064898[0].w322 + g_6064898[1].w322, f->l350, &g_6065650.t[16], g_6065650.s, 3);
		if (k >= 0) {
			m |= 1 << (k + 16);
			o->b16 = k;
		}
		k = f_13578(g_6064898[0].w322 + g_6064898[1].w322, f->l350, g_607948c, g_60794a4, 3);
		if (k >= 0) {
			m |= 1 << (k + 25);
			o->b17 = k;
		}
	} else if (f->mode & 2) {
		k = f_13484(f, &g_6065650.t[10], 3);
		if (k >= 0) {
			m |= 1 << (k + 10);
			o->b16 = k;
		}
		k = f_13484(f, g_6079454, 3);
		if (k >= 0) {
			m |= 1 << (k + 19);
			o->b17 = k;
		}
		p = g_6065650.t;
		best = (unsigned long *)((char *)g_6079394 + (unsigned char)(f->id * 76));
		for (i = 0; i < 10; p++, best++, i++) {
			if (*best != 0 && *best < (p->v & 0xfffff))
				m |= 1 << i;
		}
	} else if (f->mode & 1) {
		k = f_13532(f, &g_6065650.t[13], 3);
		if (k >= 0) {
			m |= 1 << (k + 13);
			o->b16 = k;
		}
		k = f_13532(f, g_6079474, 3);
		if (k >= 0) {
			m |= 1 << (k + 22);
			o->b17 = k;
		}
	}
	return m;
}

int f_13848(struct field *f)
{
	int m;
	struct res *r;

	m = f_135d8(f, (struct res *)((signed char)(f->id * 60) + (char *)0x060794c0));
	f_2f68(f, 11);
	r = (struct res *)((signed char)(f->id * 60) + (char *)0x060794c0);
	*(long *)r = 0;
	r->f = f;
	r->mask = m;
	return m;
}
