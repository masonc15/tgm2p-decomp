/* rom: 0x1f5b4 len: 0xa64 func: f_1f5b4 flags: -macsave=1 -optimize=1 -speed */
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
	short pos[2];              /* 0x0e0 */
	char pad1[0x2f8 - 0xe4];
	struct field *other;       /* 0x2f8 */
	char pad2[0x308 - 0x2fc];
	unsigned long flags308;    /* 0x308 */
	char pad3[0x35c - 0x30c];
	unsigned short w35c;       /* 0x35c */
	char pad4[0x379 - 0x35e];
	unsigned char b379;        /* 0x379 */
};
struct lift {
	short top;
	short n;
	short start;
	short speed;
	struct cell grid[21][12];
};
struct player {
	char b0;
	char b1;
	short w[3];
	char c[4];
	struct lift d;
	char pad[0x80c - 12 - sizeof(struct lift)];
	struct field *field;       /* 0x80c */
};
extern int f_18950(struct player *);
extern void field_clear_flag(struct field *);
extern int f_18e74(struct field *);
extern void f_15f10(struct field *, int);
extern void f_23558(struct field *);
extern void f_2e6fc(int);
extern void f_1919e(struct player *);

#define ABS(x) ((x) < 0 ? -(x) : (x))
#define GRID(d, y, x) (*(struct cell *)((char *)(d)->grid[y] + (char)((x) * 6)))

void f_1f5b4(struct player *p)
{
	struct field *q = p->field;
	struct field *f = q->other;
	struct lift *d = &p->d;
	short x, y, row;

	if (f_18950(p))
		return;
	switch (p->c[0]) {
	case 0:
		field_clear_flag(q);
		if (f->flags308 & 0x80)
			return;
		if (q->b379)
			return;
		if (f_18e74(f))
			return;
		q->b379 = 9;
		f_15f10(f, 9);
		f_23558(f);
		f_2e6fc(17);
		f->w35c |= 0x8000;
		f->flags308 |= 0x40000000;
		p->w[0] = 60;
		p->c[0]++;
		break;
	case 1:
		if (--p->w[0] == 0)
			p->c[0]++;
		break;
	case 2:
		for (x = 1; x < 11; x++) {
			f->cells[21 * q->width + x].flags = 0;
			f->cells[21 * q->width + x].b3 = 0;
		}
		d->n = 0;
		for (y = 1; y < 21; y++)
			for (x = 1; x < 11; x++) {
				GRID(d, y, x) = f->cells[y * q->width + x];
				if (f->cells[y * q->width + x].flags & 15) {
					d->top = y;
					d->n = y;
				}
			}
		for (y = 1; y < 21; y++)
			for (x = 1; x < 11; x++)
				GRID(d, y, x) = f->cells[y * q->width + x];
		if (d->n <= 0) {
			p->w[0] = 40;
			p->c[0] = 8;
			break;
		}
		p->c[0]++;
		break;
	case 3:
		f->pos[1] += 4;
		if (f->pos[1] > 48) {
			p->c[0]++;
			p->w[0] = 10;
		}
		break;
	case 4:
		if (--p->w[0] == 0)
			p->c[0]++;
		break;
	case 5:
		f->pos[1] -= 32;
		if (f->pos[1] <= 16)
			p->c[0]++;
		break;
	case 6:
		p->w[1] = 0;
		d->start = 1;
		d->speed = d->n / 2 / 7;
		p->c[0]++;
		break;
	case 7:
		if (d->top == 20)
			f->pos[1] = 0;
		if (d->top < 39 && p->w[1] >= d->speed) {
			p->w[1] = 0;
			d->speed = (20 - ABS(d->top - 20)) / 2 / 7;
			for (y = d->start, row = 1; y < d->top + 2; y++) {
				for (x = 1; x < 11; x++) {
					if (y == d->start) {
						f->cells[(20 - ABS(y - 20)) * q->width + x].flags = 0;
						f->cells[(20 - ABS(y - 20)) * q->width + x].b3 = 0;
					} else
						f->cells[(20 - ABS(y - 20)) * q->width + x] = GRID(d, row, x);
				}
				if (y != d->start)
					row++;
			}
			d->start++;
			d->top++;
			if (d->top >= 39) {
				f_2e6fc(25);
				p->w[0] = 40;
				p->c[0]++;
				break;
			}
		}
		p->w[1]++;
		break;
	case 8:
		if (--p->w[0] == 0)
			p->c[0]++;
		break;
	case 9:
	default:
		q->b379 = 0;
		f_15f10(f, 0);
		/* y, not x, here: it keeps case 7's y in a register */
		for (y = 0; y < 2; y++)
			f->pos[y] = 0;
		f->flags308 &= 0xbfffffff;
		f->flags308 &= 0xffff7fff;
		f_1919e(p);
		break;
	}
}
