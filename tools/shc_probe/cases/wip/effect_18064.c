/* rom: 0x18064 len: 0x58c func: f_18064 flags: -macsave=1 -optimize=1 -speed */
/* Work in progress. f_18064, f_18240 and f_182d6 match 100% here and also
 * appended to src/effect_176e0.c (the file probably continues); f_183a0
 * (piece-shaped sprite draw, called with bsr from f_182d6) is at 73.5%:
 * the structure matches but register priorities rotate (flags/zero/sz in
 * r13/r12/r11 instead of r11/r13/r12) and the 0x400/0x2000 constants swap. */
struct frame { char pad[12]; };
struct cell { unsigned short v; char pad[4]; };  /* v & 15: block colour */
struct sprite {
	struct frame *frames;      /* 0x18 */
	short w4;                  /* 0x1c */
	short y;                   /* 0x1e */
	short x;                   /* 0x20 */
};
/* A task slot (0x114 bytes) as these objects use it. */
struct task {
	char pad0[6];
	short tick;                /* 0x06 */
	short n;                   /* 0x08 */
	short pad1;
	short row;                 /* 0x0c */
	short pad2;
	void (*func)(struct task *); /* 0x10 */
	struct field *owner;       /* 0x14 */
	struct sprite spr;         /* 0x18 */
};
struct fx {                    /* the same, from 0x14 */
	struct field *owner;       /* 0x14 */
	struct sprite spr;         /* 0x18 */
};
struct fx2 {                   /* from 0x18 */
	struct frame *frames[2];   /* 0x18 */
	short w;                   /* 0x20 */
	short y[2];                /* 0x22 */
	short x[2];                /* 0x26 */
};
struct fx3 {                   /* from 0x18 */
	struct frame *frames[14];  /* 0x18 */
	short w[14];               /* 0x50 */
	short y;                   /* 0x6c */
};
struct point { short x, y; };
struct field {
	struct cell *cells;        /* 0x000, row-major, width per row */
	char pad0[0xde - 4];
	unsigned char height;      /* 0x0de */
	unsigned char width;       /* 0x0df */
	struct point pos;          /* 0x0e0 */
	char pad1[0x30c - 0xe4];
	unsigned short mode;       /* 0x30c */
	unsigned char id;          /* 0x30e */
	char pad2[0x314 - 0x30f];
	struct point shake;        /* 0x314 */
	char pad3[0x347 - 0x318];
	signed char x347;          /* 0x347 */
	char pad4[0x35e - 0x348];
	unsigned short x35e;       /* 0x35e */
	char pad5[0x362 - 0x360];
	unsigned char x362;        /* 0x362 */
	char pad6[0x364 - 0x363];
	short x364;                /* 0x364 */
	short pad7;
	short x368;                /* 0x368 */
	char pad8[0x37f - 0x36a];
	unsigned char x37f;        /* 0x37f */
};
struct fx4 {                   /* from 0x18 */
	long pad;
	short b;                   /* 0x1c */
	short a;                   /* 0x1e */
	short flags;               /* 0x20 */
	short n;                   /* 0x22 */
	unsigned char rot;         /* 0x24 */
};
extern struct task *f_17614(void);
extern void f_17638(struct task *);
extern void f_1159c(struct frame *, int, int, int, int);
extern void f_11680(struct frame *, short, short, short, short, short, short, short);
extern long f_2beca(int);
extern unsigned short g_6060000;
extern long g_6060008;
extern struct frame *d_3b3f4[];
extern short d_3b340[];
extern unsigned char d_3b414[];
extern struct frame d_aacd0[];
extern struct frame d_aaea4[];

extern struct frame d_a5d5c[];
extern struct frame d_a5e1c[];
extern struct frame d_a5e28[];
extern short d_3b356[];
extern unsigned char d_363cc[][4][4][4];

void f_18064(struct task *t)
{
	struct fx *e = (struct fx *)&t->owner;
	struct fx3 *s = (struct fx3 *)&e->spr;
	struct field *f = e->owner;
	short i;
	int y, x;

	y = s->y;
	x = f->shake.x - 40;
	for (i = 0; i < 10; i++, x += 8) {
		if (s->frames[i])
			f_1159c(&s->frames[i][t->tick], y, x, (char)s->w[i] + 9, 125);
	}
	if (g_6060000 >= 40)
		return;
	t->tick++;
	if (t->tick >= 32)
		f_17638(t);
}

void f_182d6(struct task *t);

void f_18240(struct field *f)
{
	struct task *t;
	struct fx4 *s;
	struct point *d, *p;
	short x, y;

	if ((t = f_17614()) != 0) {
		t->func = f_182d6;
		t->owner = f;
		s = (struct fx4 *)&t->spr;
		t->tick = 1;
		t->n = -4;
		d = &f->shake;
		p = &f->pos;
		x = d->x + p->x;
		y = d->y + p->y;
		s->a = f->x364 * 8 + x - (f->width / 2) * 8;
		s->b = (f->height - f->x368 - 2) * 8 + y - (f->height - 2) * 8 - 6;
		s->flags = f->x35e;
		s->rot = f->x362;
		s->n = 8;
	}
}

void f_183a0(struct field *f, short a, short b, short n, short flags, unsigned char rot);

void f_182d6(struct task *t)
{
	struct fx *e = (struct fx *)&t->owner;
	struct fx4 *s = (struct fx4 *)&e->spr;
	struct field *f = e->owner;
	short a, b;

	s->b += t->n;
	b = s->b;
	a = f->id == 0 ? s->a - t->tick : s->a + t->tick;
	if (t->tick % 5 == 0)
		s->n++;
	f_183a0(f, a, b, s->n, s->flags, s->rot);
	if (g_6060000 >= 40)
		return;
	t->tick++;
	if (t->tick & 1)
		t->n++;
	if (s->b > 320 || t->tick > 120)
		f_17638(t);
}

void f_183a0(struct field *f, short x, short y, short n, short flags, unsigned char rot)
{
	struct frame *gfx;
	unsigned char *p;
	short c, col, k, sz, step, i, j, xx;

	if (flags & 0x200) {
		x -= 2;
		y++;
	}
	c = f->x347 / 6;
	if (f->x347 % 6 > 0)
		c++;
	if (c <= 0)
		c = 0;
	else if (c >= 4)
		c = 4;
	c += 4;
	if (flags & 0x400)
		gfx = d_a5e1c;
	else if (flags & 0x2000) {
		gfx = d_a5e28;
		gfx += f->x37f - 1;
	}
	else
		gfx = d_a5d5c;
	if (flags & 0x2000)
		col = d_3b356[f->x37f - 1];
	else if (flags & 0x800)
		col = 48;
	else if (flags & 0x400)
		col = 128;
	else if (flags & 0x100)
		col = d_3b340[f_2beca(7)];
	else
		col = d_3b340[(flags & 15) - 2];
	c += col;
	if (flags & 0x200) {
		k = 2;
		sz = 127;
	} else {
		k = 1;
		sz = 63;
	}
	flags &= 15;
	for (j = 0; j < 4; j++) {
		xx = x;
		for (i = 0; i < 4; i++) {
			if (d_363cc[flags][j][rot][i])
				f_11680(gfx, y, xx, c, 100, sz, sz, 0);
			xx = xx + n * k;
		}
		y += n * k;
	}
}
