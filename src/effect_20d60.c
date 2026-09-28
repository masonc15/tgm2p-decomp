/* rom: 0x20d60 len: 0x218 func: f_20d60 flags: -macsave=1 -optimize=1 -speed */
struct cell {
	unsigned short flags;
	char c2;
	char c3;
	short b;
};
struct field {
	struct cell *cells;        /* 0x000 */
	char pad0[0xde - 4];
	unsigned char height;      /* 0x0de */
	unsigned char width;       /* 0x0df */
	char pad1[0x304 - 0xe0];
	short w304;                /* 0x304 */
	char pad2[0x308 - 0x306];
	unsigned long l308;        /* 0x308 */
	char pad3[0x30e - 0x30c];
	unsigned char b30e;        /* 0x30e */
	char pad4[0x379 - 0x30f];
	unsigned char b379;        /* 0x379 */
};
struct ent {
	char c;
	short delay;
	short n;
};
struct work {
	struct field *f;           /* 0x14 */
	struct ent e[21];          /* 0x18 */
};
struct task {
	char pad0;
	unsigned char state;       /* 0x01 */
	char pad1[4];
	short tick;                /* 0x06 */
	char pad2[16 - 8];
	void (*func)(struct task *); /* 0x10 */
	struct work w;             /* 0x14 */
};
extern void f_17638(struct task *);
extern unsigned short g_6060000;
extern unsigned char g_606475a[];
extern unsigned char g_6079374[];

void f_20d60(struct task *t)
{
	struct work *w = &t->w;
	struct field *f = w->f;
	struct ent *e;
	struct cell *c;
	short k, y, x;

	if (g_6060000 >= 40)
		return;
	switch (t->state) {
	case 0:
		if (g_606475a[f->b30e] & 1)
			t->tick = 3;
		for (k = 0; k < t->tick; k++) {
			for (y = 1; y < f->height; y++) {
				e = &w->e[y - 1];
				if (e->delay == 0) {
					if (e->c < 5) {
						if (e->n++ % 2 == 0)
							e->c++;
						c = &f->cells[y * f->width + 1];
						for (x = 1; x < f->width - 1; x++) {
							if ((c->flags & 15) >= 2)
								f->cells[y * f->width + x].c2 = e->c;
							c++;
						}
					} else if (e->c == 5) {
						e->c++;
						c = &f->cells[y * f->width + 1];
						for (x = 1; x < f->width - 1; x++) {
							if ((c->flags & 15) >= 2)
								c->flags = 0;
							c++;
						}
					}
				} else
					e->delay--;
			}
		}
		if (w->e[20].c >= 6) {
			t->state++;
			f->w304 = 1;
		}
		break;
	case 1:
		if ((f->l308 & 0x10000000) || (f->l308 & 0x2000)) {
			f->b379 = 0;
			g_6079374[f->b30e] = 0;
			f_17638(t);
		}
		break;
	}
}
