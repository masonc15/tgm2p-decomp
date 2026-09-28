/* rom: 0x188ac len: 0x334 func: field_clear_flag flags: -macsave=1 -optimize=1 -speed */
/* field_clear_flag (was src/field_clear.c) and f_18950 are one source file:
 * f_18950 only matches when compiled after field_clear_flag. */
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
	long flags308;             /* 0x308 */
	unsigned short mode;       /* 0x30c */
	unsigned char id;          /* 0x30e */
	char pad3[0x339 - 0x30f];
	unsigned char b339;        /* 0x339 */
	char pad4[0x35d - 0x33a];
	unsigned char b35d;        /* 0x35d */
	char pad5[0x360 - 0x35e];
	unsigned short w360;       /* 0x360 */
	char pad6[0x379 - 0x362];
	unsigned char b379;        /* 0x379 */
	char pad7[0x37e - 0x37a];
	unsigned char b37e;        /* 0x37e */
	char pad8;
	unsigned char b380;        /* 0x380 */
};
extern unsigned char g_6079374[];

void field_clear_flag(struct field *f)
{
	short x, y;

	for (y = 1; y < 21; y++)
		for (x = 1; x < 11; x++)
			f->cells[y * f->width + x].flags &= 0xdfff;
	if (f->w360 & 0x2000) {
		f->w360 &= 0xdfff;
		f->b37e = 0;
	}
	f->b380 = 0;
	g_6079374[f->id] = 0;
}
struct player {
	char b0;
	char b1;                   /* 0x001 */
	char pad[0x80c - 2];
	struct field *field;       /* 0x80c */
	long pad2;
	struct player *next;       /* 0x814 */
};
extern unsigned long g_6064880;
extern unsigned short d_3b41c[], d_3b428[], d_3b444[];
extern void f_15f10(struct field *, int);
extern void f_1919e(struct player *);

int f_18950(struct player *p)
{
	struct field *q = p->field;
	struct field *f = q->other;
	short i, x, y;

	if (g_6064880 & 0x80000000)
		return 1;
	if (q->flags308 & 64 || q->b35d == 13 || q->b339 & 32) {
		for (i = 0; i < 2; i++)
			q->e0[i] = 0;
		for (y = 1; y < 21; y++)
			for (x = 1; x < 11; x++) {
				if (!(q->mode & 16))
					f->cells[y * q->width + x].flags &= 0xbfff;
				f->cells[y * q->width + x].b2 = 1;
			}
		if (q->b379 != 0xff)
			q->b379 = 0;
		f_15f10(f, 0);
		f->w360 &= 0xf7ff;
		f->w360 &= 0xfeff;
		f_1919e(p);
		return 1;
	}
	return 0;
}
