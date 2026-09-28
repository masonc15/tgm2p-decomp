/* rom: 0x176e0 len: 0x984 func: f_176e0 flags: -macsave=1 -optimize=1 -speed */
/* Playfield effect objects. Each spawner takes a task slot (f_17614, the
 * task pool's allocator), points it at its update routine and fills in a
 * sprite; each update draws the sprite, advances tick every frame while
 * g_6060000 < 40, and frees the slot (f_17638) after a fixed number of
 * frames. All ten functions also match as one file, byte for byte including
 * pools, as well as unit by unit; the update routine for the last two
 * spawners is at 0x18064, outside this range. */
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
	char pad1[0x30c - 0xe0];
	unsigned short mode;       /* 0x30c */
	char pad2[0x314 - 0x30e];
	struct point shake;        /* 0x314 */
	char pad3[0x35e - 0x318];
	unsigned short x35e;       /* 0x35e */
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

void f_177a8(struct task *t);

void f_176e0(struct field *f, short y, short x)
{
	struct task *t;
	struct sprite *s;
	short r;

	if ((t = f_17614()) != 0) {
		t->func = f_177a8;
		t->owner = f;
		s = &t->spr;
		t->tick = 0;
		s->x = x * 8 + f->shake.x - (f->width / 2) * 8;
		s->y = (f->height - y - 1) * 8 + f->shake.y - (f->height - 1) * 8 - 6;
		r = f_2beca(8);
		s->frames = d_3b3f4[r % 8];
		s->w4 = d_3b340[(f->cells[y * f->width + x].v & 15) - 2];
	}
}

void f_177a8(struct task *t)
{
	struct sprite *s = &t->spr;
	int y, x;

	y = s->y;
	x = s->x;
	f_1159c(&s->frames[t->tick], y, x, s->w4 + 9, 115);
	if (g_6060000 >= 40)
		return;
	t->tick++;
	if (t->tick >= 32)
		f_17638(t);
}

void f_178d0(struct task *t);

void f_17834(struct field *f, short y, short x)
{
	struct task *t;
	struct sprite *s;
	short r;

	if ((t = f_17614()) != 0) {
		t->func = f_178d0;
		t->owner = f;
		s = &t->spr;
		t->tick = 0;
		s->x = (x + 2) * 8 + f->shake.x - (f->width / 2) * 8;
		s->y = (f->height - y + 1) * 8 + f->shake.y - (f->height - 1) * 8 - 6;
		s->frames = d_aacd0;
		r = f_2beca(5);
		s->w4 = d_3b414[r];
	}
}

void f_178d0(struct task *t)
{
	struct sprite *s = &t->spr;
	short r, tick, y, x;

	r = f_2beca(5);
	s->w4 = d_3b414[r];
	tick = t->tick;
	y = s->y - (tick + 63) * 32 / 63;
	x = s->x - (tick + 63) * 32 / 63;
	f_11680(&s->frames[tick], y, x, s->w4, 115, tick + 63, tick + 63, 0);
	if (g_6060000 >= 40)
		return;
	t->tick++;
	if (t->tick >= 32)
		f_17638(t);
}

void f_17a54(struct task *t);

void f_179b8(struct field *f, short y, short x)
{
	struct task *t;
	struct sprite *s;
	short r;

	if ((t = f_17614()) != 0) {
		t->func = f_17a54;
		t->owner = f;
		s = &t->spr;
		t->tick = 0;
		s->x = (x + 2) * 8 + f->shake.x - (f->width / 2) * 8;
		s->y = (f->height - y + 1) * 8 + f->shake.y - (f->height - 1) * 8 - 6;
		s->frames = d_aacd0;
		r = f_2beca(5);
		s->w4 = d_3b414[r];
	}
}

void f_17a54(struct task *t)
{
	struct fx *e = (struct fx *)&t->owner;
	struct sprite *s = &e->spr;
	struct field *f = e->owner;
	short r;
	unsigned int w;

	r = f_2beca(5);
	s->w4 = d_3b414[r];
	if (t->tick < 32) {
		int y, x;
		y = s->y - 32;
		x = s->x - 32;
		f_1159c(&s->frames[t->tick], y, x, s->w4, 115);
	}
	if (g_6060008 & 3)
		w = 8;
	else
		w = 2;
	f_1159c(d_aaea4, 100, f->shake.x - 48, w, 115);
	if (g_6060000 >= 40)
		return;
	t->tick++;
	if (t->tick >= 60)
		f_17638(t);
}

void f_17c78(struct task *t);

void f_17b34(struct field *f, short y, short x)
{
	struct task *t;
	struct fx2 *p;
	short r, i;

	if ((t = f_17614()) != 0) {
		t->func = f_17c78;
		t->owner = f;
		p = (struct fx2 *)&t->spr;
		t->tick = 0;
		p->x[0] = (x + 2) * 8 + f->shake.x - (f->width / 2) * 8;
		p->y[0] = (f->height - y + 1) * 8 + f->shake.y - (f->height - 1) * 8 - 6;
		r = f_2beca(10) - 5;
		p->x[1] = p->x[0] + r;
		r = f_2beca(10) - 5;
		p->y[1] = p->y[0] + r;
		for (i = 0; i < 2; i++) {
			r = f_2beca(8);
			p->frames[i] = d_3b3f4[r % 8];
			p->w = d_3b340[(f->x35e & 15) - 2];
		}
	}
}

void f_17c78(struct task *t)
{
	struct fx2 *p = (struct fx2 *)&t->spr;
	short i;
	int y, x;

	for (i = 0; i < 2; i++) {
		y = p->y[i];
		x = p->x[i];
		f_1159c(&p->frames[i][t->tick], y, x, p->w + 9, 115);
	}
	if (g_6060000 >= 40)
		return;
	t->tick++;
	if (t->tick >= 32)
		f_17638(t);
}

void f_18064(struct task *t);

void f_17d48(struct field *f, short y)
{
	struct task *t;
	struct fx3 *s;
	short i;
	short r;
	int a, b;

	if ((t = f_17614()) == 0)
		return;
	t->func = f_18064;
	t->owner = f;
	s = (struct fx3 *)&t->spr;
	t->tick = 0;
	t->n = f->width;
	t->row = y;
	s->y = (f->height - t->row - 1) * 8 + f->shake.y - (f->height - 1) * 8 - 6;
	r = f_2beca(0x7cf);
	i = 1;
	a = y % 3;
	b = y % 2;
	for (; i < t->n - 1; i++, r += 3) {
		if (f->mode & 64) {
			if (a == i % 3) {
				s->frames[i - 1] = d_3b3f4[r % 8];
				s->w[i - 1] = d_3b340[(f->cells[y * f->width + i].v & 15) - 2];
			} else
				s->frames[i - 1] = 0;
		} else {
			if (b == i % 2) {
				s->frames[i - 1] = d_3b3f4[r % 8];
				s->w[i - 1] = d_3b340[(f->cells[y * f->width + i].v & 15) - 2];
			} else
				s->frames[i - 1] = 0;
		}
	}
}

void f_17f10(struct field *f, short y)
{
	struct task *t;
	struct fx3 *s;
	short i, r, k;

	if ((t = f_17614()) == 0)
		return;
	t->func = f_18064;
	t->owner = f;
	s = (struct fx3 *)&t->spr;
	t->tick = 0;
	t->n = f->width;
	t->row = y;
	s->y = (f->height - t->row - 1) * 8 + f->shake.y - (f->height - 1) * 8 - 6;
	r = f_2beca(0x7cf);
	k = 0;
	for (i = 1; i < t->n - 1; i++, r += 3) {
		if (f->cells[y * f->width + i].v != 0)
			k = (k + 1) % 2;
		if (k) {
			s->frames[i - 1] = d_3b3f4[r % 8];
			s->w[i - 1] = d_3b340[(f->cells[y * f->width + i].v & 15) - 2];
		} else
			s->frames[i - 1] = 0;
	}
}
