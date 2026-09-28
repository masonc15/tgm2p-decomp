/* rom: 0x20250 len: 0x3a0 func: f_20250 flags: -macsave=1 -optimize=1 -speed */
struct frame { char pad[12]; };
struct gfx {
	struct frame *f0;          /* 0x00 */
	struct frame *f1;          /* 0x04 */
	unsigned char h0;          /* 0x08 */
	unsigned char h1;          /* 0x09 */
	char pad[2];
};
struct eff {
	char pad0[4];
	short x;                   /* 0x04 */
	short w6;                  /* 0x06 */
	short y;                   /* 0x08 */
	short w10;                 /* 0x0a */
	unsigned char kind;        /* 0x0c */
	char pad1;
	short a;                   /* 0x0e */
	short b;                   /* 0x10 */
};
struct task {
	char b0;
	unsigned char state;       /* 0x01 */
	char pad0[6 - 2];
	short timer;               /* 0x06 */
	char pad1[16 - 8];
	void (*func)(struct task *); /* 0x10 */
	struct eff e;              /* 0x14 */
};
extern unsigned char f_8b7c(void);
extern struct task *f_17614(void);
extern void f_17638(struct task *);
extern void f_2e6fc(int);
extern void f_11680(void *, short, short, short, short, short, short, short);
extern unsigned char g_606488a;
extern unsigned char g_6064888[];
extern unsigned long g_6064880;
extern struct gfx d_3b4d4[];

void f_202b0(struct task *t);

void f_20250(void)
{
	unsigned char n;
	struct task *t;
	struct eff *e;

	n = f_8b7c();
	if ((t = f_17614()) != 0) {
		t->func = f_202b0;
		e = &t->e;
		e->kind = g_606488a;
		if (e->kind > 4)
			e->kind = 5;
		if (n - 1 <= g_6064888[0])
			if (n - 1 <= g_6064888[1])
				e->kind = 5;
		g_6064880 |= 0x8000;
	}
}

void f_202b0(struct task *t)
{
	struct eff *e = &t->e;
	struct gfx *g = (struct gfx *)((char *)d_3b4d4 + (char)(e->kind * 12));
	short a, v;

	t->timer--;
	switch (t->state) {
	case 0:
		t->state++;
		t->timer = 60;
		e->y = 160;
		e->x = 90;
		e->a = e->b = e->w10 = e->w6 = 0;
	case 1:
		e->a += 4;
		if (e->a > 63)
			e->a = 63;
		f_11680(g->f0, e->x, e->y, 0, 110, 63, e->a, 0);
		f_11680(g->f1, e->x, e->y, 0, 110, 63, e->a, 0);
		if (t->timer == 0) {
			t->state++;
			t->timer = 70;
			f_2e6fc(29);
		}
		break;
	case 2:
		e->b += 4;
		if (e->b > 63)
			e->b = 63;
		f_11680(g->f0, e->x, e->y, 0, 110, 63, e->a, 0);
		f_11680(g->f1, e->x, e->y, 0, 110, 63, e->a, 0);
		/* graphics at ROM 0xa6eb4/0xa6ec0: written as d_a6eb4 the call loads
		   110 into a scratch register instead of reusing r10 */
		f_11680((struct frame *)0xa6eb4, e->x + 32, e->y, 0, 110, e->b, 63, 0);
		if (t->timer == 0) {
			t->state++;
			t->timer = 30;
			f_2e6fc(16);
		}
		break;
	case 3:
		f_11680(g->f0, e->x, e->y, 0, 110, 63, e->a, 0);
		f_11680(g->f1, e->x, e->y, 0, 110, 63, e->a, 0);
		f_11680((struct frame *)0xa6ec0, e->x + 32, e->y, 0, 110, e->b, 63, 0);
		if (t->timer == 0) {
			t->state++;
			t->timer = 16;
		}
		break;
	case 4:
		e->a -= 4;
		if (e->a < 0)
			e->a = 0;
		v = g->h0 * (64 - e->a) / 64;
		a = e->a;
		f_11680(g->f0, e->x, e->y + v, 0, 110, 63, a, 0);
		v = g->h1 * (64 - e->a) / 64;
		a = e->a;
		f_11680(g->f1, e->x, e->y + v, 0, 110, 63, a, 0);
		v = (64 - e->a) * 24 / 64;
		a = e->a;
		f_11680((struct frame *)0xa6ec0, e->x + v + 32, e->y, 0, 110, a, 63, 0);
		if (t->timer == 0) {
			/* compiles to nothing; without it SHC ranks 110 below the
			   f_11680 address and swaps r9/r10 */
			if (0)
				t->timer = 110;
			f_17638(t);
			g_6064880 &= 0xffff7fff;
		}
		break;
	}
}
