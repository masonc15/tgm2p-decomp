/* rom: 0x18be0 len: 0x40c func: f_18be0 flags: -macsave=1 -optimize=1 -speed */
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
struct player {
	char b0;
	char b1;                   /* 0x001 */
	char pad[0x80c - 2];
	struct field *field;       /* 0x80c */
	long pad2;
	struct player *next;       /* 0x814 */
};
extern unsigned short d_3b41c[], d_3b428[], d_3b444[];

int f_18be0(struct field *f)
{
	short x, y, yy;
	int moved = 0;

	for (y = 1; y < 22; y++)
		for (x = 1; x < 11; x++)
			if (f->cells[y * f->width + x].flags == 0) {
				yy = y + 1;
				while (1) {
					if (yy >= 22)
						break;
					if (f->cells[yy * f->width + x].flags != 0) {
						f->cells[y * f->width + x].flags = f->cells[yy * f->width + x].flags;
						f->cells[y * f->width + x].b3 = f->cells[yy * f->width + x].b3;
						f->cells[yy * f->width + x].flags = 0;
						f->cells[yy * f->width + x].b3 = 0;
						moved = 1;
						break;
					}
					yy++;
				}
			}
	return moved;
}

int f_18d04(struct field *f)
{
	short x, y;

	for (y = 0; y < 22; y++) {
		x = 0;
		if (f->cells[y * f->width + x].flags != 1)
			return 1;
		x = 11;
		if (f->cells[y * f->width + x].flags != 1)
			return 1;
	}
	y = 0;
	for (x = 0; x < 12; x++)
		if (f->cells[y * f->width + x].flags != 1)
			return 1;
	return 0;
}

char f_18ed4(unsigned short v);

int f_18d7c(struct player *p)
{
	struct player *t, *n;
	struct field *q;
	char a[2], b[2];
	char i;
	int r;
	unsigned char id;
	char c;

	a[0] = a[1] = 0;
	b[0] = b[1] = 0;
	t = p;
	if ((r = f_18ed4(t->b1)) == 0)
		return;
	q = t->field;
	a[q->id]++;
	b[q->id] = r;
	n = p->next;
	for (i = 0; i < 18; i++) {
		if (n == 0)
			break;
		t = n;
		n = t->next;
		r = f_18ed4(t->b1);
		if (r != 0) {
			q = t->field;
			id = q->id;
			a[id]++;
			b[q->id] = r;
			if (a[0] == 1 && a[1] == 1 && b[0] == b[1])
				continue;
			if (a[q->id] == 1 && q->b379 == t->b1)
				continue;
			c = t->b1;
			if (c)
				return 1;
		}
	}
	return 0;
}

int f_18e74(struct field *f)
{
	short i;

	for (i = 0; i < 6; i++)
		if (d_3b41c[i] == f->b379)
			return 1;
	return 0;
}

int f_18e9c(struct field *f)
{
	short i;

	for (i = 0; i < 14; i++)
		if (d_3b428[i] == f->b379)
			return 1;
	return 0;
}

char f_18ed4(unsigned short v)
{
	short i;
	char c1 = 0, c2 = 0;

	for (i = 0; i < 6; i++)
		if (d_3b41c[i] == v)
			c1 = 1;
	for (i = 0; i < 14; i++)
		if (d_3b428[i] == v)
			c2 = 2;
	return c1 + c2;
}

int f_18fb4(struct field *f)
{
	short i;

	for (i = 0; i < 3; i++)
		if (d_3b444[i] == f->b379)
			return 1;
	return 0;
}
