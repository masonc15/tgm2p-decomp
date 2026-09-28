/* rom: 0x2090c len: 0x98 func: f_2090c flags: -macsave=1 -optimize=1 -speed */
struct point { short x, y; };
struct field {
	char pad0[0xde];
	unsigned char height;      /* 0x0de */
	unsigned char width;       /* 0x0df */
	char pad1[0x314 - 0xe0];
	struct point shake;        /* 0x314 */
};
extern void f_1159c(long, int, int, int, int);
extern long d_3b51c[];

void f_2090c(struct field *f, int row, int col, short n)
{
	struct point *s = &f->shake;
	short x;
	int y;

	x = col * 8 + s->x - f->width / 2 * 8 + 4;
	y = (f->height - row - 1) * 8 + s->y - (f->height - 1) * 8 - 10;
	if (n < 0)
		n = 0;
	if (n > 15)
		n = 15;
	f_1159c(d_3b51c[15 - n], y, x, 0xb5, 124);
}
