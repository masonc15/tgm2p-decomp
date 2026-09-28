/* rom: 0x1ade8 len: 0x5ec func: f_1ade8 flags: -macsave=1 -optimize=1 -speed */
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
	char pad5[0x378 - 0x35e];
	unsigned char b378;        /* 0x378 */
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

void f_1ade8(struct player *p)
{
	struct field *q = p->field;
	struct field *f = q->other;
	struct rows *d = (struct rows *)p->d;
	int x, y, k, row;

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
		q->b379 = 5;
		f_15f10(q, 5);
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
		for (y = 1; y < 21; y++)
			for (x = 1; x < 11; x++)
				if (q->cells[y * q->width + x].flags & 15)
					d->top = y;
		d->top -= d->top / 2;
		for (x = 1; x < 11; x++)
			q->cells[21 * q->width + x].flags = 0;
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
		if (++p->w[0] >= 40) {
			p->w[0] = 0;
			p->c[0]++;
			break;
		}
		if (p->w[0] <= 10)
			break;
		k = p->w[1];
		for (row = 1; row <= d->top; row++) {
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
		if (k - 3 >= 11) {
			p->c[0]++;
			p->w[0] = 25;
		}
		break;
	case 5:
		if (--p->w[0] == 0)
			p->c[0]++;
		break;
	case 6:
		for (y = d->top; y; y--)
			for (x = 1; x < 11; x++)
				q->cells[y * q->width + x].flags = 0;
		f_2e6fc(25);
		for (y = 1; y < 21 - d->top; y++)
			for (x = 1; x < 11; x++)
				q->cells[y * q->width + x] = q->cells[(d->top + y) * q->width + x];
		for (y = 21 - d->top; y < 21; y++)
			for (x = 1; x < 11; x++) {
				q->cells[y * q->width + x].flags = 0;
				q->cells[y * q->width + x].b3 = 0;
			}
		p->w[0] = 40;
		p->c[0]++;
		break;
	case 7:
		if (--p->w[0] == 0)
			p->c[0]++;
		break;
	case 8:
		q->flags308 &= 0xbfffffff;
		q->u35c.w &= 0x7fff;
		d->hit = 0;
		q->b379 = 0;
		q->b378 = 0;
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
		p->c[0] = 7;
		break;
	}
}
