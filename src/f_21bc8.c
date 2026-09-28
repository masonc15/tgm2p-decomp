/* rom: 0x21bc8 len: 0x1cc func: f_21bc8 flags: -macsave=1 -optimize=1 -speed */
/* A task that draws sprite 0x9d (graphics d_aaa30) for 180 frames: it drops
 * in from the top, decelerating, then falls back down to y = 63 and stays.
 * The spawner starts it at state 2, so the slide-in from the left in states
 * 0 and 1 isn't used from here. When the timer runs out it clears bit 16 of
 * g_6064880 and frees itself. Matches from a fresh file. */
struct fall {
	long y;                    /* 0x00, 16.16 */
	long vel;                  /* 0x04 */
	long acc;                  /* 0x08 */
	short x;                   /* 0x0c */
};
struct task {
	char b0;
	unsigned char state;       /* 0x01 */
	char pad0[6 - 2];
	short timer;               /* 0x06 */
	short count;               /* 0x08 */
	char pad1[16 - 10];
	void (*func)(struct task *); /* 0x10 */
	struct fall f;             /* 0x14 */
};
extern struct task *f_17614();
extern void f_17638(struct task *);
extern void f_2e6fc(int);
extern void f_2af5e(int, int, void *);
extern void f_11680(void *, int, short, short, short, short, short, short);
extern unsigned short g_6060000;
extern unsigned long g_6064880;
extern char d_68410[], d_aaa30[];

void f_21bfa(struct task *t);

/* The task allocator ignores its argument here, which is why t can reuse r4. */
void f_21bc8(void)
{
	struct task *t;

	if ((t = f_17614()) != 0) {
		t->func = f_21bfa;
		t->state = 2;
		t->timer = 180;
		f_2e6fc(40);
		f_2af5e(0x9d, 1, d_68410);
	}
}

void f_21bfa(struct task *t)
{
	struct fall *o = &t->f;
	short y;

	if (g_6060000 >= 40)
		return;
	switch (t->state) {
	case 0:
		t->state++;
		t->count = 8;
		o->y = 0x40000;
		o->x = -320;
	case 1:
		o->x += 40;
		if (o->x > 0)
			o->x = 0;
		if (--t->count == 0)
			t->state++;
		break;
	case 2:
		t->state++;
		o->x = 0;
		o->y = 0x40000;
		o->vel = 0x180000;
		o->acc = 0x18000;
	case 3:
		o->y += o->vel;
		o->vel -= o->acc;
		if (o->vel < 0 && o->y <= 0x3f0000) {
			t->state++;
			o->vel = -0x20000;
			o->acc = -0x2000;
		}
		break;
	case 4:
		o->y += o->vel;
		o->vel -= o->acc;
		if (o->y >= 0x3f0000) {
			t->state++;
			o->y = 0x3f0000;
		}
		break;
	}
	if (--t->timer != 0) {
		/* y is assigned inside an argument, which SHC evaluates before the
		 * others (right to left); that's why the ROM spills and reloads it. */
		f_11680(d_aaa30, (64 - y) * 20 / 128 + 120, o->x + 0xaa, 0x9d, 125, y = *(short *)&o->y, 63, 0);
		return;
	}
	g_6064880 &= ~0x10000;
	f_17638(t);
}
