/* rom: 0x11518 len: 0x244 func: f_11518 flags: -macsave=1 -optimize=1 -speed */
/* Sprite drawing: f_11518 builds one sprite entry and appends it to sprite
 * RAM; f_1159c and f_11680 draw every piece of a multi-piece frame (the piece
 * count is the top 6 bits of the first piece's y). f_1159c is the plain draw,
 * f_11680 the 8-argument one. */
struct piece {
	short x;              /* 0x0 */
	short y;              /* 0x2: low 10 bits y, top 6 bits piece count */
	unsigned char b4;     /* 0x4 */
	unsigned char pad5;
	unsigned char b6;     /* 0x6 */
	unsigned char pad7;
	unsigned char b8;     /* 0x8 */
	unsigned char b9;     /* 0x9 */
	short w10;            /* 0xa */
};
struct spr {
	short x;
	short y;
	unsigned char b4;
	unsigned char b5;
	unsigned char b6;
	unsigned char b7;
	unsigned char b8;
	unsigned char b9;
	short w10;
	short w12;
	short w14;
};
extern struct spr g_6061932;
extern unsigned short g_60618ec;
extern struct piece d_aac10[], d_aac1c[];
extern void f_248a8(short, int);

void f_11518(struct piece *f, short x, short y, unsigned char c, short a5, short a6, short a7, unsigned char a8)
{
	short t;

	g_6061932.x = x & 0x3ff;
	g_6061932.y = y & 0x3ff;
	g_6061932.x += f->x & 0x3ff;
	g_6061932.y += f->y & 0x3ff;
	g_6061932.b4 = f->b4;
	g_6061932.b6 = (f->b6 & 0xcf) | 0x30;
	t = a6;		/* a direct store reads a6 as a byte */
	g_6061932.b5 = t;
	g_6061932.b7 = a7;
	g_6061932.b8 = c;
	g_6061932.b9 = a8;
	g_6061932.w10 = f->w10;
	/* 0x24000000 is sprite RAM through the cache-through mirror */
	*(struct spr *)(0x24000000 + g_60618ec * 16) = *(struct spr *)&g_6061932;
	g_60618ec++;
}

void f_1159c(struct piece *f, long x, long y, unsigned char c, short a5)
{
	short n;
	unsigned char attr;

	n = f->y >> 10;
	if (c == 0x94)
		attr = (f->b9 & 0x8f) | 0x40;
	else
		attr = f->b9 & 0x8f;
	f_248a8(a5, (unsigned short)f->y >> 10);
	if (c == 0) {
		for (; n; n--, f++)
			f_11518(f, x, y, f->b8, a5, 63, 63, attr);
	} else {
		for (; n; n--, f++)
			f_11518(f, x, y, c, a5, 63, 63, attr);
	}
}

void f_11680(struct piece *f, short x, short y, unsigned char c, short a5, short a6, short a7, unsigned char a8)
{
	short n;
	unsigned char attr;

	n = f->y >> 10;
	if (!c)
		c = f->b8;
	if ((c == 0x94) | a8)
		attr = (f->b9 & 0x8f) | 0x40;
	else
		attr = f->b9 & 0x8f;
	if (f == d_aac10 || f == d_aac1c)
		attr = (f->b9 & 0x8f) | 0x10;
	f_248a8(a5, (unsigned short)f->y >> 10);
	while (n) {
		f_11518(f, x, y, c, a5, a6, a7, attr);
		n--;
		f++;
	}
}
