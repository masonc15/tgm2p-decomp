/* rom: 0x1ed78 len: 0x204 func: f_1ed78 flags: -macsave=1 -optimize=1 -speed */
struct field {
	char pad0[0x2f8];
	struct field *other;       /* 0x2f8 */
	char pad1[0x308 - 0x2fc];
	unsigned long flags308;    /* 0x308 */
	char pad2[0x326 - 0x30c];
	unsigned short w326;       /* 0x326 */
	char pad3[0x35c - 0x328];
	union {
		unsigned short w;
		unsigned char b[2];
	} u35c;                    /* 0x35c */
	char pad4[0x360 - 0x35e];
	unsigned short w360;       /* 0x360 */
	char pad5[0x379 - 0x362];
	unsigned char b379;        /* 0x379 */
	char pad6[0x37c - 0x37a];
	unsigned short w37c;       /* 0x37c */
	char pad7[0x388 - 0x37e];
	unsigned char b388;        /* 0x388 */
};
struct rate {
	short last;
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
extern int f_18d7c(struct player *);
extern void f_15f10(struct field *, int);
extern void f_205f0(struct field *);
extern void f_2e6fc(int);
extern void f_1919e(struct player *);

void f_1ed78(struct player *p)
{
	struct field *q = p->field;
	struct field *f = q->other;
	struct rate *d = (struct rate *)p->d;

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
		q->b379 = 14;
		f_15f10(f, 14);
		f->u35c.w |= 0x8000;
		f->flags308 |= 0x40000000;
		f_205f0(f);
		f_2e6fc(17);
		p->w[2] = 100;
		p->w[0] = 0;
		p->c[0]++;
		break;
	case 1:
		if (--p->w[2] == 0) {
			f->w360 |= 0x100;
			p->c[0]++;
		}
		break;
	case 2:
		f->u35c.w &= 0x7fff;
		f->flags308 &= 0xbfffffff;
		p->w[0]++;
		d->last = f->w326;
		p->c[0]++;
		break;
	case 3:
		if (!(f->flags308 & 0x40000000))
			f->flags308 |= 0x40000000;
		if (p->w[0] <= 2 && (f->w360 & 0xf00) != 0x100)
			f->w360 |= 0x100;
		if (f->u35c.b[1] == 12 && d->last != f->w326) {
			if (f->b388) {
				f->flags308 &= 0xbfffffff;
				break;
			}
			if (p->w[0] >= 3) {
				p->c[0]++;
				break;
			}
			p->c[0] = 2;
		} else
			f->flags308 &= 0xbfffffff;
		break;
	case 4:
		f->u35c.w &= 0x7fff;
		if (!f_18d7c(p))
			f->flags308 &= 0xbfffffff;
		q->b379 = 0;
		f_15f10(f, 0);
		f_1919e(p);
		break;
	}
}
