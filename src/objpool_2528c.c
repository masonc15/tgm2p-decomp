/* rom: 0x2528c len: 0x350 func: f_2528c flags: -macsave=1 -optimize=1 -speed */
struct obj {
	long p;                    /* 0x00 */
	short x;                   /* 0x04 */
	char pad06[2];
	short y;                   /* 0x08 */
	char pad0a[0x14 - 0xa];
	unsigned char a14;         /* 0x14 */
	unsigned char a15;         /* 0x15 */
	short w16;                 /* 0x16 */
	short w18;                 /* 0x18 */
	short w1a;                 /* 0x1a */
	unsigned char b1c;         /* 0x1c */
	char pad1d[0x22 - 0x1d];
	short w22;                 /* 0x22 */
	char pad24[0x28 - 0x24];
	short w28;                 /* 0x28 */
	unsigned char b2a;         /* 0x2a */
	unsigned char b2b;         /* 0x2b */
	unsigned short w2c;        /* 0x2c */
	unsigned char b2e;         /* 0x2e */
	char pad2f;
	short w30;                 /* 0x30 */
	short w32;                 /* 0x32 */
	short w34;                 /* 0x34 */
	short w36;                 /* 0x36 */
	short w38;                 /* 0x38 */
	char pad3a[0x40 - 0x3a];
};
extern short g_60ad224;
extern short g_606106c[];
extern struct obj g_606006c[];
extern long g_60618f0[];

short f_2528c(void)
{
	short i;
	struct obj *o;

	i = g_606106c[g_60ad224++];
	o = &g_606006c[i];
	o->w18 = 1;
	o->w22 = 0;
	o->a14 = o->a15 = 63;
	o->w1a = 0;
	o->b1c = 0;
	o->b2a = 16;
	o->b2b = 1;
	o->w2c = 0xa030;
	o->b2e = 0;
	o->w30 = 0;
	o->w32 = 0;
	o->w34 = 0;
	o->w16 = 0;
	o->w36 = 0;
	o->w28 = 0;
	return i;
}

void f_252f8(short i)
{
	struct obj *o;
	register int n;

	if (i < 0 || i >= 64)
		return;
	o = &g_606006c[i];
	o->b2b = 0;
	o->w2c = 0x8000;
	for (n = 0; g_606106c[n] != i; n++)
		;
	g_60ad224--;
	if (n != g_60ad224) {
		g_606106c[g_60ad224] = g_606106c[n] ^ g_606106c[g_60ad224];
		g_606106c[n] = g_606106c[g_60ad224] ^ g_606106c[n];
		g_606106c[g_60ad224] = g_606106c[n] ^ g_606106c[g_60ad224];
	}
}

void f_25380(short i)
{
	struct obj *o;

	if (i < 0 || i >= 64)
		return;
	o = &g_606006c[i];
	o->a14 = 63;
	o->a15 = 63;
	o->w1a = 0;
	o->b1c = 0;
	o->b2a = 16;
	o->b2b = 2;
}

void f_253bc(short i, short x, short y, long p)
{
	struct obj *o;

	if (i < 0 || i >= 64)
		return;
	o = &g_606006c[i];
	o->p = p;
	o->x = x;
	o->y = y;
	if (o->b2b != 2)
		o->b2b = 9;
	else
		o->b2b = 2;
	o->w16 = 0;
}

void f_25402(short i, long p)
{
	struct obj *o;

	if (i < 0 || i >= 64)
		return;
	o = &g_606006c[i];
	o->p = p;
	if (o->b2b != 2)
		o->b2b = 9;
	else
		o->b2b = 2;
	o->w16 = 0;
}

void f_2543c(short i, short x, short y, long p, short w)
{
	struct obj *o;

	f_253bc(i, x, y, p);
	o = &g_606006c[i];
	o->w16 = w;
}

void f_25474(short i, short x, short y, long p, short a, short b, unsigned short fl)
{
	struct obj *o;

	if (i < 0 || i >= 64)
		return;
	o = &g_606006c[i];
	o->p = p;
	o->x = x;
	o->y = y;
	o->b2b = 1;
	o->w2c = (fl & 0x8f00) | (o->w2c & 0x70ff) | 0x8000;
	o->w34 = b + 1;
	o->w36 = b;
	o->w28 = a;
	o->w38 = 0;
	if (fl & 0x800)
		o->w16 = a - 1;
	else
		o->w16 = 0;
}

void f_254f4(short i, short x, short y, long p, short a, short b, unsigned short fl)
{
	struct obj *o;

	if (i < 0 || i >= 64)
		return;
	o = &g_606006c[i];
	o->p = p;
	o->x = x;
	o->y = y;
	o->b2b = 1;
	o->w2c = (fl & 0x8f00) | (o->w2c & 0x70ff);
	o->w34 = b + 1;
	o->w36 = b;
	o->w28 = a;
	o->w38 = 0;
	if (fl & 0x800)
		o->w16 = a - 1;
	else
		o->w16 = 0;
}

int f_25570(short v)
{
	short n;

	n = (v - v % 16) / 16;
	if (n >= 15)
		n = 14;
	else if (n < 0)
		n = 0;
	if (g_60618f0[n]++ & 1)
		return 1;
	return 0;
}
