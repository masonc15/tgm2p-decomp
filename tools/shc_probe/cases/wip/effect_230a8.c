/* rom: 0x230a8 len: 0xf0 func: f_230a8 flags: -macsave=1 -optimize=1 -speed */
struct frame { char pad[12]; };
struct point { short x, y; };
struct sprite {
	struct frame *frames;      /* 0x18 */
	short w4;                  /* 0x1c */
	short y;                   /* 0x1e */
	short x;                   /* 0x20 */
};
struct task {
	char pad0[6];
	short tick;                /* 0x06 */
	char pad1[16 - 8];
	void (*func)(struct task *); /* 0x10 */
	struct field *owner;       /* 0x14 */
	struct sprite spr;         /* 0x18 */
};
struct field {
	char pad0[0xdf];
	unsigned char width;       /* 0x0df */
	struct point pos;          /* 0x0e0 */
	char pad1[0x314 - 0xe4];
	struct point shake;        /* 0x314 */
};
extern struct task *f_17614(struct field *);
extern void f_1159c(struct frame *, int, int, int, int);
extern void f_17638(struct task *);
extern unsigned short g_6060000;
extern struct frame d_a8f90[];

void f_23118(struct task *t);

void f_230a8(struct field *f)
{
	struct task *t;
	struct sprite *s;
	short x, y;
	struct point *d, *p;

	if ((t = f_17614(f)) != 0) {
		t->func = f_23118;
		t->owner = f;
		s = &t->spr;
		t->tick = 0;
		d = &f->shake;
		p = &f->pos;
		x = p->x + d->x;
		y = d->y + p->y;
		s->x = 4;
		s->x = (s->x + 2) * 8 + x - (f->width / 2) * 8;
		s->y = y - 96;
		s->frames = d_a8f90;
		s->w4 = 0xbd;
	}
}

void f_23118(struct task *t)
{
	struct sprite *s = &t->spr;
	int y, x;

	y = s->y;
	x = s->x;
	f_1159c(&s->frames[t->tick], y, x, s->w4, 124);
	if (g_6060000 >= 40)
		return;
	t->tick++;
	if (t->tick >= 64)
		f_17638(t);
}
