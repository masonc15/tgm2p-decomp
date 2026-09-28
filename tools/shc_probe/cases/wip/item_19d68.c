/* rom: 0x19d68 len: 0xa60 func: f_19d68 flags: -macsave=1 -optimize=1 -speed */
/* 99.2%: only the else branch in state 4 (n < 3) differs; it swaps r1/r2 in its
 * three read-modify-writes. */
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
	char pad3[0x326 - 0x30e];
	unsigned short w326;       /* 0x326 */
	char pad4[0x35c - 0x328];
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
struct table {
	short t[22][12];
	short n;                   /* 0x210 */
	short last;                /* 0x212 */
	long done;                 /* 0x214 */
};
extern int f_18950(struct player *);
extern void field_clear_flag(struct field *);
extern int f_18e74(struct field *);
extern void f_15f10(struct field *, int);
extern void f_230a8(struct field *);
extern void f_2e6fc(int);
extern int f_18d7c(struct player *);
extern void f_1919e(struct player *);

void f_19d68(struct player *p)
{
	struct field *q = p->field;
	struct field *f = q->other;
	struct table *tb = (struct table *)p->d;
	short x, y;
	short v;

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
		q->b379 = 3;
		f_15f10(f, 3);
		f_230a8(f);
		f_2e6fc(17);
		f->u35c.w |= 0x8000;
		f->flags308 |= 0x40000000;
		for (y = 0; y < 22; y++)
			for (x = 0; x < 12; x++)
				f->cells[y * q->width + x].b2 = 5;
		p->w[0] = 98;
		p->c[0]++;
		break;
	case 1:
		if (--p->w[0] == 0)
			p->c[0]++;
		break;
	case 2:
		v = q->w326 % 3;
		switch (v) {
		case 0:
			for (x = 1; x < 11; x++)
				for (y = 1; y < 10; y++) {
					tb->t[y][x] = y * x % 10;
					tb->t[y + 9][x] = y * x % 10;
					if (y + 18 <= 20)
						tb->t[y + 18][x] = y * x % 10;
				}
			break;
		case 1:
			for (x = 1; x < 6; x++)
				for (y = 1; y < 21; y++) {
					tb->t[y][x] = (y - x < 0 ? x - y : y - x) % 10;
					tb->t[y][11 - x] = (y - x < 0 ? x - y : y - x) % 10;
				}
			break;
		case 2:
		default:
			for (x = 1; x < 6; x++)
				for (y = 1; y < 21; y++) {
					tb->t[y][x] = (y + x) % 10;
					tb->t[y][11 - x] = (y + x) % 10;
				}
			break;
		}
		p->w[0] = -10;
		p->w[1] = 0;
		tb->n = 0;
		tb->done = 0;
		p->c[0]++;
		break;
	case 3:
		f->u35c.w &= 0x7fff;
		f->flags308 &= 0xbfffffff;
		tb->n++;
		tb->last = f->w326;
		p->c[0]++;
		break;
	case 4:
		if (tb->done) {
			p->c[0]++;
			break;
		}
		if (!(f->flags308 & 0x40000000))
			f->flags308 |= 0x40000000;
		if (p->w[0] > 0) {
			for (x = 1; x < 11; x++)
				for (y = 1; y < 21; y++) {
					f->cells[y * q->width + x].flags &= 0xbfff;
					v = (tb->t[y][x] + p->w[1]) % 10;
					f->cells[y * q->width + x].b2 = v;
					if (v == 0)
						f->cells[y * q->width + x].flags |= 0x4000;
				}
		}
		if (p->w[0] % 3 == 0)
			p->w[1]++;
		p->w[0]++;
		if (f->u35c.b[1] == 12 && tb->last != f->w326) {
			if (p->w[0] > 600) {
				tb->done = 1;
				break;
			}
			if (tb->n >= 3) {
				tb->done = 1;
				break;
			}
			f->u35c.w &= 0x7fff;
			f->flags308 &= 0xbfffffff;
			tb->n++;
			break;
		}
		f->flags308 &= 0xbfffffff;
		break;
	case 5:
		for (y = 0; y < 22; y++)
			for (x = 0; x < 12; x++) {
				f->cells[y * q->width + x].b2 = 5;
				if (!(q->mode & 16))
					f->cells[y * q->width + x].flags &= 0xbfff;
			}
		p->c[0]++;
		break;
	case 6:
	default:
		f->u35c.w &= 0x7fff;
		if (!f_18d7c(p))
			f->flags308 &= 0xbfffffff;
		f_1919e(p);
		q->b379 = 0;
		f_15f10(f, 0);
		break;
	}
}
