/* rom: 0x238fc len: 0x60c func: f_238fc flags: -macsave=1 -optimize=1 -speed */
/* NOT FINAL, 0x238fc-0x23f08 (f_238fc builds the 16-character ranking code
 * into p->code, f_23e3e draws it; pool word 0x0600dfd0 is f_e750).
 * f_238fc: semantics believed right, structure not: alignscore -r 0.565.
 * The ROM recomputes node addresses (base r14 + hoisted 0xa8/0xb4 in r8/r9,
 * 15 in r11, 0 in r12) where SHC here CSEs them and spills one; the first
 * loop's u8/short offsets come out right with a plain `int i` and
 * g[i + 1]. The ROM exts.b after the b30e * 60 multiply means q wants
 * (signed char)(p->b30e * 60) as a byte offset (not applied yet).
 * f_23e3e: register-exact, scheduling only; 60% after this f_238fc, 90%
 * with `extern struct node g_6077608[];` (file state). */
struct node {
	unsigned char v;           /* 0x0 */
	char pad1[3];
	struct node *prev;         /* 0x4 */
	struct node *next;         /* 0x8 */
};
struct rec {
	char pad00[0x14];
	unsigned char c[3];        /* 0x14 */
	char pad17[60 - 0x17];
};
struct player {
	char pad000[0x30c];
	unsigned short w30c;       /* 0x30c */
	char b30e;                 /* 0x30e */
	char pad30f[0x314 - 0x30f];
	short w314;                /* 0x314 */
	char pad316[0x322 - 0x316];
	unsigned short w322;       /* 0x322 */
	char pad324[0x330 - 0x324];
	unsigned long l330;        /* 0x330 */
	char pad334[0x339 - 0x334];
	unsigned char b339;        /* 0x339 */
	unsigned char g[6];        /* 0x33a */
	char pad340[0x354 - 0x340];
	unsigned long l354;        /* 0x354 */
	char pad358[0x38a - 0x358];
	unsigned short w38a;       /* 0x38a */
	char pad38c[0x3a1 - 0x38c];
	char b3a1;                 /* 0x3a1 */
	char code[16];             /* 0x3a2 */
};
extern struct node g_6077608[16];
extern struct rec g_60794c0[];
extern char d_3b5fc[];
extern char d_3b6fc[];
extern int f_2beca(int);
extern void f_e750(int, int, char *, int);

void f_238fc(struct player *p)
{
	struct node *n;
	struct rec *q;
	int i;
	int r, k, sum;
	unsigned int t;
	unsigned long v;
	unsigned char cs;
	char *s;
	unsigned char *c;

	g_6077608[0].prev = &g_6077608[15];
	q = &g_60794c0[p->b30e];
	for (i = 0; i < 15; i++) {
		g_6077608[i].v = 0;
		g_6077608[i].next = &g_6077608[i + 1];
		g_6077608[i + 1].prev = &g_6077608[i];
	}
	g_6077608[15].next = g_6077608;
	c = q->c;
	sum = 0;
	sum += c[0];
	sum += c[1];
	sum += c[2];
	r = f_2beca(32);
	g_6077608[15].v = r & 15;
	g_6077608[14].v = (r >> 4) & 1;
	g_6077608[14].v |= (p->g[5] & 3) << 1;
	t = p->g[4] & 3;
	g_6077608[14].v |= (t & 1) << 3;
	g_6077608[11].v = (t >> 1) & 1;
	g_6077608[11].v |= (p->g[3] & 3) << 1;
	t = p->g[2] & 3;
	g_6077608[11].v |= (t & 1) << 3;
	g_6077608[10].v = (t >> 1) & 1;
	g_6077608[10].v |= (p->g[1] & 3) << 1;
	t = p->g[0] & 3;
	g_6077608[10].v |= (t & 1) << 3;
	g_6077608[9].v = (t >> 1) & 1;
	if (p->w30c & 1)
		v = p->l330;
	else {
		v = p->l354;
		if (v > 0xd2f0)
			v = 0xd2f1;
	}
	g_6077608[9].v |= (v & 7) << 1;
	g_6077608[8].v = (v >> 3) & 15;
	g_6077608[7].v = (v >> 7) & 15;
	g_6077608[6].v = (v >> 11) & 15;
	g_6077608[5].v = (v >> 15) & 15;
	g_6077608[4].v = (v >> 19) & 1;
	k = 0;
	if (p->w30c & 0x1000) {
		if (p->b3a1) {
			if (p->w322 == 999) {
				if (p->b339 & 0x20)
					k = 2;
				else
					k = 1;
			}
		} else if (p->w322 == 500) {
			if (p->b339 & 0x20)
				k = 2;
			else
				k = 1;
		}
	} else if (p->w30c & 1) {
		if (p->w322 == 300) {
			if (p->b339 & 0x20)
				k = 2;
			else
				k = 1;
		}
	} else if (p->w322 == 999) {
		if (p->b339 & 0x20)
			k = 2;
		else
			k = 1;
	}
	g_6077608[4].v |= k << 1;
	g_6077608[4].v |= (p->w322 & 1) << 3;
	g_6077608[3].v = (p->w322 >> 1) & 15;
	g_6077608[2].v = (p->w322 >> 5) & 15;
	g_6077608[1].v = (p->w322 >> 9) & 1;
	k = p->w38a;
	g_6077608[1].v |= (k & 7) << 1;
	g_6077608[0].v = (k >> 3) & 3;
	if (p->w30c & 0x1000)
		k = 3;
	if (p->w30c & 0x80)
		k = 2;
	if (p->w30c & 2)
		k = 1;
	if (p->w30c & 1)
		k = 0;
	g_6077608[0].v |= k << 2;
	cs = 0;
	for (i = 0; i < 12; i++)
		cs += g_6077608[(unsigned char)i].v;
	cs += g_6077608[15].v;
	cs += g_6077608[14].v;
	cs += sum;
	g_6077608[13].v = cs & 15;
	g_6077608[12].v = (cs >> 4) & 15;
	for (i = 0; i < 12; i++) {
		g_6077608[(unsigned char)i].v += d_3b5fc[r];
		g_6077608[(unsigned char)i].v %= 20;
		r = (r + 1) % 256;
	}
	n = &g_6077608[(unsigned char)(sum & 15)];
	s = p->code;
	for (i = 0; i < 16; i++) {
		*s++ = d_3b6fc[n->v % 20];
		n = n->next;
	}
}

void f_23e3e(struct player *p)
{
	int i, j;
	char *s;
	char buf[2];

	for (i = 0; i < 2; i++) {
		s = &p->code[i * 8];
		for (j = 0; j < 8; j++) {
			buf[0] = *s++;
			buf[1] = 0;
			f_e750(p->w314 + j * 10 - 40, i * 12 + 150, buf, 1);
		}
	}
}
