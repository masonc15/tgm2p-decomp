/* rom: 0x1ef7c len: 0x638 func: f_1ef7c flags: -macsave=1 -optimize=1 -speed */
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
	unsigned short w35c;       /* 0x35c */
	char pad4[0x379 - 0x35e];
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
extern int f_18e74(struct field *);
extern void f_15f10(struct field *, int);
extern void f_23738(struct field *);
extern void f_2e6fc(int);
extern void f_1919e(struct player *);

void f_1ef7c(struct player *p)
{
	struct field *q = p->field;
	struct field *f = q->other;
	short x, y;

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
		q->b379 = 16;
		f_15f10(f, 16);
		f_23738(f);
		f_2e6fc(17);
		f->w35c |= 0x8000;
		f->flags308 |= 0x40000000;
		p->w[0] = 100;
		p->c[0]++;
		break;
	case 1:
		if (--p->w[0] < 0) {
			f->flags308 &= 0xbfffffff;
			p->c[0]++;
		}
		break;
	case 2:
		for (x = 1; x < 11; x++)
			f->cells[21 * q->width + x].flags = 0;
		p->w[0] = 300;
		p->w[1] = 0;
		p->c[0]++;
		break;
	case 3:
		if (--p->w[0] <= 0) {
			p->c[0]++;
			break;
		}
		for (y = 1; y < 21; y++)
			for (x = 1; x < 11; x++)
				if (f->cells[y * q->width + x].flags & 15) {
					if (p->w[1] % 60 != x)
						f->cells[y * q->width + x].flags |= 0x4000;
					else
						f->cells[y * q->width + x].flags &= 0xbfff;
				}
		if (p->w[1] % 60 == 0)
			f_2e6fc(18);
		p->w[1]++;
		break;
	case 4:
		for (y = 1; y < 21; y++)
			for (x = 1; x < 11; x++)
				if (f->cells[y * q->width + x].flags & 15)
					if (!(q->mode & 16))
						f->cells[y * q->width + x].flags &= 0xbfff;
		p->c[0]++;
		break;
	case 5:
	default:
		q->b379 = 0;
		f_15f10(f, 0);
		f_1919e(p);
		break;
	}
}
