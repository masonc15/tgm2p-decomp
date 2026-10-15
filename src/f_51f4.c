/* rom: 0x51f4 len: 0x228 func: f_51f4 flags: -macsave=1 -optimize=1 -speed */
/* Complete compiler unit through f_5390, including its shared literal pool.
 * A local grid pointer preserves the original first-load scheduling in f_5390. */
struct cell {
	unsigned short v;
	char b2;
	unsigned char b3;
	char pad[2];
};
struct field {
	struct cell *cells;          /* 0x000 */
	unsigned short rows[9][12];  /* 0x004 */
	char pad0dc[0xde - 0xdc];
	unsigned char h;             /* 0x0de */
	unsigned char w;             /* 0x0df */
	char pad0e0[0x378 - 0xe0];
	unsigned char b378;          /* 0x378 */
	char pad379;
	unsigned char b37a;          /* 0x37a */
	unsigned char b37b;          /* 0x37b */
	char pad37c[0x388 - 0x37c];
	unsigned char b388;          /* 0x388 */
};
struct player {
	char pad0[0x2f4];
	struct field *link;          /* 0x2f4 */
};
struct grid {
	unsigned short c[22][20];
};
extern struct grid g_6077608;
extern void f_17d48(struct field *f, short y);

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
	register short x;

	for (x = 1; x < f->w - 1; x++)
		if (!(f->cells[y * f->w + x].v & ~0x4000))
			return 0;
	return 1;
}

/* Clear the rows flagged in rows. Returns rows with each row that kept a
 * 0x400 cell unflagged and bit 31 set. */
long f_52b6(struct field *f, long rows)
{
	short y, x;
	long mask;
	struct cell *p;

	for (y = 1; y < f->h - 1; y++) {
		if (rows & (1 << y)) {
			f_17d48(f, y);
			x = 1;
			p = &f->cells[y * f->w + 1];
			mask = ~(1 << y);
			for (; x < f->w - 1; x++, p++) {
				if (p->v & 0x400) {
					rows |= 0x80000000;
					rows &= mask;
					p->v &= 0xf0ff;
				} else {
					if (p->v & 0x2000) {
						f->b378 = p->b3;
						f->b37a = x;
						f->b37b = y;
					}
					p->v = 0;
					p->b3 = 0;
				}
			}
		}
	}
	return rows;
}

void f_5390(struct player *pl, short idx, int add)
{
	struct field *f;
	register unsigned short *s;
	short x;
	struct grid *g = &g_6077608;

	s = &g->c[idx][1];
	f = pl->link;
	for (x = 1; x < 11; x++)
		f->rows[(unsigned char)(f->b388 + add)][x - 1] = *s++;
}
