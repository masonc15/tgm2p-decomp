/* rom: 0x1dc78 len: 0x808 func: f_1dc78 flags: -macsave=1 -optimize=1 -speed */
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
	short e0;                  /* 0x0e0 */
	char pad1[0x2f8 - 0xe2];
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
struct mirror {
	short top;
	short n1;
	short n2;
	struct cell c[22][12];     /* 0x006 */
};
extern int f_18950(struct player *);
extern void field_clear_flag(struct field *);
extern int f_18e74(struct field *);
extern void f_15f10(struct field *, int);
extern void f_23378(struct field *);
extern void f_2e6fc(int);
extern void f_20a9c(struct field *, int);
extern int f_18d7c(struct player *);
extern void f_23198(struct field *);
extern void f_1919e(struct player *);


struct shake {
	short s0;
	short step;
};

void f_1dc78(struct player *p)
{
	struct field *q = p->field;
	struct field *f = q->other;
	struct shake *d = (struct shake *)p->d;
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
		q->b379 = 19;
		for (y = 0; y < 22; y++)
			for (x = 0; x < 12; x++)
				f->cells[y * q->width + x].b2 = 5;
		f_15f10(f, 19);
		f_23198(f);
		f_2e6fc(17);
		f->u35c.w |= 0x8000;
		f->flags308 |= 0x40000000;
		p->w[0] = 0;
		p->w[1] = 100;
		p->c[0]++;
		break;
	case 1:
		if (--p->w[1] < 0) {
			f->flags308 &= 0xbfffffff;
			p->c[0]++;
		}
		break;
	case 2:
		for (x = 1; x < 11; x++)
			f->cells[21 * q->width + x].flags = 0;
		p->w[0] = 490;
		p->w[1] = 0;
		p->w[2] = 0;
		d->step = 12;
		p->c[0]++;
		break;
	case 3:
		if (--p->w[0] <= 0) {
			p->c[0]++;
			break;
		}
		if (p->w[0] > 420 && p->w[1] >= d->step) {
			p->w[1] = 0;
			p->w[2]++;
			if (p->w[2] > 1) {
				p->w[2] = 0;
				if (d->step > 3)
					d->step -= 3;
			}
			for (y = 0; y < 22; y++)
				for (x = 0; x < 12; x++)
					if (f->cells[y * q->width + x].flags & 15) {
						if (f->cells[y * q->width + x].flags & 0x4000)
							f->cells[y * q->width + x].flags &= 0xbfff;
						else
							f->cells[y * q->width + x].flags |= 0x4000;
					}
		}
		if (p->w[0] <= 420) {
			for (y = 1; y < 21; y++)
				for (x = 1; x < 11; x++)
					if (f->cells[y * q->width + x].flags & 15)
						f->cells[y * q->width + x].flags |= 0x4000;
		}
		p->w[1]++;
		break;
	case 4:
		for (y = 1; y < 21; y++)
			for (x = 1; x < 11; x++) {
				if (!(q->mode & 16))
					f->cells[y * q->width + x].flags &= 0xbfff;
				f->cells[y * q->width + x].b2 = 5;
			}
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
