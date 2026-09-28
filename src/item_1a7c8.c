/* rom: 0x1a7c8 len: 0x620 func: f_1a7c8 flags: -macsave=1 -optimize=1 -speed */
struct cell {
	unsigned short flags;
	unsigned char b2;
	unsigned char b3;
	short b;
};
struct field {
	struct cell *cells;        /* 0x000 */
	char pad0[0xdf - 4];
	unsigned char width;       /* 0x0df */
	char pad1[0x2f8 - 0xe0];
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
};
struct player {
	char b0;
	char b1;
	short w[3];
	char c[4];
	char d[0x800];
	struct field *field;       /* 0x80c */
};
struct rows {
	short top;
	short base;
	short hit;
};
extern int f_18950(struct player *);
extern void field_clear_flag(struct field *);
extern int f_18fb4(struct field *);
extern int f_18e9c(struct field *);
extern void f_15f10(struct field *, int);
extern void f_209a4(struct field *, int);
extern void f_2e6fc(int);
extern void f_1919e(struct player *);

void f_1a7c8(struct player *p)
{
	struct field *q = p->field;
	struct field *f = q->other;
	struct rows *d = (struct rows *)p->d;
	int x, y, h, k, row;

	if (f_18950(p))
		return;
	switch (p->c[0]) {
	case 0:
		d->hit = 0;
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
		q->b379 = 4;
		f_15f10(q, 4);
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
		d->top = 0;
		for (x = 1; x < 11; x++) {
			for (y = 1, h = 0; y < 21; y++)
				if (q->cells[y * q->width + x].flags & 15)
					h = y;
			if (d->top < h)
				d->top = h;
		}
		d->base = d->top / 2;
		d->top -= d->base;
		d->base++;
		for (x = 1; x < 11; x++) {
			q->cells[21 * q->width + x].flags = 0;
			q->cells[21 * q->width + x].b3 = 0;
		}
		if (d->top <= 0) {
			p->w[0] = 25;
			p->c[0] = 10;
			break;
		}
		p->w[0] = 0;
		p->w[1] = 1;
		p->c[0]++;
		break;
	case 4:
		if (++p->w[0] >= 40)
			p->c[0]++;
		if (p->w[0] <= 10)
			break;
		k = p->w[1];
		for (row = d->base + d->top - 1; row >= d->base; row--) {
			if (k == 1) {
				f_209a4(q, row);
				f_2e6fc(20);
			}
			if (k > 0 && k < 11)
				if (q->cells[row * q->width + k].flags & 15)
				q->cells[row * q->width + k].flags |= 0x800;
			if (k - 3 > 0 && k - 3 < 11)
				if (q->cells[row * q->width + k - 3].flags & 15) {
				q->cells[row * q->width + k - 3].flags = 0;
				q->cells[row * q->width + k - 3].b3 = 0;
			}
			if (row & 1)
				k--;
		}
		p->w[1]++;
		if (k - 3 >= 11)
			p->c[0]++;
		break;
	case 5:
		p->c[0]++;
		for (k = d->top; k; k--)
			for (x = 1; x < 11; x++) {
				row = k + d->base - 1;
				if (row > 0 && row < 11)
					q->cells[row * q->width + x].flags = 0;
				q->cells[row * q->width + x].b3 = 0;
			}
		p->w[0] = 40;
		break;
	case 6:
		if (--p->w[0] == 0)
			p->c[0]++;
		break;
	case 7:
	case 8:
	case 9:
	default:
		q->flags308 &= 0xbfffffff;
		q->u35c.w &= 0x7fff;
		d->hit = 0;
		q->b379 = 0;
		f_15f10(q, 0);
		f_1919e(p);
		break;
	case 10:
		if (--p->w[0] == 0) {
			p->c[0]++;
			p->w[0] = 40;
		}
		break;
	case 11:
		f_2e6fc(20);
		p->c[0] = 6;
		break;
	}
}
