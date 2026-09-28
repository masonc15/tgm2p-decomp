/* rom: 0x21068 len: 0x108 func: f_21068 flags: -macsave=1 -optimize=1 -speed */
/* A playfield effect at a cell: sprite 0xbc placed on cell (x, y), animated
 * one frame per tick for 14 ticks. Matches from a fresh file. */
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
	char pad1[0x314 - 0xe0];
	struct point shake;        /* 0x314 */
};
extern struct task *f_17614(void);
extern void f_17638(struct task *);
extern void f_1159c(struct frame *, int, int, int, int);
extern unsigned short g_6060000;
extern struct frame d_a8ee8[];

void f_210ee(struct task *t);

void f_21068(struct field *f, short y, short x)
{
	struct task *t;
	struct sprite *s;

	if ((t = f_17614()) != 0) {
		t->func = f_210ee;
		t->owner = f;
		s = &t->spr;
		t->tick = 0;
		s->x = x * 8 + f->shake.x - (f->width / 2) * 8;
		s->y = (f->height - y - 1) * 8 + f->shake.y - (f->height - 1) * 8 - 6;
		s->frames = d_a8ee8;
		s->w4 = 0xbc;
	}
}

void f_210ee(struct task *t)
{
	struct sprite *s = &t->spr;
	int y, x;

	y = s->y;
	x = s->x;
	f_1159c(&s->frames[t->tick], y, x, s->w4, 115);
	if (g_6060000 >= 40)
		return;
	t->tick++;
	if (t->tick >= 14)
		f_17638(t);
}
