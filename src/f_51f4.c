/* rom: 0x51f4 len: 0xc2 func: f_51f4 flags: -macsave=1 -optimize=1 -speed */
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

/* 1 if no interior cell (rows 1..h-2, columns 1..w-2) is set, ignoring bit 0x4000. */
int f_51f4(struct field *f)
{
	short y, x;

	for (y = 1; y < f->h - 1; y++)
		for (x = 1; x < f->w - 1; x++)
			if (f->cells[y * f->w + x].v & ~0x4000)
				return 0;
	return 1;
}

/* 1 if every interior cell of row y is set, ignoring bit 0x4000. */
int f_5262(struct field *f, short y)
{
	short x;

	for (x = 1; x < f->w - 1; x++)
		if (!(f->cells[y * f->w + x].v & ~0x4000))
			return 0;
	return 1;
}
