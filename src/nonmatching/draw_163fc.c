/* rom: 0x163fc len: 0xd60 func: f_163fc flags: -macsave=1 -optimize=1 -speed */
/* difftest-steps: f_163fc=30000000 f_16938=30000000 (the original walks a random-sized field: up to 255 x 255 cells) */
/* Non-matching, functionally equivalent C (shiftable build only).
 * f_163fc: draws the playfield frame and every occupied cell directly
 * with f_1159c (outline frame from the empty neighbours, palette from
 * the cell flags, ticking the flash counter kept in the cell), then the
 * field overlay.
 * f_16938: draws the playfield frame, then queues one sprite per occupied
 * field cell into the sprite list at g_6077608 (picking an outline frame
 * from the empty neighbours and a palette from the cell flags, ticking
 * fade/flash counters stored in the cells), submits the list in batches
 * of at most 31 via f_1159c and finally draws the field overlay. */
struct cell {
	short w0;
	signed char b2;
	unsigned char b3;
	short w4;
};
struct frame {
	short w[6];
};
struct field {
	struct cell *cells;        /* 0x000 */
	char pad0[0xde - 0x4];
	unsigned char bde;         /* 0x0de height */
	unsigned char bdf;         /* 0x0df width */
	short e0[2];               /* 0x0e0 */
	char pad1[0x2f8 - 0xe4];
	struct field *p2f8;        /* 0x2f8 */
	char pad2[0x308 - 0x2fc];
	unsigned long l308;        /* 0x308 */
	unsigned short w30c;       /* 0x30c */
	signed char b30e;          /* 0x30e */
	char pad30f[0x314 - 0x30f];
	short w314;                /* 0x314 */
	short w316;                /* 0x316 */
	char pad3[0x379 - 0x318];
	unsigned char b379;        /* 0x379 */
};

extern unsigned long g_6064880;
extern struct frame g_6077608[];
extern struct frame d_a5d38[], d_a5d44[], d_a5d50[], d_a5d5c[];
extern struct frame d_a5e1c[], d_a5e28[];
extern struct frame d_a7328[], d_a7334[];
extern short d_3b340[], d_3b356[];
extern void f_1159c(struct frame *, int, int, int, int);

/* The ROM recomputes the cell address for every access (cells may alias
 * the field header), so do the same. */
#define CUR (&f->cells[(f->bde - i - 1) * f->bdf + j])
#define LOW (&f->cells[i * f->bdf + j])

/* Outline frame for a cell from its empty neighbours (1 below, 2 above,
 * 4 left, 8 right), as the ROM's jump tables pick it. */
#define OUTLINE(k) \
	switch (k) { \
	case 1: frm = &d_a5d5c[2]; break; \
	case 2: frm = &d_a5d5c[4]; break; \
	case 3: frm = &d_a5d5c[13]; break; \
	case 4: frm = &d_a5d5c[1]; break; \
	case 5: frm = &d_a5d5c[5]; break; \
	case 6: frm = &d_a5d5c[8]; break; \
	case 7: frm = &d_a5d5c[12]; break; \
	case 8: frm = &d_a5d5c[3]; break; \
	case 9: frm = &d_a5d5c[6]; break; \
	case 10: frm = &d_a5d5c[7]; break; \
	case 11: frm = &d_a5d5c[10]; break; \
	case 12: frm = &d_a5d5c[14]; break; \
	case 13: frm = &d_a5d5c[9]; break; \
	case 14: frm = &d_a5d5c[11]; break; \
	case 15: frm = &d_a5d5c[15]; break; \
	default: frm = d_a5d5c; break; \
	}

void f_163fc(struct field *f)
{
	short a, b, y, x, i, j, k, n;
	short w;
	unsigned short uw;
	int pal, s, sz;
	struct frame *frm;

	a = f->e0[0] + f->w314;
	b = f->e0[1] + f->w316;
	if (g_6064880 & 4) {
		frm = d_a5d44;
		sz = 16;
	} else {
		if (f->w30c & 10)
			frm = d_a5d38;
		else
			frm = d_a5d50;
		if (f->b30e)
			sz = 32;
		else
			sz = 16;
	}
	f_1159c(frm, b, a, sz, 80);
	y = b - (f->bde - 1) * 8 - 6;
	for (i = 0; i < f->bde; i++) {
		x = a - (f->bdf / 2) * 8;
		for (j = 0; j < f->bdf; j++) {
			w = CUR->w0;
			if ((w & 15) == 0 || (w & 15) == 1)
				goto next;
			k = 0;
			if (i != 0 && f->cells[(f->bde - i - 2) * f->bdf + j].w0 == 0)
				k = 2;
			if (i != f->bde - 1 && f->cells[(f->bde - i) * f->bdf + j].w0 == 0)
				k |= 1;
			if (j != 1 && f->cells[(f->bde - i - 1) * f->bdf + j - 1].w0 == 0)
				k |= 4;
			if (j != f->bdf - 2 && f->cells[(f->bde - i - 1) * f->bdf + j + 1].w0 == 0)
				k |= 8;
			frm = d_a5d5c;
			if (f->p2f8->b379 != 3 && f->b379 != 0xff)
				OUTLINE(k);
			uw = w;
			if (uw & 0x2000)
				frm = &d_a5e28[CUR->b3 - 1];
			if (uw & 0x80) {
				n = ((uw & 0x70) >> 4) - 1;
				if (n == 0)
					CUR->w0 = w & 0xff7f;
				else
					CUR->w0 = (w & 0xff8f) | (n << 4);
				pal = 0x89;
			} else {
				pal = d_3b340[(uw & 15) - 2] + 5;
				if (f->b379 == 0xff) {
					s = CUR->b2;
					if ((unsigned short)s > 5)
						s = 5;
					pal -= s;
				} else {
					if (uw & 0x400) {
						frm = d_a5e1c;
						pal = 0x89;
					}
					if (uw & 0x2000)
						pal = d_3b356[CUR->b3 - 1] + 5;
					if (f->p2f8->b379 == 3) {
						s = LOW->b2;
						if (s < 0 || s > 9)
							pal = 0x89;
						else
							pal += LOW->b2 - 1;
					}
				}
			}
			if (!(uw & 0x4000))
				f_1159c(frm, y, x, pal, 80);
next:
			x += 8;
		}
		y += 8;
	}
	if (g_6064880 & 4)
		frm = d_a7334;
	else
		frm = d_a7328;
	f_1159c(frm, b, a, 0x94, 60);
}

void f_16938(struct field *f)
{
	short a, b, y, ym, x, i, j, cnt, k, n;
	short notb;
	unsigned short uw;
	short w;
	int pal, s, off, sz, t;
	struct frame *frm, *dst;

	cnt = 0;
	a = f->e0[0] + f->w314;
	b = f->e0[1] + f->w316;
	if (g_6064880 & 4) {
		frm = d_a5d44;
		sz = 16;
	} else {
		if (f->w30c & 0x1000)
			frm = d_a5d50;
		else if (f->w30c & 0x8a)
			frm = d_a5d38;
		else
			frm = d_a5d50;
		if ((f->l308 & 1) && (f->w30c & 4))
			frm = d_a5d38;
		if (f->b30e)
			sz = 32;
		else
			sz = 16;
	}
	f_1159c(frm, b, a, sz, 80);
	dst = g_6077608;
	notb = (f->l308 & 4) == 0;
	y = b - (f->bde - 1) * 8 - 6;
	for (i = 0; i < f->bde; i++) {
		x = a - (f->bdf / 2) * 8;
		ym = y & 0x3ff;
		for (j = 0; j < f->bdf; j++) {
			w = CUR->w0;
			if ((w & 15) == 0 || (w & 15) == 1)
				goto next;
			k = 0;
			if (i != 0 && f->cells[(f->bde - i - 2) * f->bdf + j].w0 == 0)
				k = 2;
			if (i != f->bde - 1 && f->cells[(f->bde - i) * f->bdf + j].w0 == 0)
				k |= 1;
			if (j != 1 && f->cells[(f->bde - i - 1) * f->bdf + j - 1].w0 == 0)
				k |= 4;
			if (j != f->bdf - 2 && f->cells[(f->bde - i - 1) * f->bdf + j + 1].w0 == 0)
				k |= 8;
			frm = d_a5d5c;
			if (f->p2f8->b379 != 3 && f->b379 != 0xff && !(w & 0x1000) && !notb) {
				OUTLINE(k);
			}
			uw = w;
			if (uw & 0x2000)
				frm = &d_a5e28[CUR->b3 - 1];
			if (uw & 0x80) {
				n = ((uw & 0x70) >> 4) - 1;
				if (n == 0)
					CUR->w0 = w & 0xff7f;
				else
					CUR->w0 = (w & 0xff8f) | (n << 4);
				pal = 0x89;
			} else {
				off = (uw & 15) - 2;
				pal = d_3b340[off] + 5;
				if (f->b379 == 0xff) {
					if (uw & 0x2000)
						pal = d_3b356[CUR->b3 - 1] + 5;
					s = CUR->b2;
					if ((short)s > 5)
						s = 5;
					pal -= s;
				} else {
					if (uw & 0x400) {
						frm = d_a5e1c;
						pal = 0x89;
					}
					if (uw & 0x2000)
						pal = d_3b356[CUR->b3 - 1] + 5;
					t = f->p2f8->b379;
					if (t == 3 || t == 19) {
						s = LOW->b2;
						if (s < 0 || s > 9)
							pal = 0x89;
						else {
							if (uw & 0x400)
								pal = 0x80;
							else if (uw & 0x2000)
								pal = d_3b356[CUR->b3 - 1];
							else
								pal = d_3b340[off];
							pal += LOW->b2;
						}
					} else if (uw & 0x1000) {
						if (CUR->w4 != 0) {
							CUR->w4--;
							if (CUR->w4 < 10)
								pal -= 5 - CUR->w4 / 2;
						} else {
							CUR->w0 |= 0x4000;
							pal = d_3b340[off];
						}
					}
				}
			}
			if ((uw & 0x1000) && CUR->w4 == 0)
				goto next;
			if (uw & 0x4000)
				goto next;
			dst->w[0] = frm->w[0];
			dst->w[1] = frm->w[1];
			dst->w[2] = frm->w[2];
			dst->w[3] = frm->w[3];
			dst->w[4] = frm->w[4];
			dst->w[5] = frm->w[5];
			dst->w[1] = (x & 0x3ff) | 0x400;
			dst->w[0] = ym;
			((unsigned char *)dst)[8] = pal;
			dst++;
			cnt++;
next:
			x += 8;
		}
		y += 8;
	}
	dst = g_6077608;
	while (cnt != 0) {
		n = cnt > 31 ? 31 : cnt;
		dst->w[1] = (dst->w[1] & 0x3ff) | ((unsigned short)n << 10);
		f_1159c(dst, 0, 0, 0, 80);
		dst += (unsigned short)n;
		cnt -= n;
	}
	if (g_6064880 & 4)
		frm = d_a7334;
	else
		frm = d_a7328;
	f_1159c(frm, b, a, 0x94, 60);
}
