/* rom: 0x50e0 len: 0x114 func: f_50e0 flags: -macsave=1 -optimize=1 -speed */
/* Copy the inside of the playfield (rows 1..h-1, columns 1..w-2) into a
 * scratch grid and back. The grid has to be a struct member and unsigned
 * for SHC to keep the row address inside the loop and load 0xbfff as a
 * 32-bit literal, as the ROM does. */
struct cell {
	unsigned short v;
	char pad[4];
};
struct field {
	struct cell *cells;        /* 0x00 */
	char pad0[0xde - 4];
	unsigned char h;           /* 0xde */
	unsigned char w;           /* 0xdf */
};
struct grid {
	unsigned short c[22][20];
	unsigned short a;          /* 0x370 */
	unsigned short b;          /* 0x372 */
};
extern struct grid g_6077608;

void f_50e0(struct field *f)
{
	short y, x;

	for (y = 1; y < f->h; y++)
		for (x = 1; x < f->w - 1; x++)
			g_6077608.c[y][x] = f->cells[y * f->w + x].v & 0xbfff;
}

void f_5168(struct field *f)
{
	short y, x;

	for (y = 1; y < f->h; y++)
		for (x = 1; x < f->w - 1; x++)
			f->cells[y * f->w + x].v = g_6077608.c[y][x];
}
