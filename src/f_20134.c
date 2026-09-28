/* rom: 0x20134 len: 0x11c func: f_20134 flags: -macsave=1 -optimize=1 -speed */
struct frame { char pad[12]; };
struct field {
	char pad0[0x308];
	unsigned long flags;	/* 0x308 */
	char pad1[2];
	unsigned char b30e;	/* 0x30e */
	char pad2[5];
	short shake_x;		/* 0x314 */
};
struct eff { struct field *owner; };
struct task {
	char pad0[6];
	short w[2];		/* 0x06: timer, counter */
	char pad1[16 - 10];
	void (*func)(struct task *);	/* 0x10 */
	struct eff e;		/* 0x14 */
};
extern struct task *f_17614(void);
extern void f_17638(struct task *);
extern void f_1159c(struct frame *, int, int, int, int);
extern long f_2beca(int);
extern void f_17834(struct field *, int, int, int);
extern void f_331a(struct field *);
extern unsigned short g_6060000;
extern unsigned char g_606475e[];
extern struct frame d_a80cc[];

void f_20160(struct task *t);

void f_20134(struct field *f)
{
	struct task *t;
	struct eff *e;

	if ((t = f_17614()) != 0) {
		e = &t->e;
		t->func = f_20160;
		t->w[0] = 600;
		t->w[1] = 1;
		e->owner = f;
	}
}

void f_20160(struct task *t)
{
	struct eff *e = &t->e;
	struct field *f = e->owner;

	f_1159c(d_a80cc, 120, f->shake_x, 0, 126);
	if (g_606475e[f->b30e] & 0x30)
		t->w[1]++;
	if (g_6060000 >= 40)
		return;
	if (t->w[0] % 12 == 0 || t->w[1] % 10 == 0) {
		t->w[1] = 1;
		f_17834(f, f_2beca(5) + 13, f_2beca(10), f_2beca(10));
	}
	if (--t->w[0] == 0) {
		f->flags &= 0x7fffffff;
		f_331a(f);
		f_17638(t);
	}
}
