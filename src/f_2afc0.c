/* rom: 0x2afc0 len: 0x470 func: f_2afc0 flags: -macsave=1 -optimize=1 -speed */
/* Palette helpers. Palette RAM (0x04040000, used here through the
 * cache-through mirror 0x24040000) is 16 longs per palette. The copy
 * routines run as deferred tasks: their queue functions store the callback
 * and its three arguments in g_6064350[g_606434c++], and the task runner
 * (f_29f24) calls func(a, b, c), so the callbacks take three longs. The
 * callbacks only match with their loop counters declared register. */
extern long g_24040000[][16];
typedef struct { long c[16]; } PALETTE;
#define PALS ((PALETTE *)0x24040000)
typedef struct { short c[32]; } SPAL;
#define SPALS ((SPAL *)0x24040000)

struct task {
	void (*func)();
	long a;
	long b;
	long c;
};
struct fade {
	long *list;                /* 0 */
	long *dst;                 /* 4 */
	short reload;              /* 8 */
	short timer;               /* 0xa */
	unsigned short idx;        /* 0xc */
	short pad;
};
extern struct task g_6064350[];
extern unsigned short g_606434c;
extern struct fade g_6064550[];
extern void f_29c40(int, int, int);
extern void f_29c76(int, int, int);

void f_2afc0(long a, long b, long c)
{
	unsigned char pal = a;
	unsigned char n = b;
	PALETTE *src = (PALETTE *)c;
	register short i;
	register short j;

	for (i = 0; i < n; i++)
		for (j = 0; j < 16; j++)
			(PALS + pal)[i].c[j] = src[i].c[j];
}

void f_2b076(long a, long b, long c);

void f_2b034(unsigned char pal, unsigned char n, long *src)
{
	g_6064350[g_606434c].func = f_2b076;
	g_6064350[g_606434c].a = pal;
	g_6064350[g_606434c].b = n;
	g_6064350[g_606434c++].c = (long)src;
}

void f_2b076(long a, long b, long c)
{
	unsigned char pal = a;
	unsigned char n = b;
	PALETTE *src = (PALETTE *)c;
	register short i;
	register short j;

	for (i = 0; i < n; i++)
		for (j = 0; j < 16; j++)
			(PALS + pal)[i].c[j] = (src - i)->c[j];
}

void f_2b11e(long a, long b);

void f_2b0ea(unsigned char pal, short **list)
{
	g_6064350[g_606434c].func = f_2b11e;
	g_6064350[g_606434c].a = pal;
	g_6064350[g_606434c++].b = (long)list;
}

void f_2b11e(long a, long b)
{
	register short k;
	register short j;
	register short more = 1;
	unsigned char pal = a;
	short **list = (short **)b;

	for (k = 0; more; k++, pal++) {
		switch ((long)list[k]) {
		default:
			for (j = 0; j < 32; j++)
				SPALS[pal].c[j] = list[k][j];
			break;
		case -1:
			continue;
		case 0:
			more = 0;
			break;
		}
	}
}

void f_2b196(void)
{
	f_29c40(7, 0, 0);
	f_29c76(0, 0, 0);
}

void f_2b1cc(void)
{
	f_29c40(7, 0, 0);
	f_29c76(0xff, 0, 0);
}

void f_2b1e4(void)
{
	struct fade *e = g_6064550;
	int i;

	for (i = 0; i < 32; i++, e++) {
		if (e->list) {
			if (--e->timer == 0) {
				*e->dst = e->list[e->idx] & 0xffffff00;
				if ((unsigned char)e->list[e->idx])
					e->idx = 0;
				else
					e->idx++;
				e->timer = e->reload;
			}
		}
	}
}

void f_2b31e(void)
{
	int i;

	for (i = 0; i < 32; i++)
		g_6064550[i].list = 0;
}

void f_2b338(short pal, short col, short rate, long *list)
{
	long *dst = &g_24040000[pal][col];
	int i;

	for (i = 0; i < 32 && g_6064550[i].dst != dst && g_6064550[i].list != 0; i++)
		;
	g_6064550[i].list = list;
	g_6064550[i].dst = dst;
	g_6064550[i].reload = rate;
	g_6064550[i].timer = (char)rate;
	g_6064550[i].idx = 0;
}

void f_2b39e(short pal, short col)
{
	long *dst = &g_24040000[pal][col];
	int i;

	for (i = 0; g_6064550[i].dst != dst && i < 32; i++)
		;
	if (i < 32) {
		g_6064550[i].list = 0;
		g_6064550[i].dst = 0;
	}
}

void f_2b3e8(short *p)
{
	short pal;
	long v;

	while (*p == 0xa1) {
		p++;
		pal = *p++;
		v = *p++ << 16;
		v |= (unsigned short)*p++;
		f_2b0ea(pal, (short **)v);
	}
}
