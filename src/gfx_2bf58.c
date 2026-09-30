/* rom: 0x2bf58 len: 0xa4c func: f_2bf58 flags: -macsave=1 -optimize=1 -speed */
struct frame {
	short dx;                  /* 0 */
	short dy;                  /* 2 */
	unsigned char b4;          /* 4 */
	char pad5;
	unsigned char b6;          /* 6 */
	char pad7;
	unsigned char b8;          /* 8 */
	unsigned char b9;          /* 9 */
	short w10;                 /* 0xa */
};
struct obj {
	void *def;                 /* 0x00 */
	short x;                   /* 0x04 */
	char pad6[2];
	short y;                   /* 0x08 */
	char pad10[2];
	short w12;                 /* 0x0c */
	short w14;                 /* 0x0e */
	short w16;                 /* 0x10 */
	short w18;                 /* 0x12 */
	unsigned char b20;         /* 0x14 */
	unsigned char b21;         /* 0x15 */
	short frame;               /* 0x16 */
	unsigned short w24;        /* 0x18 */
	unsigned short w26;        /* 0x1a */
	unsigned char b28;         /* 0x1c */
	char pad29;
	short w30;                 /* 0x1e */
	short w32;                 /* 0x20 */
	unsigned short flags;      /* 0x22 */
	char pad36[6];
	unsigned char b42;         /* 0x2a */
	unsigned char type;        /* 0x2b */
	unsigned short w44;               /* 0x2c */
	char pad46[6];
	short w52;                 /* 0x34 */
	unsigned short w54;                 /* 0x36 */
	char pad56[4];
	long l60;                  /* 0x3c */
};
struct font_owner {
	char pad[0xd4];
	struct frame *font;        /* 0xd4 */
};
extern struct obj g_606006c[];
extern short g_606106c[];
extern short g_606193e;
extern short g_6061940;
extern short g_6061942;
extern short g_6061944;
extern short g_6061946;
extern unsigned char g_6061947;
extern short g_6061948;
extern short g_603585a[];
extern short g_6035824[];
extern struct font_owner *g_607926c;
extern char g_60b13c4[8][60];
extern void f_248a8(short id, unsigned short n);
extern void f_2e18c(short a, long b);
#define HDR (*(struct frame ***)0x2004002c)

short f_2bf58(void *def, short x, short y)
{
	short i = g_606106c[g_606193e--];

	g_606006c[i].def = def;
	g_606006c[i].type = 5;
	g_606006c[i].x = x;
	g_606006c[i].y = y;
	g_606006c[i].w52 = g_6061942;
	g_606006c[i].w54 = 1;
	g_606006c[i].b42 = g_6061944;
	g_606006c[i].w44 &= 0x7fff;
	g_606006c[i].b28 = 0;
	g_606006c[i].w26 = 0;
	g_606006c[i].w24 = 3;
	g_606006c[i].flags = 0;
	g_606006c[i].b20 = 63;
	g_606006c[i].b21 = 63;
	return i;
}

short f_2bfca(void *def, short x, short y)
{
	short i = g_606106c[g_606193e--];

	g_606006c[i].def = def;
	g_606006c[i].type = 5;
	g_606006c[i].x = x;
	g_606006c[i].y = y;
	g_606006c[i].w52 = g_6061942;
	g_606006c[i].w54 = 0x1001;
	g_606006c[i].b42 = g_6061944;
	g_606006c[i].w44 &= 0x7fff;
	g_606006c[i].b28 = 0;
	g_606006c[i].w26 = 0;
	g_606006c[i].w24 = 3;
	g_606006c[i].flags = 0;
	g_606006c[i].b20 = 63;
	g_606006c[i].b21 = 63;
	return i;
}

void f_2c03c(short a)
{
	g_6061948 = a;
}

short f_2c042(void *def, short x, short y)
{
	short i = g_606106c[g_606193e--];

	g_606006c[i].def = def;
	g_606006c[i].type = 5;
	g_606006c[i].x = x;
	g_606006c[i].y = y;
	g_606006c[i].w52 = g_6061948;
	g_606006c[i].w54 = 3;
	g_606006c[i].b42 = g_6061944;
	g_606006c[i].w44 &= 0x7fff;
	g_606006c[i].b28 = 0;
	g_606006c[i].w26 = 0;
	g_606006c[i].w24 = 3;
	g_606006c[i].flags = 0;
	g_606006c[i].b20 = 63;
	g_606006c[i].b21 = 63;
	return i;
}

short f_2c0d0(char *s)
{
	short max = 0, w = 0;
	short c;

	for (;;) {
		c = *s++;
		if (c == -1 || c == 0) {
			if (w > max)
				max = w;
			break;
		}
		if (c == -2) {
			if (w > max)
				max = w;
			w = 0;
			c = *s++;
		}
		if (c == -6 || c == -5 || c == -4 || c == -3)
			w += 10;
		else
			w += g_603585a[c];
	}
	return max;
}

int f_2c13a(short *s)
{
	short max = 0, w = 0;
	int m;

	while (*s != -1) {
		if (*s != -2 && *s != -1) {
			w++;
		} else {
			max = max > w ? max : w;
			w = 0;
		}
		s++;
	}
	m = max > w ? max : w;
	return m * 15;
}

short f_2c186(unsigned char *s)
{
	short max = 0, w = 0;
	unsigned char c;

	for (;;) {
		c = *s++;
		
		if (c == -1 || c == 0) {
			if (w > max)
				max = w;
			break;
		}
		
		if (c == -2) {
			if (w > max)
				max = w;
			w = 0;
			continue;
		}
		if (0) w += 25;
		if (c == ' ' || c == '-') {
			w += 9;
			continue;
		}
		
		c -= 'A';
		
		if (c < 0 || c > 25)
			w += 9;
		else
			w += g_6035824[c];
	}
	
	return max;
}

void f_2c1f0(void)
{
	register int i;

	for (i = g_606193e + 1; i < 64; i++) {
		g_606006c[g_606106c[i]].type = 0;
		g_606006c[g_606106c[i]].w44 = 0x8000;
		g_606193e++;
		if (g_606193e != i) {
			g_606106c[i] = g_606106c[g_606193e] ^ g_606106c[i];
			g_606106c[g_606193e] = g_606106c[g_606193e] ^ g_606106c[i];
			g_606106c[i] = g_606106c[g_606193e] ^ g_606106c[i];
		}
	}
}

void f_2c28c(void)
{
	register int i;

	for (i = g_606193e + 1; i < 64; i++) {
		if (g_606006c[g_606106c[i]].type == 5) {
			g_606006c[g_606106c[i]].type = 0;
			g_606006c[g_606106c[i]].w44 = 0x8000;
			g_606193e++;
			if (g_606193e != i) {
				g_606106c[i] = g_606106c[g_606193e] ^ g_606106c[i];
				g_606106c[g_606193e] = g_606106c[g_606193e] ^ g_606106c[i];
				g_606106c[i] = g_606106c[g_606193e] ^ g_606106c[i];
			}
		}
	}
}

void f_2c326(void)
{
	short i;

	for (i = g_606193e + 1; i < 64; i++) {
		if (g_606006c[g_606106c[i]].type == 6) {
			g_606006c[g_606106c[i]].type = 0;
			g_606006c[g_606106c[i]].w44 = 0x8000;
			g_606193e++;
			if (g_606193e != i) {
				g_606106c[i] = g_606106c[g_606193e] ^ g_606106c[i];
				g_606106c[g_606193e] = g_606106c[g_606193e] ^ g_606106c[i];
				g_606106c[i] = g_606106c[g_606193e] ^ g_606106c[i];
			}
		}
	}
}

void f_2c3ca(short a)
{
	g_6061942 = a;
}

void f_2c3d0(unsigned char a)
{
	g_6061940 = a;
}

void f_2c3d8(unsigned char a)
{
	g_6061944 = a;
}

void f_2c3e0(unsigned char a)
{
	g_6061946 = a;
}

short f_2c3e8(void *def, short x, short y, short mode)
{
	short i = g_606106c[g_606193e--];

	g_606006c[i].def = def;
	g_606006c[i].type = 6;
	g_606006c[i].x = x;
	g_606006c[i].y = y;
	g_606006c[i].w54 = 4;
	if (mode == 2)
		g_606006c[i].l60 = 16;
	else
		g_606006c[i].l60 = 0;
	g_606006c[i].b42 = g_6061944;
	g_606006c[i].w44 &= 0x7fff;
	g_606006c[i].b28 = 0;
	g_606006c[i].w26 = 0;
	g_606006c[i].w24 = 3;
	g_606006c[i].flags = 0;
	g_606006c[i].b20 = 63;
	g_606006c[i].b21 = 63;
	return i;
}

short f_2c498(void *def, short x, short y, short mode)
{
	short i = g_606106c[g_606193e--];

	g_606006c[i].def = def;
	g_606006c[i].type = 6;
	g_606006c[i].x = x;
	g_606006c[i].y = y;
	g_606006c[i].w54 = 5;
	if (mode == 2)
		g_606006c[i].l60 = 16;
	else
		g_606006c[i].l60 = 0;
	g_606006c[i].b42 = g_6061944;
	g_606006c[i].w44 &= 0x7fff;
	g_606006c[i].b28 = 0;
	g_606006c[i].w26 = 0;
	g_606006c[i].w24 = 3;
	g_606006c[i].flags = 0;
	g_606006c[i].b20 = 63;
	g_606006c[i].b21 = 63;
	return i;
}

short f_2c528(void *def, short x, short y, short mode)
{
	short i = g_606106c[g_606193e--];

	g_606006c[i].def = def;
	g_606006c[i].type = 6;
	g_606006c[i].x = x;
	g_606006c[i].y = y;
	g_606006c[i].w54 = 6;
	if (mode == 2)
		g_606006c[i].l60 = 16;
	else
		g_606006c[i].l60 = 0;
	g_606006c[i].b42 = g_6061944;
	g_606006c[i].w44 &= 0x7fff;
	g_606006c[i].b28 = 0;
	g_606006c[i].w26 = 0;
	g_606006c[i].w24 = 3;
	g_606006c[i].flags = 0;
	g_606006c[i].b20 = 63;
	g_606006c[i].b21 = 63;
	return i;
}

short f_2c5c8(void *def, short x, short y, short mode)
{
	short i = g_606106c[g_606193e--];

	g_606006c[i].def = def;
	g_606006c[i].type = 6;
	g_606006c[i].x = x;
	g_606006c[i].y = y;
	g_606006c[i].w54 = 7;
	if (mode == 2)
		g_606006c[i].l60 = 16;
	else
		g_606006c[i].l60 = 0;
	g_606006c[i].b42 = g_6061944;
	g_606006c[i].w44 &= 0x7fff;
	g_606006c[i].b28 = 0;
	g_606006c[i].w26 = 0;
	g_606006c[i].w24 = 3;
	g_606006c[i].flags = 0;
	g_606006c[i].b20 = 63;
	g_606006c[i].b21 = 63;
	return i;
}

short f_2c658(void *def, short x, short y, short mode)
{
	short i = g_606106c[g_606193e--];

	g_606006c[i].def = def;
	g_606006c[i].type = 6;
	g_606006c[i].x = x;
	g_606006c[i].y = y;
	g_606006c[i].w54 = 8;
	if (mode == 2)
		g_606006c[i].l60 = 16;
	else
		g_606006c[i].l60 = 0;
	g_606006c[i].b42 = g_6061944;
	g_606006c[i].w44 &= 0x7fff;
	g_606006c[i].b28 = 0;
	g_606006c[i].w26 = 0;
	g_606006c[i].w24 = 3;
	g_606006c[i].flags = 0;
	g_606006c[i].b20 = 63;
	g_606006c[i].b21 = 63;
	return i;
}

short f_2c6fc(void *def, short x, short y, short mode)
{
	short i = g_606106c[g_606193e--];

	g_606006c[i].def = def;
	g_606006c[i].type = 6;
	g_606006c[i].x = x;
	g_606006c[i].y = y;
	g_606006c[i].w54 = 9;
	if (mode == 2)
		g_606006c[i].l60 = 16;
	else
		g_606006c[i].l60 = 0;
	g_606006c[i].b42 = g_6061944;
	g_606006c[i].w44 &= 0x7fff;
	g_606006c[i].b28 = 0;
	g_606006c[i].w26 = 0;
	g_606006c[i].w24 = 3;
	g_606006c[i].flags = 0;
	g_606006c[i].b20 = 63;
	g_606006c[i].b21 = 63;
	return i;
}

short f_2c78c(void *def, short x, short y)
{
	short i = g_606106c[g_606193e--];

	g_606006c[i].def = def;
	g_606006c[i].type = 6;
	g_606006c[i].x = x;
	g_606006c[i].y = y;
	g_606006c[i].w54 = 0x1009;
	g_606006c[i].b42 = g_6061944;
	g_606006c[i].w44 &= 0x7fff;
	g_606006c[i].b28 = 0;
	g_606006c[i].w26 = 0;
	g_606006c[i].w24 = 3;
	g_606006c[i].flags = 0;
	g_606006c[i].b20 = 63;
	g_606006c[i].b21 = 63;
	return i;
}

short f_2c7f6(void *def, short x, short y)
{
	short i = g_606106c[g_606193e--];

	g_606006c[i].def = def;
	g_606006c[i].type = 6;
	g_606006c[i].x = x;
	g_606006c[i].y = y;
	g_606006c[i].w54 = 0x1008;
	g_606006c[i].b42 = g_6061944;
	g_606006c[i].w44 &= 0x7fff;
	g_606006c[i].b28 = 0;
	g_606006c[i].w26 = 0;
	g_606006c[i].w24 = 3;
	g_606006c[i].flags = 0;
	g_606006c[i].b20 = 63;
	g_606006c[i].b21 = 63;
	return i;
}

void f_2c878(struct obj *o, struct frame *f, int y, short x, short kind)
{
	struct obj *p = o;
	short n;
	long v;
	short xm;
	short d;

	switch (kind & 0x7fff) {
	case 4:
		n = 1;
		v = *(char *)o->def;
		break;
	case 5:
		n = 3;
		v = *(short *)o->def;
		break;
	case 6:
		n = 7;
		v = *(long *)o->def;
		break;
	}
	for (; n >= 0; n--) {
		d = (v >> (n * 4)) & 15;
		p->w30 = d < 10 ? d + '0' : d + 55;
		if (o->l60 & 16)
			p->w14 = (y - n * 9) & 0x3ff;
		else
			p->w14 = y & 0x3ff;
		p->w12 = x & 0x3ff;
		if (kind & 0x8000) {
			p->w30 = (unsigned short)(f->w10 + p->w30 + 0xffe0);
			f_248a8(o->b42, (unsigned short)p->w14 >> 10);
			f_2e18c(11, (long)p);
			y += 6;
		} else {
			f_248a8(o->b42, (unsigned short)p->w14 >> 10);
			f_2e18c(11, (long)p);
			y += 10;
		}
	}
}
