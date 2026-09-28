/* rom: 0x1e480 len: 0x134 func: f_1e480 flags: -macsave=1 -optimize=1 -speed */
struct field {
	char pad0[0x2f8];
	struct field *other;       /* 0x2f8 */
	char pad1[0x308 - 0x2fc];
	unsigned long flags308;    /* 0x308 */
	char pad2[0x35c - 0x30c];
	unsigned short w35c;       /* 0x35c */
	char pad3[0x360 - 0x35e];
	unsigned short w360;       /* 0x360 */
	char pad4[0x379 - 0x362];
	unsigned char b379;        /* 0x379 */
	char pad5[0x37c - 0x37a];
	unsigned short w37c;       /* 0x37c */
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
extern void f_20b24(struct field *);
extern void f_2e6fc(int);
extern void f_1919e(struct player *);

void f_1e480(struct player *p)
{
	struct field *q = p->field;
	struct field *f = q->other;

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
		q->b379 = 11;
		f_15f10(f, 11);
		f_20b24(f);
		f_2e6fc(17);
		f->w35c |= 0x8000;
		f->flags308 |= 0x40000000;
		p->w[0] = 99;
		p->c[0]++;
		break;
	case 1:
		if (p->w[0] == 30)
			f->w360 |= 0x400;
		if (--p->w[0] <= 0)
			p->c[0]++;
		break;
	case 2:
		p->c[0]++;
		break;
	case 3:
		f->w37c &= 0xfbff;
		f->w35c &= 0x7fff;
		f->flags308 &= 0xbfffffff;
		q->b379 = 0;
		f_15f10(f, 0);
		f_1919e(p);
		break;
	}
}
