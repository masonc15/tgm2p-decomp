/* rom: 0x8518 len: 0xc0 func: field_check_flag flags: -macsave=1 -optimize=1 -speed */
struct cell {
	unsigned short flags;
	char b2;
	char b3;
	short w4;
};
struct field {
	struct cell *cells;        /* 0x000 */
	char pad0[0xde - 4];
	unsigned char height;      /* 0x0de */
	unsigned char width;       /* 0x0df */
	char pad1[0x30e - 0xe0];
	unsigned char b30e;        /* 0x30e */
	char pad2[0x398 - 0x30f];
	unsigned char b398;        /* 0x398 */
};
extern char g_6079374[];

void field_check_flag(struct field *f)
{
	short x, y;
	struct cell *c;
	unsigned short mask = 0x2000;
	char *tbl = g_6079374;

	for (y = 1; y < f->height; y++) {
		c = &f->cells[y * f->width + 1];
		for (x = 1; x < f->width - 1; x++, c++)
			if ((c->flags & mask) &&
			    f->cells[y * f->width + x].b3 == tbl[f->b30e])
				return;
	}
	tbl[f->b30e] = 0;
}

void field_inc_398(struct field *f)
{
	f->b398++;
}
