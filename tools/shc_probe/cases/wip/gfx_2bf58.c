/* rom: 0x2bf58 len: 0x1364 func: f_2bf58 flags: -macsave=1 -optimize=1 -speed */
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

void f_2c9a4(struct obj *o, struct frame *f, int y, short x, short kind)
{
	long v;
	 short neg;
	short n, i; 
	struct obj *p = o;
	short n9;
	char buf[10];

	switch (kind & 0x7fff) {
	case 7:
		v = *(char *)o->def;
		break;
	case 8:
		v = *(short *)o->def;
		break;
	case 9:
		v = *(long *)o->def;
		break;
	}
	neg = 0;
	if (v < 0) {
		v = ~v + 1;
		neg = 1;
	}
	if (v > 100000000) {
		buf[0] = 'w';
		buf[1] = 'o';
		buf[2] = 'l';
		buf[3] = 'f';
		buf[4] = 'r';
		buf[5] = 'e';
		buf[6] = 'v';
		buf[7] = 'O';
		n = 7;
	} else {
		 for (i = 0; i < 10;) {
			buf[i++] = v % 10 + '0';
			v /= 10;
		} 
		 for (n = 9; buf[n] == '0' && n > 0; n--)
			; 
		 if (neg)
			buf[++n] = '-'; 
	}
	n9 = n * 9;
	for (neg = n; neg >= 0; neg--) {
		p->w30 = buf[neg];
		p->w12 = x & 0x3ff;
		 if (o->l60 & 16) {
			if (n)
				p->w14 = (y - n * 9) & 0x3ff;
			else
				p->w14 = y & 0x3ff;
		} else
			p->w14 = y & 0x3ff; 
		if (kind & 0x8000) {
			p->w30 = (unsigned short)(p->w30 + f->w10 + 0xffe0);
			f_248a8(o->b42, (unsigned short)p->w14 >> 10);
			f_2e18c(11, (long)p);
			y += 6;
		} else {
			f_248a8(o->b42, (unsigned short)p->w14 >> 10);
			f_2e18c(11, (long)p);
			y += 9;
		}
	}
}

void f_2cb6e(struct obj *o, struct frame *f, short y, short x, short kind)
{
	struct obj *p = o;
	char *s = o->def;
	struct frame *font = g_607926c->font;
	struct frame *dash = &font[29];
	struct frame *unk = &font[30];
	short c;
	long v;

	for (;;) {
		if (kind == 3) {
			c = *(short *)s;
			s += 2;
		} else {
			c = *s++;
		}
		if (c == -1)
			return;
		if (kind != 3 && c == 0)
			return;
		switch (c) {
		case -3:
			y += 15;
			break;
		case -2:
			y = p->y;
			x += o->w52;
			break;
		default:
			p->w12 = x & 0x3ff;
			p->w14 = y & 0x3ff;
			switch ((unsigned short)kind) {
			case 1:
				p->w30 = c + 0x1800;
				y += g_603585a[(unsigned char)c];
				break;
			case 0x1001:
				p->w16 = font->b4;
				p->w18 = (font->b6 & 0xcf) | (p->w18 & 0x30);
				if (c == ' ') {
					y += 9;
					continue;
				}
				if (c == '-') {
					p->w30 = dash->w10;
					y += 9;
				} else {
					c -= 'A';
					if (c < 0 || c > 25) {
						p->w30 = unk->w10;
						y += 9;
					} else {
						p->w30 = font[c].w10;
						y += g_6035824[(unsigned char)c];
						if (*s - 'A' == 8)
							y -= 2;
					}
				}
				break;
			case 3:
				v = (unsigned short)HDR[0]->w10 | ((HDR[0]->b9 & 15) << 16); v += c;
				p->b28 = (p->b28 & 0xf0) | ((v >> 16) & 15);
				p->w30 = v;
				y += 15;
				break;
			case 0x8001:
				y += 6;
				p->w30 = (unsigned short)(f->w10 + c + 0xffe0);
				p->b28 = f->b9 | (p->b28 & 0xf0);
				break;
			}
			f_248a8(o->b42, (unsigned short)p->w14 >> 10);
			f_2e18c(11, (long)p);
			break;
		}
	}
}

void f_2cd42(void)
{
	struct frame *f = HDR[1];
	short i;
	short *p;
	struct obj *o;
	short kind, x, y, k;

	for (i = g_606193e + 1, p = &g_606106c[i]; i < 64; i++, p++) {
		o = &g_606006c[*p];
		if (0) o->w16 = i;
		if (0) o->w16 = i;
		if (0) o->w16 = i;
		if (0) o->w16 = i;
		if (0) o->w16 = i;
		if (0) o->w16 = i;
		if (0) o->w16 = i;
		if (0) o->w16 = i;
		if (0) o->w16 = i;
		if (0) o->w16 = 0;
		if (0) o->w16 = 0;
		if (0) o->w16 = 0;
		if (0) o->w16 = (long)p;
		if (0) o->w16 = (long)p;
		if (0) o->w16 = (long)p;
		if (0) o->w16 = (long)p;
		if (0) o->w16 = (long)p;
		if (0) o->w16 = (long)f;
		if (0) o->w16 = (long)f;
		if (0) o->w16 = (long)f;
		if (0) o->w16 = 0x7fff;
		if (0) o->w16 = 0x7fff;
		if (0) o->w16 = 0xf0;
		if (0) o->w16 = 64;
		o->w18 = o->w16 = 0;
		kind = o->w54;
		x = o->x;
		if (x > 0xf0)
			continue;
		y = o->y;
		if (y > 0x140)
			continue;
		o->w26 = o->w26 ? o->w26 : g_6061940;
		o->w18 = (o->w24 & 3) << 4;
		switch (o->type) {
		case 5:
			f_2cb6e(o, f, y, x, kind);
			break;
		case 6:
			k = kind & 0x7fff;
			if (0) o->w16 = y;
			if (k == 4 || k == 5 || k == 6)
				f_2c878(o, f, y, x, kind);
			else
				f_2c9a4(o, f, y, x, kind);
			break;
		}
	}
}

#pragma inline(put_line)
static short put_line(char *row, short x)
{
	return f_2bf58(row, x, (0x140 - f_2c0d0(row)) >> 1);
}
short f_2ce54(char *s, short x, short dx, short attr)
{
	short id = 0, line = 0, col = 0;

	while (*s) {
		g_60b13c4[line][col] = *s++;
		if (g_60b13c4[line][col] == -1 || g_60b13c4[line][col] == 0) {
			g_60b13c4[line][col] = 0;
			id = put_line(g_60b13c4[line], x);
			g_606006c[id].w24 = attr & 3;
			g_606006c[id].flags = 0;
			break;
		}
		if (g_60b13c4[line][col] == -2) {
			g_60b13c4[line][col] = 0;
			id = put_line(g_60b13c4[line], x);
			g_606006c[id].w24 = attr & 3;
			g_606006c[id].flags = 0;
			line++;
			col = 0;
			x -= dx;
		} else {
			col++;
		}
	}
	return id;
}

short f_2cf62(void *def, short x, short y)
{
	short i = g_606106c[g_606193e--];

	g_606006c[i].def = def;
	g_606006c[i].type = 5;
	g_606006c[i].x = x;
	g_606006c[i].y = y;
	g_606006c[i].w52 = g_6061942;
	g_606006c[i].w54 = 0x8001;
	g_606006c[i].b42 = g_6061947;
	g_606006c[i].w44 &= 0x7fff;
	g_606006c[i].b28 = 0;
	g_606006c[i].w26 = 0;
	g_606006c[i].w24 = 3;
	g_606006c[i].flags = 0;
	g_606006c[i].b20 = 63;
	g_606006c[i].b21 = 63;
	return i;
}

short f_2cff4(void *def, short x, short y)
{
	short i = g_606106c[g_606193e--];

	g_606006c[i].def = def;
	g_606006c[i].type = 6;
	g_606006c[i].x = x;
	g_606006c[i].y = y;
	g_606006c[i].w54 = 0x8004;
	g_606006c[i].l60 = 0;
	g_606006c[i].b42 = g_6061947;
	g_606006c[i].w44 &= 0x7fff;
	g_606006c[i].b28 = 0;
	g_606006c[i].w26 = 0;
	g_606006c[i].w24 = 3;
	g_606006c[i].flags = 0;
	g_606006c[i].b20 = 63;
	g_606006c[i].b21 = 63;
	return i;
}

short f_2d060(void *def, short x, short y)
{
	short i = g_606106c[g_606193e--];

	g_606006c[i].def = def;
	g_606006c[i].type = 6;
	g_606006c[i].x = x;
	g_606006c[i].y = y;
	g_606006c[i].w54 = 0x8005;
	g_606006c[i].l60 = 0;
	g_606006c[i].b42 = g_6061947;
	g_606006c[i].w44 &= 0x7fff;
	g_606006c[i].b28 = 0;
	g_606006c[i].w26 = 0;
	g_606006c[i].w24 = 3;
	g_606006c[i].flags = 0;
	g_606006c[i].b20 = 63;
	g_606006c[i].b21 = 63;
	return i;
}

short f_2d0cc(void *def, short x, short y)
{
	short i = g_606106c[g_606193e--];

	g_606006c[i].def = def;
	g_606006c[i].type = 6;
	g_606006c[i].x = x;
	g_606006c[i].y = y;
	g_606006c[i].w54 = 0x8006;
	g_606006c[i].l60 = 0;
	g_606006c[i].b42 = g_6061947;
	g_606006c[i].w44 &= 0x7fff;
	g_606006c[i].b28 = 0;
	g_606006c[i].w26 = 0;
	g_606006c[i].w24 = 3;
	g_606006c[i].flags = 0;
	g_606006c[i].b20 = 63;
	g_606006c[i].b21 = 63;
	return i;
}

short f_2d158(void *def, short x, short y)
{
	short i = g_606106c[g_606193e--];

	g_606006c[i].def = def;
	g_606006c[i].type = 6;
	g_606006c[i].x = x;
	g_606006c[i].y = y;
	g_606006c[i].w54 = 0x8007;
	g_606006c[i].l60 = 0;
	g_606006c[i].b42 = g_6061947;
	g_606006c[i].w44 &= 0x7fff;
	g_606006c[i].b28 = 0;
	g_606006c[i].w26 = 0;
	g_606006c[i].w24 = 3;
	g_606006c[i].flags = 0;
	g_606006c[i].b20 = 63;
	g_606006c[i].b21 = 63;
	return i;
}

short f_2d1c4(void *def, short x, short y)
{
	short i = g_606106c[g_606193e--];

	g_606006c[i].def = def;
	g_606006c[i].type = 6;
	g_606006c[i].x = x;
	g_606006c[i].y = y;
	g_606006c[i].w54 = 0x8008;
	g_606006c[i].l60 = 0;
	g_606006c[i].b42 = g_6061947;
	g_606006c[i].w44 &= 0x7fff;
	g_606006c[i].b28 = 0;
	g_606006c[i].w26 = 0;
	g_606006c[i].w24 = 3;
	g_606006c[i].flags = 0;
	g_606006c[i].b20 = 63;
	g_606006c[i].b21 = 63;
	return i;
}

short f_2d230(void *def, short x, short y)
{
	short i = g_606106c[g_606193e--];

	g_606006c[i].def = def;
	g_606006c[i].type = 6;
	g_606006c[i].x = x;
	g_606006c[i].y = y;
	g_606006c[i].w54 = 0x8009;
	g_606006c[i].l60 = 0;
	g_606006c[i].b42 = g_6061947;
	g_606006c[i].w44 &= 0x7fff;
	g_606006c[i].b28 = 0;
	g_606006c[i].w26 = 0;
	g_606006c[i].w24 = 3;
	g_606006c[i].flags = 0;
	g_606006c[i].b20 = 63;
	g_606006c[i].b21 = 63;
	return i;
}
