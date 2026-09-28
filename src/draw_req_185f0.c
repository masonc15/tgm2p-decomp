/* rom: 0x185f0 len: 0x118 func: f_185f0 flags: -macsave=1 -optimize=1 -speed */
/* Deferred draw calls. Each spawner takes a task slot and stores its
 * arguments; the task's update makes the draw call on the next pass and
 * frees the slot. This matches from a fresh file but not compiled after the
 * effect functions before it, so it probably starts a source file. The
 * prototype for f_1159c must take a short fifth argument here (unlike in
 * effect_176e0.c), or f_18630 loses a register copy. */
struct frame { char pad[12]; };
struct task {
	char pad0[6];
	short tick;                /* 0x06 */
	char pad1[0x10 - 8];
	void (*func)(struct task *); /* 0x10 */
	long data[64];             /* 0x14 */
};
struct req {                   /* the task's data, from 0x14 */
	struct frame *frame;       /* 0x14 */
	short a;                   /* 0x18 */
	short b;                   /* 0x1a */
	char c;                    /* 0x1c */
	short d;                   /* 0x1e */
	short e;                   /* 0x20 */
	short g;                   /* 0x22 */
	char h;                    /* 0x24 */
};
extern struct task *f_17614(void);
extern void f_17638(struct task *);
extern void f_1159c(struct frame *, short, short, short, short);
extern void f_11680(struct frame *, short, short, short, short, short, short, short);

void f_18630(struct task *t);

void f_185f0(struct frame *frame, short a, short b, char c, short d)
{
	struct task *t;
	struct req *r;

	if ((t = f_17614()) != 0) {
		t->func = f_18630;
		t->tick = 1;
		r = (struct req *)t->data;
		r->frame = frame;
		r->a = a;
		r->b = b;
		r->c = c;
		r->d = d;
	}
}

void f_18630(struct task *t)
{
	struct req *r = (struct req *)t->data;

	f_1159c(r->frame, r->a, r->b, r->c, r->d);
	f_17638(t);
}

void f_186ae(struct task *t);

void f_1865e(struct frame *frame, short a, short b, char c, short d, short e, short g, char h)
{
	struct task *t;
	struct req *r;

	if ((t = f_17614()) != 0) {
		t->func = f_186ae;
		t->tick = 1;
		r = (struct req *)t->data;
		r->frame = frame;
		r->a = a;
		r->b = b;
		r->c = c;
		r->d = d;
		r->e = e;
		r->g = g;
		r->h = h;
	}
}

void f_186ae(struct task *t)
{
	struct req *r = (struct req *)t->data;

	f_11680(r->frame, r->a, r->b, r->c, r->d, r->e, r->g, r->h);
	f_17638(t);
}
