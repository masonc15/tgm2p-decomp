/* rom: 0x23f08 len: 0x534 func: f_23f08 flags: -macsave=1 -optimize=1 -speed */
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

