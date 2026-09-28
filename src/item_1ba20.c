/* rom: 0x1ba20 len: 0x7e0 func: f_1ba20 flags: -macsave=1 -optimize=1 -speed */
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
extern int f_18950(struct player *);
extern void field_clear_flag(struct field *);
extern int f_18fb4(struct field *);
extern int f_18e9c(struct field *);
extern void f_15f10(struct field *, int);
extern void f_209a4(struct field *, int);
extern void f_2e6fc(int);
extern void f_1919e(struct player *);

struct rows {
	short row;
	short hit;
};

void f_1ba20(struct player *p)
{
	struct field *q = p->field;
	struct field *f = q->other;
	struct rows *d = (struct rows *)p->d;
	short x, i, j, row, empty;
	int k;

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
		q->b379 = 8;
		q->u35c.w |= 0x8000;
		f_15f10(q, 8);
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
			q->u35c.w |= 0x8000;
			p->c[0]++;
		}
		break;
	case 3:
		d->row = 21;
		empty = 1;
		do {
			d->row--;
			if (d->row == 1)
				break;
			for (x = 1; x < 11; x++)
				if (q->cells[d->row * q->width + x].flags != 0)
					empty = 0;
		} while (empty);
		for (x = 1; x < 11; x++) {
			q->cells[21 * q->width + x].flags = 0;
			q->cells[21 * q->width + x].b3 = 0;
		}
		if (d->row <= 1) {
			p->w[0] = 25;
			p->c[0] = 10;
			break;
		}
		p->w[0] = 0;
		p->c[0]++;
		break;
	case 4:
		p->w[0]++;
		row = d->row;
		k = p->w[0];
		if (k % 8 == 0 && k > 10) {
			f_2e6fc(13);
			f_209a4(q, row);
			for (x = 1; x < 11; x++) {
				if (q->cells[row * q->width + x].flags != 0)
					q->cells[row * q->width + x].flags = 0;
				q->cells[row * q->width + x].b3 = 0;
			}
			row -= 2;
			if (row < 1) {
				p->c[0]++;
				p->w[0] = 25;
			}
			d->row = row;
		}
		break;
	case 5:
		if (--p->w[0] == 0) {
			p->c[0]++;
			f_2e6fc(25);
		}
		break;
	case 6:
		p->c[0]++;
		i = 1;
		k = d->row;
		if (k == 0) {
			j = 2;
			while (1) {
				for (x = 1; x < 11; x++) {
					q->cells[j * q->width + x] = q->cells[(j + i) * q->width + x];
					q->cells[(j + i) * q->width + x].flags = 0;
					q->cells[(j + i) * q->width + x].b3 = 0;
				}
				i++;
				j++;
				if (i == 10)
					break;
			}
		} else {
			for (j = 1; i != 11; i++, j++)
				for (x = 1; x < 11; x++) {
					q->cells[j * q->width + x] = q->cells[(j + i) * q->width + x];
					q->cells[(j + i) * q->width + x].flags = 0;
					q->cells[(j + i) * q->width + x].b3 = 0;
				}
		}
		p->w[0] = 40;
		break;
	case 7:
		if (--p->w[0] == 0)
			p->c[0]++;
		break;
	case 8:
	case 9:
	default:
		q->flags308 &= 0xbfffffff;
		q->u35c.w &= 0x7fff;
		q->b379 = 0;
		f_15f10(q, 0);
		d->hit = 0;
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
