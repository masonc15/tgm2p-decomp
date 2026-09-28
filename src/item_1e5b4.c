/* rom: 0x1e5b4 len: 0x568 func: f_1e5b4 flags: -macsave=1 -optimize=1 -speed */
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
	short e0[2];               /* 0x0e0 */
	char pad1[0x2f8 - 0xe4];
	struct field *other;       /* 0x2f8 */
	char pad2[0x308 - 0x2fc];
	unsigned long flags308;    /* 0x308 */
	unsigned short mode;       /* 0x30c */
	unsigned char id;          /* 0x30e */
	char pad3[0x35c - 0x30f];
	union {
		unsigned short w;
		unsigned char b[2];
	} u35c;                    /* 0x35c */
	unsigned short w35e;       /* 0x35e */
	char pad4[0x362 - 0x360];
	unsigned char b362;        /* 0x362 */
	char pad5;
	short w364;                /* 0x364 */
	char pad6[0x379 - 0x366];
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
struct drop {
	short lvl;
	short base;
	short speed;
	unsigned char up[2];
	unsigned char hits[2];
	unsigned char n;
	unsigned char col[8];      /* 0x00b */
};
extern unsigned char g_606475e[];
extern short d_3b4ac[];
extern int f_18950(struct player *);
extern void field_clear_flag(struct field *);
extern int f_18e74(struct field *);
extern void f_15f10(struct field *, int);
extern void f_2e6fc(int);
extern void f_2090c(struct field *, int, int, int);
extern void f_207d0(struct field *, int, int);
/* defined with short y, x in effect_176e0.c; this caller only matches
   with int parameters (no separate short copy of k - i) */
extern void f_176e0(struct field *, int, int);
extern void f_1919e(struct player *);
short f_1ead4(struct field *f);

void f_1e5b4(struct player *p)
{
	struct field *q = p->field;
	struct field *f = q->other;
	struct drop *d = (struct drop *)p->d;
	short i, j, n, k, x;

	if (f_18950(p))
		return;
	if (d->lvl < 1)
		d->lvl = 1;
	if (d->lvl > 10)
		d->lvl = 10;
	switch (p->c[0]) {
	case 0:
		field_clear_flag(q);
		if (f->flags308 & 0x80)
			return;
		if (q->b379)
			return;
		if (f_18e74(f))
			return;
		q->b379 = 12;
		f_15f10(f, 12);
		f->u35c.w |= 0x8000;
		f->flags308 |= 0x40000000;
		d->lvl = f_1ead4(f);
		if (d->lvl < 0 || d->lvl > 11)
			d->lvl = 5;
		d->base = d->lvl * 14 - 70;
		d->speed = 80;
		d->up[0] = d->up[1] = 0;
		d->hits[0] = d->hits[1] = 0;
		p->w[0] = 20;
		p->c[0]++;
		break;
	case 1:
		if (--p->w[0] == 0) {
			p->w[0] = 30;
			p->w[1] = 0;
			p->c[0]++;
		}
		break;
	case 2:
		if (d->speed > 0) {
			if (g_606475e[f->id] & 32) {
				d->up[0]++;
				if (d->up[0] % 3 == 0)
					d->lvl++;
				if (d->lvl < 1)
					d->lvl = 1;
				if (d->lvl > 10)
					d->lvl = 10;
				d->base = d->lvl * 14 - 70;
			}
			if (g_606475e[f->id] & 16) {
				d->up[1]++;
				if (d->up[1] % 3 == 0)
					d->lvl--;
				if (d->lvl < 1)
					d->lvl = 1;
				if (d->lvl > 10)
					d->lvl = 10;
				d->base = d->lvl * 14 - 70;
			}
			if (g_606475e[f->id] & 14)
				d->hits[0]++;
			if ((g_606475e[q->id] & 14) && !(q->mode & 0x200))
				d->hits[1]++;
		}
		if (d->hits[0] / 10 - d->hits[1] / 15 <= 0)
			d->n = 1;
		else
			d->n = d->hits[0] / 10 - d->hits[1] / 15 + 1;
		i = 0;
		j = 0;
		if (d->n > 0) {
			do {
				x = d_3b4ac[i] + d->lvl;
				if (x > 0 && x < 11) {
					f_2090c(f, 20, x, d->speed / 5);
					d->col[j] = x;
					i++;
					j++;
				} else
					i++;
			} while (j < d->n);
		}
		if (d->speed <= 0) {
			if (--p->w[0] == 0) {
				p->w[0] = 0;
				p->w[1] = 21;
				p->c[0]++;
			}
			break;
		}
		if (++p->w[1] % 4)
			break;
		if ((d->speed -= 5) < 0)
			d->speed = 0;
		f_2e6fc(30);
		break;
	case 3:
		for (n = 0; n < d->n; n++) {
			f_2090c(f, 20, d->col[n], 0);
			if (p->w[0] == 0)
				f_207d0(f, 20, d->col[n]);
		}
		if (p->w[0] == 0)
			f_2e6fc(21);
		if (++p->w[0] >= 19) {
			p->w[0] = 40;
			p->c[0]++;
			break;
		}
		k = p->w[1];
		p->w[1] -= 2;
		for (n = 0; n < d->n; n++)
			for (i = 0; i < 2; i++)
				if (f->cells[(k - i) * q->width + d->col[n]].flags != 0)
					if (k - i > 0 && k - i < 21) {
						f_176e0(f, k - i, d->col[n]);
						f->cells[(k - i) * q->width + d->col[n]].flags = 0;
						f->cells[(k - i) * q->width + d->col[n]].b3 = 0;
						f_2e6fc(19);
					}
		break;
	case 4:
		if (--p->w[0] <= 0)
			p->c[0]++;
		break;
	case 5:
	default:
		for (i = 0; i < 2; i++)
			f->e0[i] = 0;
		f->flags308 &= 0xbfffffff;
		f->u35c.w &= 0x7fff;
		q->b379 = 0;
		f_15f10(f, 0);
		f_1919e(p);
		break;
	}
}

short f_1ead4(struct field *f)
{
	if ((f->w35e & 15) == 2 && (f->b362 == 1 || f->b362 == 3))
		return f->w364 + 1;
	return f->w364;
}
