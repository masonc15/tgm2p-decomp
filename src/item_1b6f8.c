/* rom: 0x1b6f8 len: 0x328 func: f_1b6f8 flags: -macsave=1 -optimize=1 -speed */
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
extern int f_18fb4(struct field *);
extern int f_18e9c(struct field *);
extern void f_15f10(struct field *, int);
extern void f_209a4(struct field *, int);
extern void f_2e6fc(int);
extern void f_1919e(struct player *);

void f_1b6f8(struct player *p)
{
	struct field *q = p->field;
	struct field *f = q->other;
	short *hit = (short *)p->d;
	int x, y, k;

	if (f_18950(p))
		return;
	switch (p->c[0]) {
	case 0:
		*hit = 0;
		p->c[0]++;
	case 1:
		field_clear_flag(q);
		if (q->flags308 & 0x80)
			return;
		if (q->b379)
			return;
		if (f_18fb4(f))
			*hit = 1;
		if (f_18e9c(f))
			return;
		q->b379 = 7;
		f_15f10(q, 7);
		q->u35c.w |= 0x8000;
		if (*hit)
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
		p->c[0]++;
		p->w[0] = 0;
		break;
	case 4:
		q->e0 += 4;
		if (q->e0 > 48) {
			p->c[0]++;
			p->w[0] = 10;
		}
		break;
	case 5:
		if (--p->w[0] == 0)
			p->c[0]++;
		break;
	case 6:
		q->e0 -= 32;
		if (q->e0 <= -16)
			p->c[0]++;
		break;
	case 7:
		for (y = 1; y < 22; y++)
			for (x = 1; x < 11; x++)
				if (q->cells[y * q->width + x].flags == 0) {
					for (k = x + 1; ; k++) {
						if (k >= 11) {
							x = 12;
							break;
						}
						if (q->cells[y * q->width + k].flags != 0) {
							q->cells[y * q->width + x] = q->cells[y * q->width + k];
							q->cells[y * q->width + k].flags = 0;
							q->cells[y * q->width + k].b3 = 0;
							break;
						}
					}
				}
		f_2e6fc(25);
		p->c[0]++;
		break;
	case 8:
		q->e0 += 2;
		if (q->e0 >= 0) {
			q->e0 = 0;
			p->w[0] = 0;
			p->c[0]++;
		}
		break;
	case 9:
		if (--p->w[0] <= 0) {
			p->c[0]++;
			p->w[0] = 40;
		}
		break;
	case 10:
		if (--p->w[0] == 0)
			p->c[0]++;
		break;
	case 11:
		q->u35c.w &= 0x7fff;
		q->b379 = 0;
		f_15f10(q, 0);
		*hit = 0;
		q->e0 = 0;
		for (k = 0; k < 3; k++)
			p->w[k] = 0;
		q->flags308 &= 0xbfffffff;
		f_1919e(p);
		break;
	}
}
