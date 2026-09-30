/* rom: 0x101b8 len: 0x218 func: f_101b8 flags: -macsave=1 -optimize=1 -speed */
/* Byte-exact prefix (f_101b8, f_10228 and the literal pool after them) of the HUD source file
 * 0xf958-0x11518.  The pool for both functions is dumped inside f_10228 (after its first bra) and at
 * its end, so the prefix is 0x101b8-0x103d0.  Nothing in it branches or loads past 0x103d0. */
struct pl {
	char pad0[0x2f4];
	struct pl *owner;          /* 0x2f4 */
	char pad1[0x30e - 0x2f8];
	unsigned char b30e;        /* 0x30e */
	char pad2[0x314 - 0x30f];
	short pos[2];              /* 0x314 */
	char pad3[0x379 - 0x318];
	unsigned char b379;        /* 0x379 */
	char pad4[0x380 - 0x37a];
	char b380;                 /* 0x380 */
	char pad5[0x3b4 - 0x381];
};
struct ent {
	char pad0;
	char b1;
	char pad1[0x80c - 2];
	struct pl *owner;          /* 0x80c */
	long pad2;
	struct ent *next;          /* 0x814 */
};
struct frame { char pad[12]; };
extern unsigned char g_6079374[];
extern struct frame d_aa934[];
extern char f_18ed4(unsigned short);
extern void f_1159c(long, short, short, unsigned char, short);

void f_101b8(struct pl *p)
{
	if (g_6079374[p->b30e]) {
		f_1159c((long)&d_aa934[g_6079374[p->b30e]], p->pos[1] - 190, p->pos[0] + 20, 4, 61);
	}
	if (p->b380)
		g_6079374[p->b30e] = p->b380;
}

void f_10228(struct ent *p)
{
	struct ent *t, *n;
	struct pl *q;
	char a[2], b[2], c[2];
	char i;
	int r;
	unsigned char id;
	char k, v;
	int y, x;
	char s;

	a[0] = a[1] = 0;
	b[0] = b[1] = 0;
	c[0] = c[1] = 0;
	if (0) f_1159c(0, y, 0, 0, 0); /* dead code: steers SHC register ranking */
	t = p;
	if ((r = f_18ed4(t->b1)) == 0)
		return;
	q = t->owner;
	a[q->b30e]++;
	b[q->b30e] = r;
	n = p->next;
	for (i = 0; i < 18; i++) {
		if (n == 0)
			break;
		t = n;
		n = t->next;
		r = f_18ed4(t->b1);
		if (r != 0) {
			q = t->owner;
			k = r - 1;
			id = q->b30e;
			k ^= id;
			a[id]++;
			b[q->b30e] = r;
			if (a[0] == 1 && a[1] == 1 && b[0] == b[1])
				continue;
			if (a[q->b30e] == 1 && q->b379 == t->b1)
				continue;
			c[k]++;
			v = t->b1;
			s = r;
			switch (k + s) {
			case 1:
			case 3:
			case 5:
				y = q->pos[1] + 5;
				x = q->pos[0] - 60;
				break;
			case 2:
			case 6:
				y = q->pos[1] + 5;
				x = q->pos[0] + 30;
				break;
			}
			if (v)
				f_1159c((long)&d_aa934[v], y, x, 4, 40);
		}
	}
}
