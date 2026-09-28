/* rom: 0x1d1bc len: 0x484 func: f_1d1bc flags: -macsave=1 -optimize=1 -speed */
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
extern int f_18950(struct player *);
extern void field_clear_flag(struct field *);
extern int f_18e74(struct field *);
extern int f_18e9c(struct field *);
extern void f_15f10(struct field *, int);
extern void f_23288(struct field *);
extern void f_2e6fc(int);
extern void f_20a9c(struct field *, int);
extern void f_1919e(struct player *);

void f_1d1bc(struct player *p)
{
	struct field *q = p->field;
	struct field *f = q->other;
	long *t = (long *)p->d;
	short x, y, v;

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
		if (f_18e9c(f))
			return;
		q->b379 = 17;
		f_15f10(q, 17);
		f_15f10(f, 17);
		f_23288(f);
		f_2e6fc(17);
		q->u35c.w |= 0x8000;
		f->u35c.w |= 0x8000;
		q->flags308 |= 0x40000000;
		f->flags308 |= 0x40000000;
		*t = 0;
		p->w[0] = 60;
		p->w[1] = 1;
		p->c[0]++;
		break;
	case 1:
		if (--p->w[0] == 0) {
			p->c[0]++;
			f_2e6fc(20);
		}
		break;
	case 2:
		if ((*t)++ & 1) {
			f_20a9c(f, p->w[1]);
			for (y = 1; y < 21; y++) {
				v = f->cells[y * q->width + p->w[1]].flags;
				if (!(v & 0x2000))
					f->cells[y * q->width + p->w[1]] = q->cells[y * q->width + p->w[1]];
				q->cells[y * q->width + p->w[1]].flags = v & 0xdfff;
			}
			p->w[1]++;
		}
		for (x = 1; x < 11; x++) {
			f->cells[21 * q->width + x].flags = 0;
			f->cells[21 * q->width + x].b3 = 0;
			q->cells[21 * q->width + x].flags = 0;
			q->cells[21 * q->width + x].b3 = 0;
		}
		if (p->w[1] != 11)
			break;
		p->w[0] = 40;
		p->c[0]++;
		break;
	case 3:
		if (--p->w[0] == 0) {
			q->flags308 &= 0xbfffffff;
			f->flags308 &= 0xbfffffff;
			f->u35c.w &= 0x7fff;
			q->b379 = 0;
			f_15f10(q, 0);
			f_15f10(f, 0);
			f_1919e(p);
		}
		break;
	}
}
