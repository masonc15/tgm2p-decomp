/* rom: 0x1c200 len: 0x508 func: f_1c200 flags: -macsave=1 -optimize=1 -speed */
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
struct save {
	short flags[22][12];
	short col[22];             /* 0x210 */
	short n;                   /* 0x23c */
};
extern int f_18950(struct player *);
extern void field_clear_flag(struct field *);
extern int f_18e74(struct field *);
extern void f_15f10(struct field *, int);
extern void f_23648(struct field *);
extern void f_2e6fc(int);
extern int f_2beca(int);
extern void f_176e0(struct field *, int, int);
extern void f_1919e(struct player *);

void f_1c200(struct player *p)
{
	struct field *q = p->field;
	struct field *f = q->other;
	struct save *d = (struct save *)p->d;
	short x, y, i, j, v;

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
		q->b379 = 10;
		f_15f10(f, 10);
		f_23648(f);
		f_2e6fc(17);
		f->u35c.w |= 0x8000;
		f->flags308 |= 0x40000000;
		p->w[0] = 0;
		p->w[1] = 60;
		p->c[0]++;
		break;
	case 1:
		if (--p->w[1] == 0) {
			d->n = 0;
			p->c[0]++;
		}
		break;
	case 2:
		for (y = 0; y < 22; y++)
			for (x = 0; x < 12; x++)
				d->flags[y][x] = f->cells[y * q->width + x].flags;
		for (y = 0; y < 22; y++)
			d->col[y] = 0;
		for (i = 1; i < 21; i++) {
			while ((x = (f_2beca(64) & 15) + 1) >= 11)
				;
			if (f->cells[i * q->width + x].flags != 0) {
				d->col[i] = x;
				d->n++;
			}
		}
		p->c[0]++;
		break;
	case 3:
		for (j = 1; j < 21; j++) {
			if (!d->col[j])
				continue;
			f->cells[j * q->width + d->col[j]].flags = (p->w[0] & 1) ? 3 : 7;
		}
		if (++p->w[0] < 30)
			break;
		for (y = 0; y < 22; y++)
			for (x = 0; x < 12; x++)
				f->cells[y * q->width + x].flags = d->flags[y][x];
		p->w[0] = 0;
		f_2e6fc(32);
		p->c[0]++;
		break;
	case 4:
		if (++p->w[0] >= 21) {
			p->w[0] = 40;
			p->c[0]++;
			f->e2 = 0;
			break;
		}
		f->e2 = (p->w[0] & 2) ? -3 : 3;
		v = d->col[p->w[0]];
		if (v) {
			for (i = 1; i < 21; i++)
				if (d->col[i])
					f_176e0(f, i, d->col[i]);
			f->cells[p->w[0] * q->width + v].flags = 0;
			f->cells[p->w[0] * q->width + v].b3 = 0;
			if (p->w[1] == 0) {
				f_2e6fc(19);
				p->w[1]++;
			}
		}
		if (p->w[1]) {
			if (++p->w[1] > 5)
				p->w[1] = 0;
		}
		break;
	case 5:
		if (--p->w[0] == 0)
			p->c[0]++;
		break;
	case 6:
		f->flags308 &= 0xbfffffff;
		f->u35c.w &= 0x7fff;
		q->b379 = 0;
		f_15f10(f, 0);
		f_1919e(p);
		break;
	}
}
