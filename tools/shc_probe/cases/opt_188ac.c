/* rom: 0x188ac len: 0xa4 func: field_clear_flag flags: -macsave=1 -optimize=1 -speed */
/* Clears bit 13 of every cell in the 10x20 playfield. */
struct cell {
	unsigned short flags;
	short a;
	short b;
};
struct field {
	struct cell *cells;        /* 0x000 */
	char pad0[0xdf - 4];
	unsigned char width;       /* 0x0df */
	char pad1[0x30e - 0xe0];
	unsigned char b30e;        /* 0x30e */
	char pad2[0x360 - 0x30f];
	unsigned short w360;       /* 0x360 */
	char pad3[0x37e - 0x362];
	unsigned char b37e;        /* 0x37e */
	char pad4;
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
	g_6079374[f->b30e] = 0;
}
