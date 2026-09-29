/* rom: 0x13484 len: 0x1530 func: f_13484 flags: -macsave=1 -optimize=1 -speed */
/* WIP (not byte-exact): f_13484..f_13848 exact; f_138a8 82%, f_13b9c 97%, f_13f50 99%, f_1421c 97% (register ties);
 * f_1441a/f_14670 (static, bsr from f_1421c) ~94-98% ignoring registers; f_14758 58%.  Object is 0x153c bytes vs 0x1530. */
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
struct w4 {
	short w[4];
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
extern struct res g_60794c0[];
extern unsigned long g_60794bc;
extern short g_607946c[3];
extern void f_2f68(struct field *, int);
extern void f_2e6fc(int);
extern int f_3127c(char *, char *, int);
extern char *d_3b230[];
extern char *d_3b2d8[];
extern char *d_3b2dc[];
extern char g_6033744[];
extern unsigned char g_606475a[];
extern unsigned char g_606475e[];
extern unsigned long g_6064880;
extern void f_228fc(struct field *);
struct frame { char pad[12]; };
extern void f_11680(struct frame *, short, short, unsigned char , short, short, short, unsigned char );
extern void f_1159c(struct frame *, long , long , unsigned char , short);
extern void f_18708(unsigned long, int, int);
extern void f_122cc(unsigned long, int, int);
extern void f_121fe(char *, short, short, char, short);
extern void f_11254(long, long, long, char, long, long, long, long);
extern struct frame d_a782c[], d_a7838[], d_a7844[], d_a788c[], d_a7850[];
extern struct frame d_a7898[], d_a78a4[], d_a78b0[], d_a78bc[], d_a78c8[], d_a78d4[];
extern struct frame *d_3b330[];
extern struct frame *d_3b13c[];
extern unsigned long g_6060008;
static void f_1441a(struct res *, unsigned char);
static void f_14670(struct res *, unsigned char);
static void f_14758(struct res *, unsigned char);

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

int f_138a8(struct res *o, struct rank *tab, void *aux, short pos, short n, unsigned short kind)
{
	struct field *f;
	struct field *p0;
	struct field *p1;
	short *a16;
	struct rank2 *a8;
	volatile int m;
	short i;
	volatile short j;
	short k;
	short *s;

	f = o->f;
	a16 = aux;
	a8 = aux;
	m = 0;
	j = pos + n - 1;
	p0 = &g_6064898[0];
	p1 = &g_6064898[1];
	for (i = n - 1; i >= 0; i--, j--) {
		m |= 1 << j;
		if (o->mask & (1 << j)) {
			tab[i].name = *(long *)o->e[0].name;
			switch (kind) {
			case 0:
				tab[i].v = (f->l354 & 0xfffff) | ((f->w38a & 31) << 27);
				if (f->w322 == 999)
					tab[i].v |= 0x4000000;
				if (f->b339 & 0x20)
					tab[i].v |= 0x2000000;
				s = &a16[i];
				*s = 0;
				for (k = 0; k < 6; k++)
					*s |= (f->b33a[k] & 3) << (k * 2);
				break;
			case 1:
				tab[i].v = (f->l330 & 0xfffff) | ((f->w38a & 31) << 27);
				if (f->w322 == 300)
					tab[i].v |= 0x4000000;
				if (f->b339 & 0x20)
					tab[i].v |= 0x2000000;
				break;
			case 2:
				tab[i].v = (f->l350 & 0xfffff) | ((f->w38a & 31) << 27);
				if (p0->w322 == 300 && p1->w322 == 300)
					tab[i].v |= 0x4000000;
				if (p0->b339 & 0x20)
					tab[i].v |= 0x2000000;
				a8[i].name = *(long *)o->e[1].name;
				a8[i].a = p0->w322;
				a8[i].b = p1->w322;
				break;
			}
			return m;
		}
		if (i > 0) {
			tab[i] = tab[i - 1];
			switch (kind) {
			case 0:
				a16[i] = a16[i - 1];
				break;
			case 2:
				*(struct w4 *)&a8[i] = *(struct w4 *)&a8[i - 1];
				break;
			}
		}
	}
	return m;
}

void f_13b9c(struct res *o)
{
	struct field *f;
	unsigned short i;
	struct rank *e;
	unsigned long *best;

	f = o->f;
	o->mask = f_135d8(f, 0);
	if (o->mask & 0xe000)
		g_60794bc |= f_138a8(o, &g_6065650.t[13], 0, 13, 3, 1);
	if (o->mask & 0x1c00000)
		f_138a8(o, g_6079474, 0, 22, 3, 1);
	if (o->mask & 0x1c00)
		g_60794bc |= f_138a8(o, &g_6065650.t[10], g_6065650.u, 10, 3, 0);
	if (o->mask & 0x380000)
		f_138a8(o, g_6079454, g_607946c, 19, 3, 0);
	if (o->mask & 0x70000)
		g_60794bc |= f_138a8(o, &g_6065650.t[16], g_6065650.s, 16, 3, 2);
	if (o->mask & 0xe000000)
		f_138a8(o, g_607948c, g_60794a4, 25, 3, 2);
	e = g_6065650.t;
	best = (unsigned long *)((char *)g_6079394 + (unsigned char)(f->id * 76));
	for (i = 0; i < 10; best++, e++, i++) {
		if (o->mask & (1 << i)) {
			e->name = *(long *)o->e[0].name;
			e->v = (*best & 0xfffff) | ((f->w38a & 31) << 27);
			if (f->w322 == 999)
				e->v |= 0x4000000;
			if (f->b339 & 0x20)
				e->v |= 0x2000000;
			g_60794bc |= 1 << i;
		}
	}
}

void f_13f10(struct ent *o, struct field *f)
{
	o->f = f;
	o->timer = 1800;
	o->pos = 0;
	o->cur = 0;
	o->rep = 12;
	o->wait = 30;
	o->name[0] = o->name[1] = o->name[2] = o->name[3] = *d_3b230[39];
	o->b4 = 0;
}

unsigned short f_13f50(struct ent *o)
{
	unsigned short done;
	unsigned char id;
	unsigned short hold;
	unsigned short trig;
	short k;

	done = 0;
	if (o->wait)
		o->wait--;
	hold = g_606475a[o->f->id];
	trig = g_606475e[o->f->id];
	switch (o->pos) {
	case 0:
	case 1:
	case 2:
		if (trig & 32) {
			o->cur++;
			o->cur %= 42;
			if (o->pos == 0 && o->cur == 40)
				o->cur = 41;
			o->rep = 12;
		}
		if (trig & 16) {
			if (o->cur-- == 0)
				o->cur = 41;
			if (o->pos == 0 && o->cur == 40)
				o->cur = 39;
			o->rep = 12;
		}
		if (hold & 0x30) {
			if (o->rep-- == 0) {
				if (hold & 32) {
					o->cur++;
					o->cur %= 42;
					if (o->pos == 0 && o->cur == 40)
						o->cur = 41;
				} else {
					if (o->cur-- == 0)
						o->cur = 41;
					if (o->pos == 0 && o->cur == 40)
						o->cur = 39;
				}
				o->rep = 12;
			}
		}
		if ((trig & 8) && o->wait == 0) {
			f_2e6fc(13);
			if (o->cur == 40) {
				if (o->pos == 0)
					break;
				o->pos--;
				o->name[o->pos] = ' ';
				if (o->pos == 0)
					o->cur = 0;
			} else if (o->cur == 41) {
				done = 1;
				o->pos = 4;
				o->cur = 39;
			} else {
				o->name[o->pos] = *d_3b230[o->cur];
				o->pos++;
				if (o->pos == 3)
					o->cur = 41;
			}
		} else if (trig & 4) {
			f_2e6fc(13);
			if (o->pos == 0)
				break;
			if (--o->pos == 0)
				o->cur = 0;
			o->name[o->pos] = ' ';
		}
		break;
	case 3:
		if (trig & 32) {
			if (o->cur++ == 41)
				o->cur = 40;
		}
		if (trig & 16) {
			if (o->cur-- == 40)
				o->cur = 41;
		}
		if (trig & 8) {
			f_2e6fc(13);
			if (o->cur == 40) {
				o->pos--;
				o->name[o->pos] = ' ';
			} else {
				done = 1;
				o->pos = 4;
				o->cur = 39;
			}
		} else if (trig & 4) {
			f_2e6fc(13);
			o->pos--;
			o->name[o->pos] = ' ';
		}
		break;
	case 4:
	default:
		return 1;
	}
	o->timer--;
	if (o->timer == 0) {
		done = 1;
		o->cur = 39;
	}
	if (done) {
		for (k = 0; f_3127c(d_3b2dc[k], g_6033744, 3); k++) {
			if (f_3127c(d_3b2dc[k], o->name, 3) == 0) {
				*(long *)o->name = *(long *)*d_3b2d8;
				break;
			}
		}
		o->name[3] = 0;
		f_2e6fc(9);
	}
	return done;
}

int f_1421c(struct field *f)
{
	struct res *st;
	unsigned short d;

	st = (struct res *)((signed char)(f->id * 60) + (char *)g_60794c0);
	if ((g_6064880 & 1) && ((unsigned char *)0x0606475a)[f->id] == 14)
		f->flags308 |= 0x80000;
	if (f->mode & 0x660)
		f->flags308 &= ~0x80000;
	if (f->w304 != 1)
		return;
	if (!(f->flags308 & 0x80000) && st->mask == 0) {
		f->flags308 &= ~0x100;
		f_2f68(f, 7);
		return 1;
	}
	st->b18 += 4;
	if (st->b18 > 63)
		st->b18 = 63;
	switch (st->state) {
	case 0:
		st->state++;
		st->w6 = 18;
		st->b18 = 4;
		f->flags308 |= 0x100;
		if (st->f->mode & 4) {
			f_13f10(&st->e[0], &g_6064898[0]);
			f_13f10(&st->e[1], &g_6064898[1]);
		} else {
			f_13f10(&st->e[0], st->f);
		}
	case 1:
		if (st->w6)
			st->w6--;
		if (st->f->mode & 4) {
			if (st->w6 == 0) {
				d = f_13f50(&st->e[0]);
				d &= f_13f50(&st->e[1]);
				if (d) {
					st->state++;
					st->w4 = 0xc0;
					f_13b9c(st);
				}
			}
			f_14758(st, 0);
		} else {
			if (st->w6 == 0 && f_13f50(&st->e[0])) {
				st->state++;
				st->w4 = 0xc0;
				f_13b9c(st);
			}
			f_1441a(st, 0);
		}
		break;
	case 2:
		if (st->f->mode & 4)
			f_14758(st, 0xff);
		else
			f_1441a(st, 0xff);
		if (--st->w4 < 0) {
			f->flags308 &= ~0x100;
			f_2f68(f, 7);
			if (f->flags308 & 0x80000)
				f->w300 = 1440;
			f_228fc(f);
			return 1;
		}
		break;
	}
	return 0;
}

static void f_1441a(struct res *st, unsigned char c)
{
	struct field *f;
	int x;
	char flash;
	struct ent *e;

	f = st->f;
	if ((f->mode & 0x1083) && st->mask == 0) {
		f_14670(st, c);
		return;
	}
	x = st->f->id * 160;
	f_11680(d_a782c, 42, x + 20, 0, 110, 63, st->b18, 0);
	f_11680(d_a7838, 85, x + 20, 0, 110, 63, st->b18, 0);
	if (f->mode & 2)
		f_11680(d_a7844, 128, x + 20, 0, 110, 63, st->b18, 0);
	else if (f->mode & 1)
		f_11680(d_a788c, 128, x + 20, 0, 110, 63, st->b18, 0);
	f_11680(d_a7850, 171, x + 20, 0, 110, 63, st->b18, 0);
	if (st->w6 == 0) {
		f_1159c(d_3b330[st->b16], 62, x + 38, 0, 110);
		f_1159c(d_3b330[st->b17], 105, x + 38, 0, 110);
		if (f->mode & 2)
			f_18708(f->l354, 148, x + 20);
		else if (f->mode & 1)
			f_122cc(f->l330, 148, x + 24);
		flash = (g_6060008 & 3) ? 8 : 0;
		e = &st->e[0];
		if (c == 0xff)
			f_121fe(e->name, 183, x + 36, flash, 110);
		else
			f_121fe(e->name, 183, x + 36, 0, 110);
		f_1159c(d_3b13c[*d_3b230[e->cur] - 32], 183, e->pos * 16 + x + 36, flash, 110);
	}
}

static void f_14670(struct res *st, unsigned char c)
{
	int x;
	char flash;
	struct ent *e;

	x = st->f->id * 160;
	f_11680(d_a7850, 105, x + 20, 0, 110, 63, st->b18, 0);
	if (st->w6 == 0) {
		flash = (g_6060008 & 3) ? 8 : 0;
		e = &st->e[0];
		if (c == 0xff)
			f_121fe(e->name, 117, x + 36, flash, 110);
		else
			f_121fe(e->name, 117, x + 36, 0, 110);
		f_1159c(d_3b13c[*d_3b230[e->cur] - 32], 117, e->pos * 16 + x + 36, flash, 110);
	}
}

static void f_14758(struct res *st, unsigned char c)
{
	char flash;
	struct ent *e;
	int z;

	z = 110;

	f_11680(d_a7898, 43, 107, 0, z, 63, st->b18, 0);
	f_11680(d_a78a4, 80, 107, 0, z, 63, st->b18, 0);
	f_11680(d_a78b0, 117, 107, 0, z, 63, st->b18, 0);
	f_11680(d_a78bc, 154, 107, 0, z, 63, st->b18, 0);
	f_11680(d_a78c8, 172, 107, 0, z, 63, st->b18, 0);
	f_11680(d_a78d4, 190, 107, 0, z, 63, st->b18, 0);
	if (st->w6 != 0)
		return;
	f_1159c(d_3b330[st->b16], 56, 138, 0, z);
	f_1159c(d_3b330[st->b17], 93, 138, 0, z);
	f_18708(st->f->l350, 130, 121);
	flash = (g_6060008 & 3) ? 8 : 0;
	e = &st->e[0];
	f_121fe(e->name, 167, 124, (c == 0xff || e->pos > 3) ? flash : 0, z);
	f_1159c(d_3b13c[*d_3b230[e->cur] - 32], 167, e->pos * 16 + 124, flash, z);
	f_11254(e->f->w322, 169, 189, 0, z, 3, 0, 2);
	e = &st->e[1];
	f_121fe(e->name, 185, 124, (c == 0xff || e->pos > 3) ? flash : 0, z);
	f_1159c(d_3b13c[*d_3b230[e->cur] - 32], 185, e->pos * 16 + 124, flash, z);
	f_11254(e->f->w322, 187, 189, 0, z, 3, 0, 2);
}
