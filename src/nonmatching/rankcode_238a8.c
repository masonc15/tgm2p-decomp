/* rom: 0x238a8 len: 0x596 func: f_238a8 flags: -macsave=1 -optimize=1 -speed */
/* Non-matching, functionally equivalent C (shiftable build only).
 * f_238a8: relinks the 16-node ring at g_6077608 so that node 14 and 15
 *   sit after node n-1 (node 13 is cut off).
 * f_238fc: builds the 16-character ranking code for a player: packs mode,
 *   grade, time and level bits plus a random salt into the nodes, adds a
 *   checksum, scrambles with d_3b5fc and spells it out through d_3b6fc
 *   into p->code, starting from the node picked by the record checksum. */
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
extern struct rec g_60794c0[2];
extern char d_3b5fc[];
extern char d_3b6fc[];
extern int f_2beca(int);

void f_238a8(int n)
{
	char *base = (char *)g_6077608;
	unsigned char a = (n - 1) * 12;
	short b = (n - 1) * 12;

	g_6077608[13].next = 0;
	((struct node *)(base + a))->next = &g_6077608[14];
	g_6077608[14].prev = (struct node *)(base + a);
	g_6077608[15].next = (struct node *)(base + b + 12);
	((struct node *)(base + b + 12))->prev = &g_6077608[15];
}

void f_238fc(struct player *p)
{
	struct node *n;
	struct rec *q;
	short i;
	int r, k;
	unsigned short sum;
	unsigned int t;
	unsigned long v;
	unsigned int cs;
	char *s;
	unsigned char *c;

	g_6077608[0].prev = &g_6077608[15];
	q = (struct rec *)((char *)g_60794c0 + (signed char)(p->b30e * 60));
	for (i = 0; i < 15; i++) {
		*((unsigned char *)g_6077608 + (unsigned char)(i * 12)) = 0;
		g_6077608[i].next = &g_6077608[i + 1];
		g_6077608[i + 1].prev = (struct node *)((unsigned long)g_6077608 + (unsigned char)(i * 12));
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
	/* k still holds w38a here, as in the ROM */
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
	cs = (unsigned char)cs;
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
