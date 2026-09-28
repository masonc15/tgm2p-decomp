/* rom: 0x2e06c len: 0x14c func: f_2e06c flags: -macsave=1 -optimize=1 -speed */
struct sprite {
	short x;                   /* 0 */
	short y;                   /* 2 */
	unsigned char b4, b5, b6, b7, b8, b9;
	short w10;
	short w12;
	short w14;
};
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
	struct frame *def;         /* 0 */
	short x;                   /* 4 */
	char pad6[2];
	short y;                   /* 8 */
	char pad10[2];
	short w12;                 /* 0xc */
	short w14;                 /* 0xe */
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
};
extern struct sprite g_6061932;
extern unsigned short g_60618ec;
extern void (*g_603595c[])(long);
#define SPRITE_RAM ((struct sprite *)0x24000000)

void f_2e06c(void)
{
	g_6061932.x = 200;
	g_6061932.y = 10;
	g_6061932.b4 = 0;
	g_6061932.b5 = 63;
	g_6061932.b6 = 48;
	g_6061932.b7 = 63;
	g_6061932.b8 = 0;
	g_6061932.b9 = 0;
	g_6061932.w10 = 31;
	SPRITE_RAM[g_60618ec] = *&g_6061932;
	g_60618ec++;
}

void f_2e0cc(struct obj *o)
{
	g_6061932.x = o->w12;
	g_6061932.y = o->w14;
	g_6061932.b4 = o->w16;
	g_6061932.b6 = o->w18;
	g_6061932.b5 = 63;
	g_6061932.b7 = 63;
	g_6061932.b8 = o->w26;
	g_6061932.b9 = 0;
	g_6061932.w10 = 15;
	SPRITE_RAM[g_60618ec] = *&g_6061932;
	g_60618ec++;
}

void f_2e128(struct obj *o)
{
	g_6061932.x = o->w12;
	g_6061932.y = o->w14;
	g_6061932.b4 = o->w16;
	g_6061932.b6 = o->w18;
	g_6061932.b5 = o->b20;
	g_6061932.b7 = o->b21;
	g_6061932.b8 = o->w26;
	g_6061932.b9 = o->b28;
	g_6061932.w10 = o->w30;
	SPRITE_RAM[g_60618ec] = *&g_6061932;
	g_60618ec++;
}

void f_2e18c(short a, long b)
{
	(*g_603595c[(unsigned char)a])(b);
}
