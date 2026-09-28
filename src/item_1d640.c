/* rom: 0x1d640 len: 0x636 func: f_1d640 flags: -macsave=1 -optimize=1 -speed */
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
extern void f_1919e(struct player *);

void f_1d640(struct player *p)
{
	struct field *q = p->field;
	struct field *f = q->other;
	struct mirror *d = (struct mirror *)p->d;
	int x, y, i, j;
	short s;

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
		q->b379 = 18;
		f_15f10(f, 18);
		f_23378(f);
		f_2e6fc(17);
		f->u35c.w |= 0x8000;
		f->flags308 |= 0x40000000;
		d->n2 = 0;
		p->w[0] = 0;
		p->w[1] = 60;
		p->c[0]++;
		break;
	case 1:
		if (--p->w[1] <= 0)
			p->c[0]++;
		break;
	case 2:
		for (y = 1; y < 22; y++)
			for (x = 1; x < 12; x++)
				d->c[(short)y][(char)x] = f->cells[y * q->width + 11 - x];
		d->top = 0;
		for (i = 1; i < 21; i++)
			for (j = 1; j < 11; j++)
				if (f->cells[i * q->width + j].flags & 15)
					d->top = i;
		p->w[0] = 46;
		p->w[1] = 1;
		p->c[0]++;
		break;
	case 3:
		if (!(f->flags308 & 0x40000000))
			f->flags308 |= 0x40000000;
		if (p->w[0] <= 0) {
			p->w[0] = 0;
			p->c[0]++;
			d->n2++;
			break;
		}
		if (p->w[1] < 11) {
			for (y = 1; y < 21; y++)
				f->cells[y * q->width + p->w[1]] = d->c[(short)y][(char)p->w[1]];
			f_20a9c(f, p->w[1]);
			p->w[1]++;
		}
		p->w[0]--;
		break;
	case 4:
		f->flags308 &= 0xbfffffff;
		f->u35c.w &= 0x7fff;
		p->c[0]++;
		break;
	case 5:
		if (!(f->flags308 & 0x40000000))
			f->flags308 |= 0x40000000;
		s = f->u35c.w;
		if ((unsigned char)s == 12)
			p->c[0]++;
		break;
	case 6:
		for (y = 1; y < 22; y++)
			for (x = 1; x < 12; x++)
				d->c[(short)y][(char)x] = f->cells[y * q->width + 11 - x];
		d->top = 0;
		for (i = 1; i < 21; i++)
			for (j = 1; j < 11; j++)
				if (f->cells[i * q->width + j].flags & 15)
					d->top = i;
		if (d->top <= 0) {
			p->c[0] = 99;
			break;
		}
		p->w[0] = 15;
		p->w[1] = 1;
		p->c[0]++;
		break;
	case 7:
		if (p->w[0] <= 0) {
			p->w[0] = 0;
			p->c[0]++;
			d->n2++;
			break;
		}
		if (p->w[1] < 11) {
			for (y = 1; y < 21; y++)
				f->cells[y * q->width + p->w[1]] = d->c[(short)y][(char)p->w[1]];
			f_20a9c(f, p->w[1]);
			p->w[1]++;
		}
		p->w[0]--;
		break;
	case 8:
		if (d->n2 < 3) {
			p->c[0] = 4;
			break;
		}
		p->c[0]++;
		break;
	case 9:
	default:
		if (!f_18d7c(p))
			f->flags308 &= 0xbfffffff;
		f->u35c.w &= 0x7fff;
		d->n1 = 0;
		q->b379 = 0;
		f_15f10(f, 0);
		f_1919e(p);
		break;
	}
}
