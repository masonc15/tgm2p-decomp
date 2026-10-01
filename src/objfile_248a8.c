/* rom: 0x248a8 len: 0x11c func: f_248a8 flags: -macsave=1 -optimize=1 -speed */
struct obj {
	long p;                    /* 0x00 */
	short x;                   /* 0x04 */
	char pad06[2];
	short y;                   /* 0x08 */
	char pad0a[0x14 - 0xa];
	unsigned char a14;         /* 0x14 */
	unsigned char a15;         /* 0x15 */
	short w16;                 /* 0x16 */
	unsigned short w18;        /* 0x18 */
	unsigned short w1a;        /* 0x1a */
	unsigned char b1c;         /* 0x1c */
	char pad1d[0x22 - 0x1d];
	unsigned short w22;        /* 0x22 */
	char pad24[0x28 - 0x24];
	short w28;                 /* 0x28 */
	unsigned char b2a;         /* 0x2a */
	unsigned char b2b;         /* 0x2b */
	unsigned short w2c;        /* 0x2c */
	unsigned char b2e;         /* 0x2e */
	unsigned char b2f;         /* 0x2f */
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
extern short g_60618ec;
extern short g_60611ec[];
extern short g_60610ec[];
extern short g_606193e, g_6061944, g_6061946;
extern unsigned short g_6060000;
extern unsigned short g_6060002;
extern void f_2e18c(int, int);
struct part {
	short x;                   /* 0x0 */
	short y;                   /* 0x2, top 6 bits: parts in this frame */
	unsigned char b4;
	unsigned char b5;
	unsigned char b6;
	unsigned char b7;
	unsigned char b8;
	unsigned char b9;
	short wA;
};
struct spr {
	short x;
	short y;
	unsigned char a4, a5, a6, a7, a8, a9;
	short aA;
	short aC;
	short aE;
};
extern struct spr g_6061932;
extern short g_60356c8[];
void f_24b68(struct obj *o);
void f_24e9c(struct obj *o);

void f_248a8(short id, unsigned short n)
{
	short i;
	short base;

	base = g_60618ec;
	for (i = 0; i < n - 1; i++)
		g_60611ec[base + i] = base + i + 1;
	g_60611ec[base + i] = g_60610ec[id];
	g_60610ec[id] = base;
}

void f_248f0(unsigned char id)
{
	short *p;

	p = &g_60610ec[id];
	*p = 0;
}

void f_248fe(void)
{
	int i, j;

	j = 0xe0;
	for (i = 0; i < 8; i++, j++)
		((char *)0x2405ff00)[j] = 0;
}

void f_2491c(void)
{
	short i;
	struct obj *o;

	f_2e18c(1, 0);
	f_248fe();
	*(char *)0x2405ffe8 = 0x13;
	*(char *)0x2405ffe9 = 0x67;
	o = g_606006c;
	for (i = 0; i < 64; i++, o++) {
		o->b2b = 0;
		o->w2c = 0x8000;
		g_606106c[i] = i;
	}
	g_60ad224 = 0;
	g_606193e = 63;
	g_6061944 = 125;
	g_6061946 = 127;
}

