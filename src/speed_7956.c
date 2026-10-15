/* rom: 0x7956 len: 0x2b2 func: f_7956 context-rom: 0x7658 flags: -macsave=1 -optimize=1 -speed */
/* Retain only f_7956, ROM 0x7956-0x7c08. The preceding f_7658 is genuine,
 * unlinked compiler context and remains unmatched. Its shared literal pool
 * layout is checked by the default range verifier. No register forcing. */
struct player {
	char pad0[0x2f4];
	struct player *other;      /* 0x2f4 */
	char pad1[0x300 - 0x2f8];
	short a300[4];             /* 0x300 */
	unsigned long l308;        /* 0x308 */
	unsigned short w30c;       /* 0x30c */
	unsigned char idx;         /* 0x30e */
	char pad3[0x314 - 0x30f];
	short x;                   /* 0x314 */
	char pad4[0x322 - 0x316];
	unsigned short level;      /* 0x322 */
	char pad5[0x328 - 0x324];
	unsigned short w328;       /* 0x328 */
	char pad6[0x348 - 0x32a];
	char b348;        /* 0x348 */
	char pad7[0x350 - 0x349];
	unsigned long l350;        /* 0x350 */
	char pad8[0x38a - 0x354];
	short w38a;                /* 0x38a */
	char pad9[0x39c - 0x38c];
	short w39c;                /* 0x39c */
	char pad10[0x3a1 - 0x39e];
	unsigned char b3a1;        /* 0x3a1 */
};
extern unsigned long g_6064880;
extern unsigned long g_6060008;
extern short g_6064886;
extern unsigned char g_606475e[];
extern unsigned char g_6079284[];
extern unsigned char g_6079286[];
extern short d_38f58[];
extern short d_38f60;
extern unsigned long d_3a8c0[];
extern void f_117a8(struct player *, char);
extern void f_1199c(struct player *);
extern void f_11cc6(struct player *);
extern void f_2e6fc(int);
extern int f_2beca(int);
extern void f_17834(struct player *, int, int, int);
extern void f_1159c(long, long, long, unsigned char, short);
extern void f_11254(long, short, int, char, short, short, long, unsigned char);
extern void f_11680(unsigned long, short, short, unsigned char, short, short, short, unsigned char);

int f_7658(struct player *p, unsigned char n)
{
	unsigned char h;
	short q, r, d;
	int s;

	if (p->w30c & 0x40) {
		h = n >> 1;
		if (n % 2)
			n++;
	} else {
		h = n;
	}
	if (p->w30c & 0x40) {
		if (n > 7) {
			p->w328 += (h - 1) * 2;
			p->level = p->level + 4;
		} else if (n > 4) {
			p->w328 += (h - 1) * 2;
			p->level = p->level + 3;
		} else if (n > 2) {
			p->w328 += (h - 1) * 2;
			p->level = p->level + 2;
		} else if (n >= 1) {
			p->level += 1;
		} else {
			p->w328 = 1;
		}
	} else {
		if (n > 3) {
			p->w328 += (n - 1) * 2;
			p->level += 4;
		} else if (n > 2) {
			p->w328 += (n - 1) * 2;
			p->level += 3;
		} else if (n > 1) {
			p->w328 += (n - 1) * 2;
			p->level += 2;
		} else if (n == 1) {
			p->level += 1;
		} else {
			p->w328 = 1;
		}
	}
	f_117a8(p, h);
	f_1199c(p);
	if (n) {
		if ((p->w30c & 0x1000) && (g_6064880 & 1) && d_38f60 <= p->level
		    && !p->b3a1 && p->l350 <= 0x300c) {
			p->b3a1 = 1;
			p->w38a = 18;
			f_2e6fc(15);
		}
		f_11cc6(p);
	}
	q = p->level / 4;
	if (p->level % 4)
		q++;
	if ((p->w30c & 1) && (g_6064880 & 1)) {
		if (d_38f58[2] <= p->level)
			p->level = d_38f58[2];
	} else if ((p->w30c & 0x82) && (g_6064880 & 1)) {
		if (d_38f58[9] <= p->level)
			p->level = d_38f58[9];
	} else if ((p->w30c & 0x1000) && (g_6064880 & 1)) {
		if (p->b3a1) {
			if (d_38f58[9] <= p->level)
				p->level = d_38f58[9];
		} else {
			if (d_38f58[4] <= p->level)
				p->level = d_38f58[4];
		}
	} else if (p->w30c & 4) {
		if (d_38f58[2] <= p->level)
			p->level = d_38f58[2];
	}
	s = (q + g_6079284[p->idx] + g_6079286[p->idx] * 2) * n * p->w328;
	r = p->level / 2;
	if (p->level % 2)
		r++;
	if (s)
		s += r;
	d = p->b348 - p->w39c;
	if (d > 0 && s)
		s += d * 7;
	return s;
}

void f_7956(struct player *p)
{
	unsigned short mask;
	unsigned short mode;
	int dy, k;
	int t, ox, oy;
	short v;

	mask = p->idx == 0 ? 0x100 : 0x200;
	if (g_6064880 & 0x400) {
		mode = 0;
		dy = 0;
		k = 48;
		if (p->a300[1] < 63)
			p->a300[1] += 4;
		else
			p->a300[1] = 63;
	} else if (g_6064880 & mask) {
		mode = 1;
		if (g_6060008 & 3)
			dy = 8;
		else
			dy = 0;
		k = 32;
		if (p->a300[1] > 63)
			p->a300[1] -= 4;
		else
			p->a300[1] = 63;
		if (g_606475e[p->idx] & 0xfe)
			p->a300[0]++;
		if (g_6064886 > 10) {
			if (p->a300[3]-- == 0 || p->a300[0] % 5 == 0) {
				p->a300[3] = 20;
				p->a300[0] = 1;
				p->a300[2] = (p->a300[2] + 1) & 1;
				f_2e6fc(32);
				f_17834(p, f_2beca(13) + 5, f_2beca(10), f_2beca(10));
			}
		}
		if (g_6064880 & 0x80000) {
			f_1159c(0xaaeb0, 0x8c, p->x, 0, 110);
		} else if (g_6064880 & 0x40000) {
			f_1159c(0xaaebc, 0x8c, p->x - 35, 0, 110);
			f_1159c(0xaaec8, 0x99, p->x - 8, 0, 110);
			f_11254(p->level, 0x96, p->x - 36, 0, 110, 3, 0, 2);
			f_11254(p->other->level, 0x96, p->x + 10, 0, 110, 3, 0, 2);
		}
	} else {
		mode = 2;
		dy = 0;
		k = 48;
		if (p->a300[1] < 63)
			p->a300[1] += 4;
		else
			p->a300[1] = 63;
	}
	v = p->a300[1];
	ox = -(k * (t = ((v - 63) * 1024) / 63));
	ox >>= 10;
	oy = -(t * 8);
	oy >>= 10;
	f_11680(d_3a8c0[mode], oy + 120, p->x + ox, dy, 125, v, v, 0);
}
