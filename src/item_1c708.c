/* rom: 0x1c708 len: 0xab4 func: f_1c708 flags: -macsave=1 -optimize=1 -speed */
struct cell {
	unsigned short flags;
	unsigned char b2;
	unsigned char b3;
	short b;
};
struct field {
	struct cell *cells;        /* 0x000 */
	short garbage[8][12];      /* 0x004 */
	char pad0[0xdf - 0xc4];
	unsigned char width;       /* 0x0df */
	short e0;                  /* 0x0e0 */
	short e2;                  /* 0x0e2 */
	char pad1[0x2f8 - 0xe4];
	struct field *other;       /* 0x2f8 */
	char pad2[0x308 - 0x2fc];
	unsigned long flags308;    /* 0x308 */
	unsigned short mode;       /* 0x30c */
	char pad3[0x35c - 0x30e];
	union {
		unsigned short w;
		unsigned char b[2];
	} u35c;                    /* 0x35c */
	char pad5[0x379 - 0x35e];
	unsigned char b379;        /* 0x379 */
	char pad6[0x388 - 0x37a];
	unsigned char b388;        /* 0x388 */
};
struct player {
	char b0;
	char b1;
	short w[3];
	char c[4];
	char d[0x800];
	struct field *field;       /* 0x80c */
};
struct state {
	short hit;
	short n;
	short rows;
};
/* garbage row n, with the byte offset truncated the way the ROM does */
#define GROW(f, n) ((short *)((char *)(f)->garbage + (unsigned char)((n) * 24)))

extern unsigned long g_6064880;
extern int f_18950(struct player *);
extern void field_clear_flag(struct field *);
extern int f_18fb4(struct field *);
extern int f_18e9c(struct field *);
extern void f_15f10(struct field *, int);
extern void f_2e6fc(int);
extern int f_18be0(struct field *);
extern long f_2beca(int);
extern void f_17d48(struct field *, int);
extern void f_1919e(struct player *);

void f_1c708(struct player *p)
{
	struct field *q = p->field;
	struct field *f = q->other;
	struct state *d = (struct state *)p->d;
	short x, y, k, h, hole, row, n;
	int m;

	if (f_18950(p))
		return;
	if (f_18950(p))
		return;
	switch (p->c[0]) {
	case 0:
		d->hit = 0;
		d->n = 0;
		d->rows = 0;
		p->c[0]++;
	case 1:
		field_clear_flag(q);
		if (q->flags308 & 0x80)
			return;
		if (q->b379)
			return;
		if (f_18fb4(f))
			d->hit = 1;
		if (f_18e9c(f))
			return;
		q->b379 = 15;
		f_15f10(q, 15);
		if (g_6064880 & 2)
			f_15f10(f, 15);
		q->u35c.w |= 0x8000;
		if (d->hit)
			p->c[0]++;
		else {
			q->flags308 |= 0x40000000;
			p->c[0] += 2;
		}
		break;
	case 2:
		if (q->u35c.b[1] == 12) {
			q->flags308 |= 0x40000000;
			p->c[0]++;
		}
		break;
	case 3:
		p->w[0] = 30;
		p->c[0]++;
		for (x = 1; x < 11; x++) {
			q->cells[21 * q->width + x].flags = 0;
			q->cells[21 * q->width + x].b3 = 0;
		}
		break;
	case 4:
		if (--p->w[0] == 0) {
			p->w[1] = 4;
			p->c[0]++;
		}
		break;
	case 5:
		q->e2 -= 8;
		if (--p->w[1] == 0) {
			p->w[1] = 2;
			p->c[0]++;
		}
		break;
	case 6:
		q->e2 += 16;
		if (--p->w[1] == 0) {
			f_2e6fc(25);
			p->c[0]++;
		}
		break;
	case 7:
		d->n++;
		if (d->n >= 2) {
			q->e2 = 0;
			p->w[1] = 4;
			p->c[0]++;
		} else {
			p->w[0] = 10;
			p->c[0] = 4;
		}
		break;
	case 8:
		if (--p->w[1] == 0) {
			for (x = 1; x < 11; x++)
				for (y = 1; y < 20; y++)
					if (q->cells[y * q->width + x].flags == 0)
						for (k = y + 1; ; k++) {
							if (k > 20)
								break;
							if (q->cells[k * q->width + x].flags) {
								q->cells[y * q->width + x] = q->cells[k * q->width + x];
								q->cells[k * q->width + x].flags = 0;
								q->cells[k * q->width + x].b3 = 0;
								break;
							}
						}
			p->c[0]++;
			p->w[1] = 25;
		}
		break;
	case 9:
		if (--p->w[1] == 0)
			p->c[0]++;
		break;
	case 10:
		f_18be0(q);
		for (h = 1; h < 21; h++) {
			for (x = 1; x < 11; x++)
				if (!(q->cells[h * q->width + x].flags & 15))
					break;
			if (x < 11)
				break;
		}
		h--;
		if ((g_6064880 & 2) || (q->mode & 0x200)) {
			m = 0;
			hole = (unsigned long)f_2beca(0x4a8) % 10 + 1;
			for (row = h; row; row--, m++) {
				f_17d48(q, row);
				for (x = 1; x < 11; x++) {
					if (f->b388 + m < 8) {
						if (x == hole)
							GROW(f, f->b388 + m)[x - 1] = 0;
						else
							GROW(f, f->b388 + m)[x - 1] = q->cells[row * q->width + x].flags;
					}
					q->cells[row * q->width + x].flags = 0;
					q->cells[row * q->width + x].b3 = 0;
				}
			}
			if (f->b388 + m > 8)
				m = 8 - f->b388;
			if (m > 1) {
				f->b388 += m;
				f_2e6fc(20);
			}
		} else {
			m = 0;
			for (row = h; row; row--, m++) {
				f_17d48(q, row);
				for (x = 1; x < 11; x++) {
					q->cells[row * q->width + x].flags = 0;
					q->cells[row * q->width + x].b3 = 0;
				}
			}
			if (m > 1)
				f_2e6fc(20);
		}
		d->rows = h;
		if (d->rows == 0) {
			p->w[1] = 40;
			p->c[0] = 13;
		} else {
			p->w[1] = 25;
			p->c[0]++;
		}
		break;
	case 11:
		if (--p->w[1] == 0) {
			p->c[0]++;
			break;
		}
		break;
	case 12:
		if (d->rows) {
			n = d->rows;
			f_2e6fc(25);
			for (y = 1; y < 21 - n; y++)
				for (x = 1; x < 11; x++)
					q->cells[y * q->width + x] = q->cells[(y + n) * q->width + x];
			for (y = 21 - n; y < 21; y++)
				for (x = 1; x < 11; x++) {
					q->cells[y * q->width + x].flags = 0;
					q->cells[y * q->width + x].b3 = 0;
				}
		}
		p->w[1] = 40;
		p->c[0]++;
		break;
	case 13:
	default:
		if (--p->w[1] == 0) {
			d->hit = 0;
			q->b379 = 0;
			f_15f10(q, 0);
			if (g_6064880 & 2)
				f_15f10(f, 0);
			q->e2 = 0;
			q->flags308 &= 0xbfffffff;
			q->u35c.w &= 0x7fff;
			f_1919e(p);
		}
		break;
	}
}
