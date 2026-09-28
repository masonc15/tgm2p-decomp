/* rom: 0x23f08 len: 0x9a0 func: f_23f08 flags: -macsave=1 -optimize=1 -speed */
/* NOT FINAL: probe.py 1228/1232 (99.7%) for 0x23f08-0x248a8 (includes the two
 * empty handlers now in src/stubs_23f08.c: they must be defined in this file,
 * before their uses, or SHC hoists their addresses out of loops). Every
 * function is byte-exact except f_2452e (82/86): the table load goes into
 * p's register and is copied to t, where the ROM loads into t (r7) and copies
 * to p (r6). ~150 variants tried (see v/s2452e*.py). */
typedef void (*func)();
struct task {
	struct task *parent;       /* 0x00 */
	struct task *child;        /* 0x04 */
	struct task *prev;         /* 0x08, sibling */
	struct task *next;         /* 0x0c, sibling */
	struct task *lprev;        /* 0x10, run list */
	struct task *lnext;        /* 0x14 */
	unsigned short n;          /* 0x18, handlers on the stack */
	short id;                  /* 0x1a */
	char slot[4];              /* 0x1c */
	char pad20[0x100 - 0x20];
	char work[4][128];         /* 0x100 */
	char pad300[0x3ec - 0x300];
	long top;                  /* 0x3ec, func of the top handler entry */
	char pad3f0[4];
	long f3f4;                 /* 0x3f4 */
	func main;                 /* 0x3f8 */
	func exit;                 /* 0x3fc */
};
struct desc {
	func top;
	func main;
	func init;
	func exit;
};
extern short g_60ad21c, g_6060000, g_6060068, g_60ad21e, g_60ad220;
extern struct task g_607d218[];
extern struct task *g_607cf10[];
extern struct task *g_607d210, *g_607d214, *g_60ad218, *g_606005c;
extern long *g_6060060, *g_6060064;

#define TOP(t) ((long *)((char *)(t) + 0x3ec))

void f_23f08()
{
}

void f_23f0c()
{
}

void f_23f10(void)
{
	long *p;
	unsigned int i;

	g_60ad21c = 0;
	p = (long *)g_607d218;
	for (i = 0; i < 0xc000; i++)
		*p++ = 0;
	for (i = 0; i < 0xc0; i++)
		g_607cf10[i] = &g_607d218[i];
	g_6060068 = 0;
	g_607d210 = g_607d214 = 0;
}

void f_23f5e(void)
{
	long *p;
	short n;

	g_6060000 = g_60ad21c;
	if (g_6060068) {
		g_60ad218 = g_607d210;
		g_606005c = g_60ad218;
		do {
			g_60ad21e = 0;
			if ((unsigned short)g_6060000 >= 40) {
				if (*(func *)((char *)g_60ad218 + 0x3f8) != f_23f08) {
					p = TOP(g_606005c) - (g_606005c->n - 1) * 4;
					n = g_606005c->n;
					while (n > 1) {
						g_6060060 = p;
						g_6060064 = p - 3;
						(*(func *)p)();
						if (g_60ad21e)
							break;
						n--;
						p += 4;
					}
					if (g_60ad21e == 0) {
						g_6060060 = (long *)((char *)g_60ad218 + 0x3f8);
						(**(func *)((char *)g_60ad218 + 0x3f8))();
					}
				}
			} else {
				p = TOP(g_606005c) - (g_606005c->n - 1) * 4;
				n = g_606005c->n;
				while (n > 0) {
					g_6060060 = p;
					g_6060064 = p - 3;
					(*(func *)p)();
					if (g_60ad21e)
						break;
					n--;
					p += 4;
				}
			}
			g_60ad218 = g_60ad218->lnext;
			g_606005c = g_60ad218;
		} while (g_60ad218);
	}
}

void f_240ae(long key, func fn)
{
	struct task *saved;
	long *p;
	unsigned short n;

	saved = g_606005c;
	if (g_6060068) {
		g_606005c = g_607d210;
		do {
			p = TOP(g_606005c) - (g_606005c->n - 1) * 4;
			n = g_606005c->n;
			while (n > 0) {
				if (*p == key) {
					(*fn)(p);
					break;
				}
				n--;
				p += 4;
			}
			g_606005c = g_606005c->lnext;
		} while (g_606005c);
	}
	g_606005c = saved;
}

void f_24138(long key, long val)
{
	struct task *saved;
	register long *p;
	unsigned short n;

	saved = g_606005c;
	if (g_6060068) {
		g_606005c = g_607d210;
		do {
			p = TOP(g_606005c) - (g_606005c->n - 1) * 4;
			n = g_606005c->n;
			while (n > 0) {
				if (*p == key)
					*p = val;
				p += 4;
				n--;
			}
		} while (g_606005c);
	}
	g_606005c = saved;
}

int f_2418e(struct task *t, long fn, long a, long b, long c)
{
	long *p;
	unsigned short i;

	if (t->n) {
		p = TOP(t);
		i = 0;
		while (i < t->n && *p != (long)f_23f0c) {
			if (i >= 15)
				return 0;
			i++;
			p -= 4;
		}
		if (i >= t->n)
			t->n++;
	} else {
		p = TOP(t);
		t->n++;
	}
	*p-- = fn;
	*p-- = c;
	*p-- = b;
	*p = a;
	return 1;
}

long f_24218(struct task *t, long fn, long a, long b)
{
	long *p;
	register long *q;
	unsigned short i;
	unsigned short k;
	long mem;

	if (t->n) {
		p = TOP(t);
		i = 0;
		while (i < t->n && *p != (long)f_23f0c) {
			i++;
			p -= 4;
		}
		if (i >= t->n)
			t->n++;
	} else {
		p = TOP(t);
		i = 0;
		t->n++;
	}
	i++;
	*p-- = fn;
	for (k = 0; k < 4; k++) {
		if (t->slot[k] == 0) {
			t->slot[k] = i;
			mem = 0x100;
			mem += (long)((char *)t + k * 128);
			mem &= 0x0fffffff;
			q = (long *)mem;
			mem |= (long)k << 28;
			*p-- = mem;
			break;
		}
	}
	if (k >= 4)
		return -1;
	for (i = 0; i < 32; i++)
		*q++ = 0;
	*p-- = b;
	*p = a;
	return mem;
}

void f_242f6(struct task *t)
{
	long *p;
	unsigned short i;

	p = TOP(t);
	for (i = 0; i < t->n; i++, p -= 4)
		*p = (long)f_23f0c;
	t->n = 0;
	for (i = 0; i < 4; i++)
		t->slot[i] = 0;
}

void f_24344(struct task *t, long fn)
{
	long *p;
	register unsigned short i, k;

	p = TOP(t);
	for (i = 0; i < t->n; i++) {
		if (*p == fn) {
			if (i + 1 == t->n)
				t->n--;
			*p = (long)f_23f0c;
			p--;
			i++;
			if ((unsigned long)t >= (unsigned long)p)
				return;
			for (k = 0; k < 4; k++) {
				if (t->slot[k] == i + 1) {
					t->slot[k] = 0;
					k = 4;
				}
			}
			break;
		}
		p -= 4;
	}
}

void f_243be(struct task *t, long *p)
{
	unsigned short i, k;
	unsigned long d;

	d = (char *)TOP(t) - (char *)p;
	i = d >> 4;
	if (i + 1 == t->n)
		t->n--;
	*p = (long)f_23f0c;
	p--;
	if ((unsigned long)t >= (unsigned long)p)
		return;
	for (k = 0; k < 4; k++) {
		if (t->slot[k] == i + 1) {
			t->slot[k] = 0;
			break;
		}
	}
}

long *f_2440a(struct task *t, long fn)
{
	long *p;
	short i;

	p = TOP(t);
	for (i = 0; i < t->n; i++) {
		if (*p == fn)
			return p;
		p -= 4;
	}
	return 0;
}

short f_2443c(long fn, unsigned short flags, short a, short b, short c, short d, short e, short f)
{
	long *p;
	short *w;
	short i;

	p = TOP(g_606005c);
	for (i = 0; i < g_606005c->n; i++) {
		w = (short *)(p - 3);
		if (*p == fn
		    && (!(flags & 1) || w[0] == a)
		    && (!(flags & 2) || w[1] == b)
		    && (!(flags & 4) || w[2] == c)
		    && (!(flags & 8) || w[3] == d)
		    && (!(flags & 16) || w[4] == e)
		    && (!(flags & 32) || w[5] == f)) {
			f_243be(g_606005c, p);
			return i;
		}
		p -= 4;
	}
	return -1;
}

struct task *f_2452e(struct task *parent)
{
	struct task *t;
	long *p;
	unsigned int i;
	short n;

	n = g_6060068;
	if (n >= 0xbf)
		return 0;
	p = (long *)g_607cf10[n];
	t = (struct task *)p;
	for (i = 0; i < 0x100; i += 4) {
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
	}
	if (parent) {
		t->parent = parent;
		t->next = parent->child;
		if (parent->child)
			parent->child->prev = t;
		parent->child = t;
	}
	if (g_6060068 == 0) {
		t->lnext = 0;
		t->lprev = 0;
		g_607d214 = t;
		g_607d210 = t;
	} else {
		t->lprev = g_607d214;
		t->lnext = 0;
		g_607d214->lnext = t;
		g_607d214 = t;
	}
	if (!g_60ad220)
		g_60ad220 = 1;
	t->id = g_60ad220++;
	g_6060068++;
	return t;
}

void f_245da(struct task *self)
{
	struct task *t, *parent, *child, *prev, *next, *q;

	t = self;
	parent = t->parent;
	child = t->child;
	prev = t->prev;
	next = t->next;
	if (child) {
		if (prev)
			prev->next = child;
		else if (parent)
			parent->child = child;
		child->prev = prev;
		for (q = child; ; q = q->next) {
			q->parent = parent;
			if (q->next == 0)
				break;
		}
		if (next) {
			q->next = next;
			next->prev = q;
		}
	} else {
		if (prev)
			prev->next = next;
		else if (parent)
			parent->child = next;
		if (next)
			next->prev = prev;
	}
	if (t == g_607d210)
		g_607d210 = t->lnext;
	if (t == g_607d214)
		g_607d214 = t->lprev;
	self->id = 0;
	if (t->lprev)
		t->lprev->lnext = t->lnext;
	if (t->lnext)
		t->lnext->lprev = t->lprev;
	g_607cf10[--g_6060068] = t;
}

struct task *f_2467e(struct desc *d, struct task *parent)
{
	struct task *saved, *t;

	saved = g_606005c;
	if (parent == (struct task *)-1)
		parent = saved;
	t = f_2452e(parent);
	if (t == 0)
		return 0;
	g_606005c = t;
	if (d->exit)
		*(func *)((char *)t + 0x3fc) = d->exit;
	else
		*(func *)((char *)t + 0x3fc) = f_23f08;
	if (d->main)
		*(func *)((char *)t + 0x3f8) = d->main;
	else
		*(func *)((char *)t + 0x3f8) = f_23f08;
	if (d->top)
		*(func *)((char *)t + 0x3ec) = d->top;
	else
		*(func *)((char *)t + 0x3ec) = f_23f08;
	t->n = 1;
	if (d->init)
		(*d->init)();
	g_606005c = saved;
	return t;
}

struct task *f_24724(func *d, struct task *parent)
{
	struct task *saved, *t;
	func *p;
	short i;

	saved = g_606005c;
	if (parent == (struct task *)-1)
		parent = saved;
	t = f_2452e(parent);
	if (t == 0)
		return 0;
	g_606005c = t;
	p = &t->exit;
	for (i = 1; i < 16 && d[i]; i++) {
		if (i < 4) {
			*p = d[i];
			p--;
		} else {
			if (i == 4)
				p--;
			*p = d[i];
			p -= 4;
		}
	}
	t->n = i - 4;
	(*d[0])();
	g_606005c = saved;
	return t;
}

void f_247b0(struct task *t)
{
	struct task *saved;
	func *f;

	f = &t->exit;
	saved = g_606005c;
	g_606005c = t;
	(**f)();
	f_245da(t);
	g_606005c = saved;
	if (t == g_606005c)
		g_60ad21e = 1;
	(void)&saved;
	(void)&f;
}

void f_247ee(void)
{
	struct task *t, *next;

	if (g_6060068) {
		for (t = g_607d210; t; t = next) {
			next = t->lnext;
			f_247b0(t);
		}
	}
}

void f_24828(long fn)
{
	struct task *t, *next;
	long *p;
	unsigned short i;

	if (g_6060068) {
		for (t = g_607d210; t; t = next) {
			next = t->lnext;
			p = TOP(t);
			for (i = 0; i < t->n; i++) {
				if (*p == fn) {
					f_247b0(t);
					break;
				}
				p -= 4;
			}
		}
	}
}

void f_24886(short v)
{
	g_60ad21c = v;
}

void f_2488c(void)
{
	g_60ad21c = 0;
}

short f_24894(void)
{
	return g_60ad21c;
}
