/* rom: 0x2261c len: 0xa8c func: f_2261c flags: -macsave=1 -optimize=1 -speed */
/* f_2261c is the task f_21d94 spawns: it moves the 38 entries of t->r up the
 * field, draws the ones inside the y window through the per-player table at
 * g_607cddc, and when its timers run out either spawns f_22bde/f_22d94 (via
 * f_22baa/f_22d3a) or calls one of f_22e42..f_23048, which all end in
 * f_22f4e.
 * Shapes this needs: the owner is read through pp = &t->owner (add #20 then
 * mov.l @r4) with r = (struct roll *)(pp + 1); the loop's `continue`s make
 * SHC jump to the loop test and spill i; f_11680 in branch A and B ends in a
 * `continue` so their tails cross-jump; f->pos as short[2] so &f->pos is a
 * CSE temp; the tail test is an early return.  f_22bde wants a register
 * parameter plus a late copy t = p, f_22fd4/f_23048 a local copy u of t, and
 * f_22e42/f_22e74 (own) and f_22f10 (start) small inline helpers. */
struct frame { char pad[12]; };
struct cell {
	unsigned short flags;
	unsigned char b2;
	unsigned char b3;
	short b;
};
struct field {
	struct cell *cells;        /* 0x000 */
	char pad0[0xdf - 4];
	unsigned char width;       /* 0x0df */
	short pos[2];              /* 0x0e0 */
	char pad1[0x2f4 - 0xe4];
	struct field *f2f4;        /* 0x2f4 */
	char pad2[0x300 - 0x2f8];
	short w300;                /* 0x300 */
	char pad3[0x308 - 0x302];
	unsigned long flags308;    /* 0x308 */
	unsigned short mode;       /* 0x30c */
	unsigned char id;          /* 0x30e */
	char pad4[0x314 - 0x30f];
	short shake[2];            /* 0x314 */
	char pad5[0x322 - 0x318];
	unsigned short w322;       /* 0x322 */
	char pad6[0x338 - 0x324];
	unsigned char b338;        /* 0x338 */
	unsigned char b339;        /* 0x339 */
	char pad7[0x38a - 0x33a];
	short w38a;                /* 0x38a */
	char pad8[0x39e - 0x38c];
	unsigned char b39e;        /* 0x39e */
};
struct roll {
	long b0;                   /* 0x00 */
	char pad0[0xc - 4];
	short step;                /* 0x0c */
	short div;                 /* 0x0e */
	short xs[38];              /* 0x10 */
	short ys[38];              /* 0x5c */
	short y0;                  /* 0xa8 */
	short y21;                 /* 0xaa */
	short y13;                 /* 0xac */
	char pad1[0xb0 - 0xae];
	long l;                    /* 0xb0 */
};
struct task {
	char pad0[6];
	short w6;                  /* 0x06 */
	short w8;                  /* 0x08 */
	short wa;                  /* 0x0a */
	short wc;                  /* 0x0c */
	char pad1[0x10 - 0xe];
	void (*func)(struct task *); /* 0x10 */
	struct field *owner;       /* 0x14 */
	struct roll r;             /* 0x18 */
};
extern unsigned long g_6064880;
extern unsigned char g_606475a[];
extern unsigned char g_606475e[];
extern unsigned short g_6060000;
extern unsigned short g_607cdd8[];
extern struct frame *g_607cddc[];
extern char g_60356bc[];
extern struct frame d_aac28[], d_a80cc[], d_aaf4c[], d_a80b4[], d_a80e4[];
extern struct task *f_17614(void);
extern void f_17638(struct task *);
extern void f_2e6fc(int);
extern void f_44d4(struct field *, int);
extern void f_331a(struct field *);
extern void f_11680(struct frame *, short, short, short, short, short, short, short);
extern void f_1159c(struct frame *, int, int, int, int);
extern long f_2beca(int);
extern void f_17834(struct field *, int, int, int);
extern int f_e39c(char *);
extern void f_e490(int, int, char *, short, char);

void f_22baa(struct field *f);
void f_22bde(struct task *p);
void f_22d3a(struct field *f);
void f_22d94(struct task *t);
void f_22e42(struct task *t);
void f_22e74(struct task *t);
void f_22f10(struct task *t);
void f_22f4e(struct task *t, struct field *f, short n);
void f_22fd4(struct task *t);
void f_23048(struct task *t);

static struct field *start(struct task *t)
{
	struct field *f = t->owner;

	f->flags308 |= 0x80000000;
	return f;
}

static struct field *own(struct task *t)
{
	return t->owner;
}

#define LIST(f) ((struct frame **)((char *)g_607cddc + (short)((f)->id * 0x98)))

void f_2261c(struct task *t)
{
	struct field **pp = &t->owner;
	struct roll *r = (struct roll *)(pp + 1);
	struct field *f = *pp;
	short i, x, y;
	short lo, hi, v;

	if ((g_6064880 & 1) && g_606475a[f->id] == 14)
		f->flags308 |= 0x80000;
	if (f->mode & 0x660)
		f->flags308 &= 0xfff7ffff;
	lo = r->y0;
	hi = r->y21;
	if ((f->mode & 0x1081) && (g_606475a[f->id] & 1) ||
	    (f->mode & 4) && ((g_606475a[0] & 1) || (g_606475a[1] & 1))) {
		r->step = 3;
		r->div = 1;
	} else if (f->mode & 2) {
		r->step = 1;
		r->div = 4;
	} else {
		r->step = 1;
		r->div = 2;
	}
	for (i = 0; i < 38; i++) {
		v = r->ys[i];
		if (v > r->y21 && t->w6 % r->div == 0)
			if (i < 36 || i == 36 && v > r->y13 || i == 37 && v > r->y13 + 16)
				r->ys[i] -= r->step;
		if (t->wa <= 0 || r->l)
			continue;
		if (r->ys[i] > hi && r->ys[i] < lo) {
			if (r->ys[i] > lo - 8) {
				f_11680(LIST(f)[i], f->pos[1] + r->ys[i], r->xs[i] + f->pos[0], 0xb2, 70, 63 - (r->ys[i] - lo + 8) * 5, 63, 0);
				continue;
			}
			if (r->ys[i] < hi + 8) {
				f_11680(LIST(f)[i], f->pos[1] + r->ys[i], r->xs[i] + f->pos[0], 0xb2, 70, (r->ys[i] - hi) * 5 + 23, 63, 0);
				continue;
			}
			f_11680(LIST(f)[i], r->ys[i] + f->pos[1], r->xs[i] + f->pos[0], 0xb2, 70, 63, 63, 0);
		}
	}
	if (f->mode & 2) {
		if (t->w6 <= 3700) {
			if (f->flags308 & 64) {
				if ((f->b338 & 117) == 117 && !(f->mode & ~31)) {
					f_2e6fc(22);
					f->w38a = 18;
				}
				f_17638(t);
			}
		} else {
			r->l = 1;
			f->b339 |= 32;
			f->flags308 |= 0x80000000;
			if ((f->b338 & 117) == 117 && !(f->mode & ~31)) {
				if (f->b39e >= 32) {
					f->w38a = 19;
					f_2e6fc(22);
					f_22baa(f);
				} else {
					f->w38a = 19;
					f_2e6fc(22);
					f_22baa(f);
					f->b339 &= 0xdf;
				}
			} else
				f_22d3a(f);
			f_44d4(f, 2);
			for (y = 1; y < 21; y++)
				for (x = 1; x < 11; x++) {
					f->cells[y * f->width + x].flags &= 0xefff;
					f->cells[y * f->width + x].flags &= 0xbfff;
				}
			f_17638(t);
		}
	} else if (t->w6 >= 2220) {
		f->flags308 &= 0x7fffffff;
		if (!(f->flags308 & 64)) {
			f->b339 |= 32;
			f_44d4(f, 2);
			if (g_6064880 & 4)
				f_44d4(f->f2f4, 2);
		}
		f_331a(f);
		f_17638(t);
	} else if (t->wa <= 0) {
		r->l = 1;
		if (f->mode & 0x1000) {
			if (f->w322 >= 999)
				f_22fd4(t);
			else
				f_23048(t);
		} else if (f->mode & 128)
			f_22e74(t);
		else if (f->mode & 1)
			f_22e42(t);
		else
			f_22f10(t);
	}
	if (g_6060000 >= 40)
		return;
	t->w6 += r->step == 3 ? r->step * 2 : r->step;
	if (r->ys[36] <= r->y13)
		t->wa -= r->step == 3 ? r->step * 2 : r->step;
}

void f_22baa(struct field *f)
{
	struct task *t;
	struct field **pp;

	if ((t = f_17614()) != 0) {
		pp = &t->owner;
		t->func = f_22bde;
		t->w6 = 600;
		t->w8 = 1;
		*pp = f;
		f->w300 = 123;
	}
}

void f_22bde(register struct task *p)
{
	struct field **pp = &p->owner;
	register struct field *f = *pp;
	struct task *t;
	register short v;
	register int dx, dy;
	int i;

	if ((g_6064880 & 1) && g_606475a[f->id] == 14)
		f->flags308 |= 0x80000;
	if (f->mode & 0x660)
		f->flags308 &= 0xfff7ffff;
	f->flags308 |= 0x80000000;
	if (f->w300 > 63)
		f->w300 -= 2;
	else
		f->w300 = 63;
	v = f->w300;
	dx = -((v - 63) * 1024 / 63 * 40) >> 10;
	dy = -((v - 63) * 1024 / 63 * 24) >> 10;
	for (i = 0; i < 1; i++)
		f_11680(d_aac28, dy + 120, f->shake[i] + dx, 0xb3, 125, v, v, i);
	t = p;
	if (g_6060000 < 40) {
		f_22f4e(t, f, 15);
		if (--t->w6 == 0) {
			f->flags308 &= 0x7fffffff;
			f_331a(f);
			f_17638(t);
		}
	}
}

void f_22d3a(struct field *f)
{
	struct task *t;
	struct field **pp;

	if ((t = f_17614()) != 0) {
		pp = &t->owner;
		t->func = f_22d94;
		t->w6 = 600;
		t->w8 = 1;
		*pp = f;
	}
}

void f_22d94(struct task *t)
{
	struct field **pp = &t->owner;
	struct field *f = *pp;

	if ((g_6064880 & 1) && g_606475a[f->id] == 14)
		f->flags308 |= 0x80000;
	if (f->mode & 0x660)
		f->flags308 &= 0xfff7ffff;
	f_1159c(d_a80cc, 100, f->shake[0], 0, 125);
	f_1159c(d_aaf4c, 140, f->shake[0], 0, 125);
	if (g_6060000 < 40) {
		f_22f4e(t, f, 30);
		if (--t->w6 == 0) {
			f->flags308 &= 0x7fffffff;
			f_331a(f);
			f_17638(t);
		}
	}
}

void f_22e42(struct task *t)
{
	struct field *f = own(t);

	f->flags308 |= 0x80000000;
	f_22f4e(t, f, 15);
	f_1159c(d_a80b4, 120, f->shake[0], 0, 125);
}

void f_22e74(struct task *t)
{
	struct field *f = own(t);
	int i;

	f->flags308 |= 0x80000000;
	f_22f4e(t, f, 15);
	i = 0;
	f_e490(f->shake[i] - f_e39c(g_60356bc) / 2, 120, g_60356bc, 15, i);
}

void f_22f10(struct task *t)
{
	struct field *f = start(t);

	f->f2f4->flags308 |= 0x80000000;
	f_22f4e(t, f, 15);
	f_1159c(d_a80e4, 120, f->shake[0], 0, 125);
}

void f_22f4e(struct task *t, struct field *f, short n)
{
	if (g_606475e[f->id] & 0xfe)
		t->wc++;
	if (g_607cdd8[f->id]-- == 0 || t->wc % 10 == 0) {
		g_607cdd8[f->id] = n;
		t->wc = 1;
		f_17834(f, f_2beca(5) + 13, f_2beca(10), f_2beca(10));
		f_2e6fc(32);
	}
}

void f_22fd4(struct task *t)
{
	struct task *u = t;
	struct field *f = t->owner;
	int i;

	f->flags308 |= 0x80000000;
	for (i = 0; i < 1; i++)
		f_11680(d_aac28, 100, f->shake[i], 0xb3, 125, 63, 63, i);
	f_22f4e(u, f, 15);
}

void f_23048(struct task *t)
{
	struct task *u = t;
	struct field *f = t->owner;
	int i;

	f->flags308 |= 0x80000000;
	for (i = 0; i < 1; i++)
		f_e490(f->shake[i] - f_e39c(g_60356bc) / 2, 120, g_60356bc, 15, i);
	f_22f4e(u, f, 30);
}
