/* rom: 0x209a4 len: 0x180 func: f_209a4 flags: -macsave=1 -optimize=1 -speed */
/* WIP: f_209a4 and f_20a32 match; f_20a9c (sprite draw) is at 49%, with the y
 * expression's add order and the arg pushes still off. Scored with funcscore.py. */
struct frame { char pad[12]; };
struct sprite {
	struct frame *frames;      /* 0x18 */
	short w4;                  /* 0x1c */
	short y;                   /* 0x1e */
	short x;                   /* 0x20 */
};
struct task {
	char pad0[6];
	short tick;                /* 0x06 */
	short n;                   /* 0x08 */
	char pad1[16 - 10];
	void (*func)(struct task *); /* 0x10 */
	struct field *owner;       /* 0x14 */
	struct sprite spr;         /* 0x18 */
};
struct point { short x, y; };
struct field {
	char pad0[0xde];
	unsigned char height;      /* 0x0de */
	unsigned char width;       /* 0x0df */
	struct point pos;          /* 0x0e0 */
	char pad1[0x314 - 0xe4];
	struct point shake;        /* 0x314 */
};
extern struct task *f_17614(void);
extern void f_17638(struct task *);
extern void f_1159c(struct frame *, int, int, int, int);
extern unsigned short g_6060000;
extern struct frame d_a887c[], d_a8bdc[];

void f_20a32(struct task *t);

void f_209a4(struct field *f, short y)
{
	struct task *t;
	struct sprite *s;
	short x;

	if ((t = f_17614()) != 0) {
		t->func = f_20a32;
		t->owner = f;
		s = &t->spr;
		t->tick = 0;
		t->n = 0;
		x = f->shake.x + f->pos.x;
		s->x = 4;
		s->x = (s->x + 2) * 8 + x - (f->width / 2) * 8;
		s->y = (f->height - y - 1) * 8 + f->shake.y - (f->height - 1) * 8 - 6;
		s->frames = d_a887c;
		s->w4 = 0xb8;
	}
}

void f_20a32(struct task *t)
{
	struct sprite *s = &t->spr;
	int y, x;

	y = s->y;
	x = s->x;
	f_1159c(&s->frames[t->n], y, x, s->w4, 124);
	if (g_6060000 >= 40)
		return;
	if (t->tick & 1)
		t->n++;
	t->tick++;
	if (t->n >= 8)
		f_17638(t);
}

void f_20a9c(struct field *f, short x)
{
	struct point *d = &f->shake;
	short sx;
	int sy;

	sx = x * 8 + d->x - (f->width / 2) * 8;
	sy = (f->height - 20 - 1) * 8 + d->y - (f->height - 1) * 8 - 6;
	f_1159c(d_a8bdc, sy, sx, 0xba, 115);
}
