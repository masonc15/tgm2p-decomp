/* rom: 0x2f68 len: 0xa9a func: f_2f68 flags: -macsave=1 -optimize=1 -speed */
struct player;
struct slot {
	char pad0[12];
	unsigned long l0c;         /* 0x0c; unsigned lets the store reuse the zero in r13 */
	char pad1[60 - 16];
};
struct player {
	char pad0[0x2f4];
	struct player *other;      /* 0x2f4 */
	char pad1[0x2fe - 0x2f8];
	char b2fe;                 /* 0x2fe */
	char pad2;
	short w300;                /* 0x300 */
	short w302;                /* 0x302 */
	short w304;                /* 0x304 */
	unsigned short w306;       /* 0x306 */
	unsigned long l308;        /* 0x308 */
	unsigned short w30c;       /* 0x30c */
	unsigned char idx;         /* 0x30e */
	char pad3[0x314 - 0x30f];
	short x;                   /* 0x314 */
	char pad4[0x322 - 0x316];
	unsigned short level;      /* 0x322 */
	char pad5[0x33a - 0x324];
	char b33a[6];              /* 0x33a */
	char pad6[0x347 - 0x340];
	char b347;                 /* 0x347 */
	char b348;                 /* 0x348 */
	char b349;                 /* 0x349 */
	char pad7[0x350 - 0x34a];
	long l350;                 /* 0x350 */
	long l354;                 /* 0x354 */
	char pad8[0x35c - 0x358];
	unsigned short w35c;       /* 0x35c */
	char pad9[0x38a - 0x35e];
	unsigned short w38a;                /* 0x38a */
	char pad10[0x39a - 0x38c];
	char b39a;                 /* 0x39a */
	char pad11;
	short w39c;                /* 0x39c */
};
extern unsigned long g_6064880;
extern unsigned char g_6079374[];
extern unsigned char g_606475a[];
extern unsigned char g_6079284[];
extern unsigned char g_6079286[];
extern unsigned char g_6079288[];
extern short g_607928a[];
extern struct slot g_60794c0[2];
extern short d_39812[];
extern short d_39bfc[];
extern short d_3903e[];
extern short d_3a3d0[];
extern short d_39fe6[];
extern short d_39428[];
extern unsigned char f_81cc(struct player *);
extern void f_20c14(struct player *);
extern void f_12e2(struct player *, int);
extern void f_1175c(struct player *);
extern void f_21454(struct player *, int, int);
extern void f_15f10(struct player *, int);
extern void f_13848(struct player *);
extern void f_8314(struct player *);

void f_2f68(struct player *p, int s)
{
	p->w35c &= 0xff00;
	p->w35c |= s;
}

void f_2f7a(struct player *p)
{
	p->l308 |= 0x40000;
	f_2f68(p, 10);
	p->w300 = 80;
}

void f_2fa2(struct player *p)
{
	f_2f68(p, 12);
	p->w35c &= 0x7fff;
}

void f_2fc0(struct player *p)
{
	int i;

	f_2f68(p, 2);
	p->l308 |= 0x20000;
	if (g_6064880 & 4)
		p->l308 &= ~0x4000;
	else
		p->l308 |= 0x4000;
	g_6079286[p->idx] = 0;
	g_6079284[p->idx] = 0;
	p->w39c = 0;
	if (p->w30c & 0x80) {
	} else if (p->w30c & 0x1000) {
		if (p->level > 500)
			i = 500;
		else
			i = p->level;
		p->b348 = d_39bfc[i];
	} else {
		if (p->level > 999)
			p->level = 999;
		if (p->level >= 500)
			p->b348 = d_39812[p->level - 500];
	}
	if (p->w30c & 0x2000)
		p->b348 = d_39812[499];
	p->b347 = p->b348;
}

void f_30d0(struct player *p)
{
	f_2f68(p, 3);
	p->b347 = 0;
	g_6079288[p->idx] = 0;
	p->l308 &= ~0x4000;
	p->l308 |= 0x80;
}

void f_3108(struct player *p)
{
	int i;

	f_2f68(p, 4);
	if (p->w30c & 0x1000) {
		if (p->level > 500)
				i = 500;
			else
				i = p->level;
			p->w300 = d_3a3d0[i];
	} else if (p->w30c & 0x80) {
		p->w300 = 40;
	} else {
		if (p->level > 999)
			p->level = 999;
		if (p->level >= 500)
			p->w300 = d_3903e[p->level - 500];
		else
			p->w300 = 40;
	}
	if (p->w30c & 0x2000)
		p->w300 = d_3903e[499];
	if (g_6064880 & 4)
		p->w300 = 1;
}

void f_31dc(struct player *p, unsigned char flag)
{
	int i;

	p->l308 &= ~0x80;
	f_2f68(p, 5);
	if (flag) {
		if (p->w30c & 0x1000) {
			if (p->level > 500)
				i = 500;
			else
				i = p->level;
			p->w300 = d_3a3d0[i];
		} else if (p->w30c & 0x80) {
			p->w300 = 25;
		} else {
			if (p->level > 999)
				p->level = 999;
			if (p->level >= 500)
				p->w300 = d_3903e[p->level - 500];
			else
				p->w300 = 25;
		}
	} else {
		if (p->w30c & 0x1000) {
			if (p->level > 500)
				i = 500;
			else
				i = p->level;
			p->w300 = d_39fe6[i];
		} else if (p->w30c & 0x80) {
			p->w300 = 25;
		} else {
			if (p->level >= 500)
				p->w300 = d_39428[p->level - 500];
			else
				p->w300 = 25;
		}
	}
	if (p->w30c & 0x2000)
		p->w300 = d_3903e[499];
}

void f_32fc(struct player *p)
{
	f_2f68(p, 6);
	p->b2fe = 0;
}

void f_331a(struct player *p)
{
	struct slot *q = &g_60794c0[p->idx];
	short i;

	p->w300 = 360;
	p->w302 = 1;
	p->w306 = f_81cc(p);
	g_607928a[p->idx] = 96;
	p->l308 &= ~0x24000;
	p->l308 |= 0x50000;
	if (p->l308 & 0x20)
		if (p->w30c & 0x1085)
			if (p->l308 & 0x40) {
				if (0) p->w302 = 0;	/* dead store: steers 180 into r3 */
				p->w300 = 180;
				goto skip;
			}
	p->w304 = 0;
	f_20c14(p);
skip:
	p->l308 &= ~0x20;
	p->l308 |= 0x40;
	f_2f68(p, 7);
	if (g_6064880 & 4) {
		p->other->l308 |= 0x80000040;
		p->other->l308 &= ~0x20000;
	}
	if (g_6064880 & 2) {
		f_12e2(p, 0);
		p->l350 = p->w38a = p->level = 0;
		for (i = 0; i < 6; i++)
			p->b33a[i] = 0;
		f_1175c(p);
		f_21454(p, 67, p->x + 49);
	}
	f_15f10(p, 0);
	q->l0c = 0;
	if (!(p->w30c & ~0x1f))
		if (!(p->w30c & 8))
			f_13848(p);
	if (p->w30c == 0x1000 || p->w30c == 0x80) {
		f_13848(p);
		p->l354 = p->l350;
	}
	f_8314(p);
	p->b39a = 0;
}

void f_3510(struct player *p)
{
	unsigned short m;

	f_2f68(p, 9);
	p->l308 |= 0x50000;
	p->w300 = 1;
	p->w306 = 12;
	m = p->idx == 0 ? 0x100 : 0x200;
	if (g_6064880 & 0x400)
		p->w302 = 0;
	else if (g_6064880 & m)
		p->w302 = 0x80;
	else
		p->w302 = 0;
}

void f_3574(struct player *p)
{
	f_2f68(p, 13);
	p->w300 = 0;
	p->w302 = 1;
	g_6079374[p->idx] = 0;
}

void f_35a0(struct player *p)
{
	if (g_606475a[p->idx] & 0x30)
		p->b349++;
	else
		p->b349 = 0;
}

/* Collision test for the active piece: the field is a grid of 6-byte cells
 * with walls stored as cells, and the piece shape is a 4x4 grid per rotation
 * in d_363cc, indexed by the piece type in the low nibble of flags. Big mode
 * (flag 0x200) doubles every shape cell. Overlaps are counted in g_6077608,
 * split by whether the cell has bit 15 set, and any overlap is a collision. */
struct cell {
	unsigned short v;
	char pad[4];
};
struct field {
	struct cell *cells;
	char pad0[0xde - 4];
	unsigned char h;           /* 0xde, rows including the walls */
	unsigned char w;           /* 0xdf, columns including the walls */
	char pad1[0x35e - 0xe0];
	unsigned short flags;      /* 0x35e; low nibble is the piece type */
};
struct hits {
	char pad[0x370];
	unsigned short a;          /* overlaps with plain cells */
	unsigned short b;          /* overlaps with cells that have bit 15 set */
};
extern struct hits g_6077608;
extern unsigned char d_363cc[16][4][4][4];   /* [type][row][rot][col] */

unsigned char f_35f0(struct field *f, short x, short y, unsigned char rot)
{
	unsigned short i;
	unsigned short c;
	struct cell *p;
	unsigned short k;
	unsigned short j;

	/* `g_6077608.b += 0` here and the dead store before the final return
	 * emit nothing, but they set which constants SHC keeps in registers and
	 * the order of the literal pool (0xbfff before 0x8000). */
	g_6077608.b += 0;
	k = f->flags;
	g_6077608.b = 0;
	g_6077608.a = 0;
	k &= 15;
	if (f->flags & 0x200) {
		y += 1;
		x -= 2;
		for (i = 0; i < 8; i++) {
			if ((y - i) < f->h) {
				p = &f->cells[((y - i) * f->w) + x];
				for (j = 0; j < 8; j++, p++) {
					if ((x + j) >= 0) {
						c = p->v & 0xbfff;
						if (d_363cc[k][i >> 1][rot][j >> 1] && c) {
							if (p->v & 0x8000)
								g_6077608.b++;
							else
								g_6077608.a++;
						}
					}
				}
			}
		}
	} else {
		for (i = 0; i < 4; i++) {
			if ((y - i) < f->h) {
				for (j = 0; j < 4; j++) {
					if ((x + j) >= 0) {
						c = f->cells[(((y - i) * f->w) + x) + j].v & 0xbfff;
						if ((d_363cc[k][i][rot][j] != 0) && (c != 0)) {
							if (f->cells[(((y - i) * f->w) + x) + j].v & 0x8000)
								g_6077608.b++;
							else
								g_6077608.a++;
						}
					}
				}
			}
		}
	}
	if (g_6077608.a + g_6077608.b)
		return 1;
	if (0) g_6077608.a = 0x8000;
	return 0;
}
