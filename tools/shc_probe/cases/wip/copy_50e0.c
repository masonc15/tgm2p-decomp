/* rom: 0x50e0 len: 0x114 func: f_50e0 flags: -macsave=1 -optimize=1 -speed */
/* WIP (40%): copies playfield flags into a 20-short-wide table and back.
 * The row offset (short)(y * 40) now matches; the inner-loop address order
 * and the 32-bit 0x0000bfff mask don't. f_5168 (the copy back) is still the
 * plain draft. */
struct cell {
	unsigned short flags;
	short a;
	short b;
};
struct field {
	struct cell *cells;        /* 0x000 */
	char pad0[0xde - 4];
	unsigned char height;      /* 0x0de */
	unsigned char width;       /* 0x0df */
};
extern short g_6077608[];

void f_50e0(struct field *f)
{
	short x, y;

	for (y = 1; y < f->height; y++)
		for (x = 1; x < f->width - 1; x++)
			((short *)((char *)g_6077608 + (short)(y * 40)))[x] = f->cells[y * f->width + x].flags & 0xbfff;
}
void f_5168(struct field *f)
{
	short x, y;

	for (y = 1; y < f->height; y++)
		for (x = 1; x < f->width - 1; x++)
			f->cells[y * f->width + x].flags = ((short *)((char *)g_6077608 + (short)(y * 40)))[x];
}
