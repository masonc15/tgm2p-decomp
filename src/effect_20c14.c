/* rom: 0x20c14 len: 0x14c func: f_20c14 flags: -macsave=1 -optimize=1 -speed */
struct cell {
	unsigned short flags;
	unsigned char c2;
	char c3;
	short b;
};
struct field {
	struct cell *cells;        /* 0x000 */
	char pad0[0xde - 4];
	unsigned char height;      /* 0x0de */
	unsigned char width;       /* 0x0df */
	char pad1[0x2f4 - 0xe0];
	struct field *other;       /* 0x2f4 */
	char pad2[0x30c - 0x2f8];
	unsigned short mode;       /* 0x30c */
	char pad3[0x379 - 0x30e];
	unsigned char b379;        /* 0x379 */
};
struct ent {
	char c;
	short x;
	short y;
};
struct work {
	struct field *f;           /* 0x14 */
	struct ent e[21];          /* 0x18 */
};
struct task {
	char pad0[6];
	short tick;                /* 0x06 */
	char pad1[16 - 8];
	void (*func)(struct task *); /* 0x10 */
	struct work w;             /* 0x14 */
};
extern struct task *f_17614(void);
extern unsigned long g_6064880;
extern void f_20d60(struct task *);

void f_20c14(struct field *f)
{
	struct task *t;
	struct work *w;
	short i, x0;
	short y, x;
	struct ent *e;
	struct cell *c;

	if ((t = f_17614()) == 0)
		return;
	f->b379 = 0xff;
	if (g_6064880 & 4)
		f->other->b379 = 0xff;
	t->func = f_20d60;
	t->tick = 1;
	if (f->mode & 0x10)
		x0 = 0xf0;
	else
		x0 = 0;
	w = &t->w;
	w->f = f;
	for (i = 0; i < 21; i++) {
		e = &w->e[i];
		e->x = i * 8 + x0;
		e->y = 0;
		e->c = -5;
	}
	for (y = 1; y < f->height; y++) {
		c = &f->cells[y * f->width + 1];
		for (x = 1; x < f->width - 1; x++) {
			c->c2 = 0;
			c++;
		}
	}
}
