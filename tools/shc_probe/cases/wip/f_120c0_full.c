/* rom: 0x120c0 len: 0x1128 func: f_120c0 flags: -macsave=1 -optimize=1 -speed */
struct frame { char pad[12]; };
struct pair { long a, b; };
struct quad { short a, b, c, d; };
extern struct pair g_6079454[3];
extern short g_607946c[3];
extern struct pair g_6079474[3];
extern struct pair g_607948c[3];
extern struct quad g_60794a4[3];
extern struct pair d_3b064[3];
extern struct pair d_3b07c[3];
extern struct pair d_3b094[3];
extern struct quad d_3b0ac[3];
extern struct frame *d_3b13c[];
extern struct frame *d_3b0c4[];
extern void f_1159c(struct frame *, short, short, short, short);
extern void f_185f0(struct frame *, short, short, char, short);

void f_120c0(void)
{
	short i;

	for (i = 0; i < 3; i++) {
		g_6079454[i] = d_3b064[i];
		g_607946c[i] = 0;
	}
	for (i = 0; i < 3; i++)
		g_6079474[i] = d_3b07c[i];
	for (i = 0; i < 3; i++) {
		g_607948c[i] = d_3b094[i];
		g_60794a4[i] = d_3b0ac[i];
	}
}

void f_121fe(unsigned char *s, short x, short y, char c, short e)
{
	while (*s != 0) {
		f_1159c(d_3b13c[*s++ - 32], x, y, c, e);
		y += 16;
	}
}

void f_12252(unsigned long n, short x, short y)
{
	short i;
	long d;

	if (n > 999999)
		n = 999999;
	y += 80;
	for (i = 0; i < 6; i++) {
		d = n % 10;
		f_1159c(d_3b13c[d + 16], x, y, 0, 110);
		n /= 10;
		y -= 16;
		if (n == 0)
			break;
	}
}

void f_122cc(unsigned long n, short x, short y)
{
	short i;
	long d;

	if (n > 999999)
		n = 999999;
	y += 60;
	for (i = 0; i < 6; i++) {
		d = n % 10;
		f_1159c(d_3b0c4[d], x, y, 0, 110);
		n /= 10;
		y -= 12;
		if (n == 0)
			break;
	}
}

void f_12380(unsigned long t, short x, short y)
{
	unsigned char m, s, c;

	m = t / 3600;
	t -= m * 3600;
	s = t / 60;
	t -= s * 60;
	c = t * 100 / 60;
	f_1159c(d_3b0c4[m / 10], x, y, 0, 110);
	f_1159c(d_3b0c4[m % 10], x, y + 11, 0, 110);
	f_1159c((struct frame *)0xa7130, x, y + 22, 0, 110);
	f_1159c(d_3b0c4[s / 10], x, y + 27, 0, 110);
	f_1159c(d_3b0c4[s % 10], x, y + 38, 0, 110);
	f_1159c((struct frame *)0xa7130, x, y + 49, 0, 110);
	f_1159c(d_3b0c4[c / 10], x, y + 54, 0, 110);
	f_1159c(d_3b0c4[c % 10], x, y + 65, 0, 110);
}

void f_124a8(short x, short y)
{
	f_185f0((struct frame *)0xa7694, x, y + 31, 0, 110);
	f_185f0((struct frame *)0xa76a0, x, y + 85, 0, 110);
	f_185f0((struct frame *)0xa76ac, x, y + 126, 0, 110);
	f_185f0((struct frame *)0xa76d0, x, y + 215, 0, 110);
}

void f_12530(short x, short y)
{
	f_185f0((struct frame *)0xa7694, x, y + 42, 0, 110);
	f_185f0((struct frame *)0xa76dc, x, y + 214, 0, 110);
}

void f_12570(short x, short y)
{
	f_185f0((struct frame *)0xa76c4, x, y + 10, 0, 110);
	f_185f0((struct frame *)0xa7694, x, y + 97, 0, 110);
	f_185f0((struct frame *)0xa76a0, x, y + 166, 0, 110);
	f_185f0((struct frame *)0xa76ac, x, y + 229, 0, 110);
}

void f_125c8(short x, short y)
{
	f_185f0((struct frame *)0xa76b8, x, y + 35, 0, 110);
	f_185f0((struct frame *)0xa76ac, x, y + 230, 0, 110);
}

struct rec { char name[4]; unsigned long v; };
struct ent { char name[4]; unsigned short a, b; };
struct data {
	struct rec *r;             /* 0x14 */
	void *b;                   /* 0x18 */
	short x;                   /* 0x1c */
	short y;                   /* 0x1e */
	short wc;                  /* 0x20 */
	short rank;                /* 0x22 */
};
struct task {
	char pad0[6];
	short tick;                /* 0x06 */
	short mode;                /* 0x08 */
	short wait;                /* 0x0a */
	short delay;               /* 0x0c */
	short pad1;
	void (*func)(struct task *); /* 0x10 */
	struct data d;             /* 0x14 */
};
extern struct task *f_17614(void);
extern void f_11254(long, short, short, char, short, short, long, short);
extern void f_11680(struct frame *, short, short, short, short, short, short, short);
extern void f_216d0(short, short, short);
extern struct frame *d_3b0ec[];
extern short d_38f58[];
extern char g_6033644[];

struct state {
	char pad0[0x44];
	struct pair t44[19];
	struct quad tdc[3];
	short tf4[3];
};
extern struct state g_6065650;

void f_12608(struct task *t)
{
	struct data *d = &t->d;
	struct rec *r = d->r;
	struct ent *b = d->b;
	short x = d->x;
	short y = d->y;

	if (t->delay)
		t->delay--;
	else if (d->wc) {
		d->wc += 24;
		if (d->wc > 0)
			d->wc = 0;
	}
	if (t->wait) {
		t->wait--;
		return;
	}
	if (d->y) {
		short s = d->y / 4;
		if (s > 16)
			s = 16;
		d->y -= s;
	}
	if (0)
		d->rank = 40;
	if (0)
		d->rank = 40;
	if (0)
		d->rank = 0;
	if (0)
		b->a = 5;
	switch (t->mode) {
	case 0:
	if (0)
		r->v = r->v;
		f_1159c((d_3b13c + 16)[d->rank], x, y + 10, 0, 110);
		f_121fe((unsigned char *)r->name, x, y + 31, 0, 110);
		f_1159c(d_3b0ec[(r->v >> 27) & 31], x, y + 85, 0, 110);
		f_12380(r->v & 0xfffff, x + 2, y + 126);
		f_216d0(*(short *)b, x, y + 215);
		if (r->v & 0x2000000)
			f_11680((struct frame *)0xa79c4, x + 10, d->wc, 202, 40, 12, 63, 0);
		else if (r->v & 0x4000000)
			f_11680((struct frame *)0xa79c4, x + 10, d->wc, 0, 40, 12, 63, 0);
		break;
	case 1:
		f_1159c((d_3b13c + 16)[d->rank], x, y + 10, 0, 110);
		f_121fe((unsigned char *)r->name, x, y + 42, 0, 110);
		f_12252(r->v & 0xfffff, x, y + 214);
		if (r->v & 0x4000000)
			f_11680((struct frame *)0xa79c4, x + 10, d->wc, 0, 40, 12, 63, 0);
		break;
	case 2:
	if (0)
		d->x = x + 2;
	if (0)
		d->x = x + 2;
	if (0)
		d->x = x + 2;
	if (0)
		d->x = x + 2;
	if (0)
		d->y = y + 85;
	if (0)
		d->x = x + 10;
		if (d->rank == 0)
			f_11254(0, x + 2, y + 10, 0, 40, 3, 0, 2);
		else
			f_11254(d_38f58[d->rank - 1], x + 2, y + 10, 0, 40, 3, 0, 2);
		f_1159c((struct frame *)0xa76e8, x + 2, y + 34, 0, 110);
		f_11254(d_38f58[d->rank], x + 2, y + 42, 0, 40, 3, 0, 2);
		f_121fe((unsigned char *)r->name, x, y + 97, 0, 110);
		f_1159c(d_3b0ec[(r->v >> 27) & 31], x, y + 166, 0, 110);
		f_12380(r->v & 0xfffff, x + 2, y + 229);
		if (r->v & 0x2000000)
			f_11680((struct frame *)0xa79c4, x + 10, d->wc, 202, 40, 12, 63, 0);
		else if (r->v & 0x4000000)
			f_11680((struct frame *)0xa79c4, x + 10, d->wc, 0, 40, 12, 63, 0);
		break;
	case 3:
		f_1159c((d_3b13c + 16)[d->rank], x, y + 10, 0, 110);
		f_121fe((unsigned char *)r->name, x, y + 35, 0, 110);
		f_11254(b->a, x + 2, y + 85, 0, 40, 3, 0, 2);
		f_121fe((unsigned char *)g_6033644, x, y + 118, 0, 110);
		f_121fe((unsigned char *)b->name, x, y + 141, 0, 110);
		f_11254(b->b, x + 2, y + 191, 0, 40, 3, 0, 2);
		f_12380(r->v & 0xfffff, x + 2, y + 230);
		if (r->v & 0x4000000)
			f_11680((struct frame *)0xa79c4, x + 10, d->wc, 0, 40, 12, 63, 0);
		break;
	}
}

void f_12a58(void *r, void *b, short x, char c, short rank, short mode, short wait, short delay)
{
	struct task *t;
	struct data *d;

	if ((t = f_17614()) != 0) {
		t->func = f_12608;
		t->tick = 1;
		t->mode = mode;
		t->wait = wait;
		t->delay = delay;
		d = &t->d;
		d->r = r;
		d->b = b;
		d->x = x;
		d->y = 320;
		d->wc = -320;
		d->rank = rank;
	}
}

#define E g_6065650

void f_12ad4(signed char m)
{
	short i, j;

	switch (m) {
	case 0:
	if (0)
		E.tf4[0] = 3;
		for (j = 10, i = 0; i < 3; i++, j++)
			f_12a58(&E.t44[j], &E.tf4[i], i * 22 + 50, 0, i + 1, 0, i * 12, 90);
		{
		struct pair *p = g_6079454;
		short *q = g_607946c;
		short y = 170;
		for (i = 0; i < 3; i++, p++, q++, y += 22)
			f_12a58(p, q, y, 0, i + 1, 0, i * 12 + 36, 90);
		}
		break;
	case 1:
		for (j = 13, i = 0; i < 3; i++, j++)
			f_12a58(&E.t44[j], 0, i * 22 + 50, 0, i + 1, 1, i * 12, 90);
		for (i = 0; i < 3; i++)
			f_12a58(&g_6079474[i], 0, i * 22 + 170, 0, i + 1, 1, i * 12 + 36, 90);
		break;
	case 2:
		for (j = 0, i = 0; i < 10; i++, j++)
			f_12a58(&E.t44[j], 0, i * 19 + 54, 0, i, 2, i * 12, 138);
		break;
	case 3:
		for (j = 16, i = 0; i < 3; i++, j++)
			f_12a58(&E.t44[j], &E.tdc[i], i * 22 + 50, 0, i + 1, 3, i * 12, 90);
		for (i = 0; i < 3; i++)
			f_12a58(&g_607948c[i], &g_60794a4[i], i * 22 + 170, 0, i + 1, 3, i * 12 + 36, 90);
		break;
	}
}

extern void f_2164e();
extern long f_2b4e8(void);
extern void f_247ee(void);
extern void f_29cc6(long, long, long, long);
extern long f_149b4(void);
extern void f_175c4(void);
extern void f_124a8(short, short);
extern void f_12530(short, short);
extern void f_12570(short, short);
extern void f_125c8(short, short);
extern long g_6064750;
extern unsigned char g_606475e[2];
extern long g_6079538;
extern short g_6060022;
extern short g_dd;
extern long g_de;

long f_12fa8(signed char m)
{
	short t, done;

	f_2164e((done = 0, t = 0));

	f_12ad4(m);
	for (;;) {
		if (f_2b4e8() != 0) {
			f_247ee();
			return 11;
		}
		t++;
		if (g_6064750 == 0) {
			if (done) {
				switch (g_6079538) {
				case 7:
					return 8;
				case 8:
					return 9;
				case 9:
				case 10:
				default:
					return 3;
				}
			}
			if (t >= 360) {
	if (0)
		g_dd = 110;
				f_29cc6(2, 20, 0, 6);
				done = 1;
			}
		}
		if (g_6060022 == 0 && f_149b4() != 0) {
			f_247ee();
			return 12;
		}
		if (g_6064750 == 0) {
			if ((g_606475e[0] & 16) || (g_606475e[1] & 16)) {
				m = (m + 3) % 4;
				f_175c4();
				f_12ad4(m);
				f_29cc6(0, 20, 0, 6);
				t = 0;
			} else if ((g_606475e[0] & 32) || (g_606475e[1] & 32)) {
				m = (m + 1) % 4;
				f_175c4();
				f_12ad4(m);
				f_29cc6(0, 20, 0, 6);
				t = 0;
			}
		}
	if (0)
		g_de = t;
		switch (m) {
		case 0:
			f_185f0((struct frame *)0xa761c, 14, 10, 0, 110);
			f_124a8(35, 0);
			f_185f0((struct frame *)0xa767c, 134, 10, 0, 110);
			f_124a8(155, 0);
			break;
		case 1:
			f_185f0((struct frame *)0xa7634, 14, 10, 0, 110);
			f_12530(35, 0);
			f_185f0((struct frame *)0xa767c, 134, 10, 0, 110);
			f_12530(155, 0);
			break;
		case 2:
			f_185f0((struct frame *)0xa764c, 14, 10, 0, 110);
			f_12570(37, 0);
			break;
		case 3:
			f_185f0((struct frame *)0xa7664, 14, 10, 0, 110);
			f_125c8(35, 0);
			f_185f0((struct frame *)0xa767c, 134, 10, 0, 110);
			f_125c8(155, 0);
			break;
		}
	}
}
