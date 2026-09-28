/* rom: 0x21d94 len: 0x888 func: f_21d94 flags: -macsave=1 -optimize=1 -speed */
/* NOT FINAL: f_21d94 (one 0x850-byte function) plus f_225e4. Builds the
 * 38-entry per-player list at g_607cddc from five shuffled groups and spawns
 * task f_2261c. Register-normalized ~58%. Local aggregate initializers are
 * written as struct copies from extern g_60355b8.. because SHC rejects
 * non-constant auto initializers; the copies compile to the same mov
 * sequences. The case loops in the ROM are unrolled (by 2, 2, 2, 3) and keep
 * their loop bounds in callee-saved registers; this version does not. */
struct point { short x, y; };
struct field {
	char pad0[0xde];
	unsigned char height;      /* 0x0de */
	unsigned char width;       /* 0x0df */
	char pad1[0x30c - 0xe0];
	unsigned short mode;       /* 0x30c */
	unsigned char id;          /* 0x30e */
	char pad2[0x314 - 0x30f];
	struct point shake;        /* 0x314 */
};
struct roll {
	long b0;                   /* 0x00 */
	char pad0[0x10 - 4];
	short xs[38];              /* 0x10 */
	short ys[38];              /* 0x5c */
	short y0;                  /* 0xa8 */
	short y21;                 /* 0xaa */
	short y13;                 /* 0xac */
	char pad1[0xb0 - 0xae];
	long l;                    /* 0xb0 */
};
struct task {
	char pad0[6];
	short w6;                  /* 0x06 */
	short w8;                  /* 0x08 */
	short wa;                  /* 0x0a */
	short wc;                  /* 0x0c */
	char pad1[0x10 - 0xe];
	void (*func)(struct task *); /* 0x10 */
	struct field *owner;       /* 0x14 */
	struct roll r;             /* 0x18 */
};
struct l13 { long v[13]; };
struct l2 { long v[2]; };
struct l4 { long v[4]; };
struct l3 { long v[3]; };
struct l14 { long v[14]; };
struct b5 { unsigned char v[5]; };
struct s13 { short v[13]; };
struct s76 { long v[19]; };
extern struct task *f_17614(void);
extern void f_2e6fc(int);
extern int f_2afca(int);
extern void f_2af5e(int, int, void *);
extern void f_2261c(struct task *);
extern unsigned long g_6064880;
extern short g_607cdd8[];
extern long g_607cddc[];
extern const struct l13 g_60355b8;
extern const struct l2 g_60355ec, g_60355f4;
extern const struct l4 g_60355fc;
extern const struct l3 g_603560c;
extern const struct l14 g_6035618;
extern const struct b5 g_6035650;
extern const struct s13 g_6035656;
extern const struct s76 g_6035670;
extern char d_68490[], d_68550[];

#define LIST(f) ((long *)((char *)g_607cddc + (short)((f)->id * 0x98)))
#define ROWY(f, r) (((f)->height - (r) - 1) * 8 + (f)->shake.y - ((f)->height - 1) * 8 - 6)

void f_21d94(struct field *f)
{
	struct l13 a;
	struct l2 b;
	struct l2 c;
	struct l4 d;
	struct l3 e;
	struct l14 last;
	short pos[38];
	struct b5 order;
	struct s13 gap;
	struct task *t;
	struct roll *r;
	register short i, n, y;
	short j, k, p, q;
	unsigned char tmp;

	a = g_60355b8;
	b = g_60355ec;
	c = g_60355f4;
	d = g_60355fc;
	e = g_603560c;
	last = g_6035618;
	order = g_6035650;
	gap = g_6035656;
	if ((f->mode & 4) || ((f->mode & 0x1081) && (g_6064880 & 1)))
		f_2e6fc(15);
	for (k = 0; k < 10; k++) {
		p = f_2afca(64) % 5;
		q = f_2afca(64) % 5;
		if (p != q) {
			tmp = order.v[p];
			order.v[p] = order.v[q];
			order.v[q] = tmp;
		}
	}
	i = 1;
	y = 6;
	pos[0] = 1;
	LIST(f)[0] = last.v[0];
	for (j = 0; j < 5; j++) {
		switch (order.v[j]) {
		case 1:
			for (n = 0; n < 13; n++) {
				pos[i] = y;
				y += 2;
				LIST(f)[i] = a.v[n];
				i++;
			}
			break;
		case 2:
			for (n = 0; n < 2; n++) {
				pos[i] = y;
				y += 2;
				LIST(f)[i] = b.v[n];
				i++;
			}
			break;
		case 3:
			for (n = 0; n < 2; n++) {
				pos[i] = y;
				y += 2;
				LIST(f)[i] = c.v[n];
				i++;
			}
			break;
		case 4:
			for (n = 0; n < 4; n++) {
				pos[i] = y;
				y += 2;
				LIST(f)[i] = d.v[n];
				i++;
			}
			break;
		case 5:
			for (n = 0; n < 3; n++) {
				pos[i] = y;
				y += 2;
				LIST(f)[i] = e.v[n];
				i++;
			}
			break;
		default:
			continue;
		}
		y += 3;
	}
	for (n = 0; n < 13; n++) {
		pos[i] = gap.v[n] + y;
		LIST(f)[i] = last.v[n + 1];
		i++;
	}
	if ((t = f_17614()) != 0) {
		r = &t->r;
		t->func = f_2261c;
		t->owner = f;
		g_607cdd8[f->id] = 15;
		t->w6 = 0;
		t->w8 = 0;
		t->wa = 240;
		t->wc = 1;
		r->l = 0;
		r->y0 = ROWY(f, 0);
		r->y21 = ROWY(f, 21);
		r->y13 = ROWY(f, 13);
		for (k = 0; k < 38; k++) {
			if (f->mode & 4)
				r->xs[k] = f->shake.x - f->width / 2 * 8 + 24;
			else
				r->xs[k] = f->shake.x - f->width / 2 * 8 + 8;
			r->ys[k] = ROWY(f, -pos[k]);
		}
		f_2af5e(0xb2, 1, d_68490);
		f_2af5e(0xb3, 1, d_68550);
	}
}

void f_225e4(void)
{
	struct s76 x;

	x = g_6035670;
}
