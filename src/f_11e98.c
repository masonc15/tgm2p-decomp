/* rom: 0x11e98 len: 0x150 func: f_11e98 flags: -macsave=1 -optimize=1 -speed */
/* Two playfield and display helpers, a wrapper and empty stubs. This matches
 * from a fresh file, and f_11fe8.c still matches compiled after it, so the
 * two may be one source file. */
struct cell {
	unsigned short flags;
	short a;
	short b;
};
struct field {
	struct cell *cells;        /* 0x000 */
	char pad0[0xdf - 4];
	unsigned char width;       /* 0x0df */
};
extern void f_18708(long, int, int);
extern long g_6079424[];

/* Counts the cells of the 10x20 playfield with any of the low 4 flag bits set. */
int f_11e98(struct field *f)
{
	short x, y;
	int n = 0;

	for (x = 1; x < 11; x++)
		for (y = 1; y < 21; y++)
			if (f->cells[y * f->width + x].flags & 15)
				n++;
	return n;
}

/* Draws ten values 12 pixels apart with f_18708, then their sum 8 pixels
 * below the last. */
void f_11f60(long *v, int y, int attr)
{
	int i;
	long sum = 0;

	for (i = 0; i < 10; i++) {
		f_18708(v[i], y, attr);
		sum += v[i];
		y += 12;
	}
	f_18708(sum, y + 8, attr);
}

void f_11fc0(void)
{
}

/* The ROM calls f_11f60 and returns rather than jumping to it, and f_11fd4
 * doesn't inline this. Calls to the empty f_11fc0 (inlined to nothing) are
 * one way to get both: any call blocks the tail call, and three make this
 * too big to inline. The original probably had statements that compile
 * away, such as debug output. */
void f_11fc4(void)
{
	f_11f60(g_6079424, 40, 0xaa);
	f_11fc0();
	f_11fc0();
	f_11fc0();
}

void f_11fd4(void)
{
	f_11fc4();
}

void f_11fd8(void)
{
}
