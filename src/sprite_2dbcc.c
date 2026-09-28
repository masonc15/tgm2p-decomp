/* rom: 0x2dbcc len: 0x4a0 func: f_2dbcc flags: -macsave=1 -optimize=1 -speed */
/* Sprite-list writers: fill the 16-byte sprite command at g_6061932, then
 * append it to sprite RAM (0x24000000, cache-through 0x04000000) at index
 * g_60618ec. f_2dbcc and f_2dc54 restart the list (count = 0) and emit two
 * fixed sprites; the others build one sprite from an object.
 * Matching note: the append copies from `*&g_6061932`, which makes SHC load
 * the struct's address again for the copy, as the ROM does; a plain
 * `= g_6061932` reuses the register. The same idiom matches the next two
 * sprite writers at 0x2e06c and 0x2e0cc (see spr_e.c). */
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
	char pad29[3];
	short w32;                 /* 0x20 */
	unsigned short flags;      /* 0x22 */
};
extern struct sprite g_6061932;
extern unsigned short g_60618ec;
#define SPRITE_RAM ((struct sprite *)0x24000000)

void f_2dbcc(void)
{
	g_60618ec = 0;
	g_6061932.x = g_6061932.y = -16;
	g_6061932.b7 = 63;
	g_6061932.b5 = 63;
	g_6061932.b4 = 0;
	g_6061932.b6 = 0;
	g_6061932.b8 = 0;
	g_6061932.b9 = 0;
	g_6061932.w10 = 0;
	SPRITE_RAM[g_60618ec] = *&g_6061932;
	g_60618ec++;
	SPRITE_RAM[g_60618ec] = *&g_6061932;
	g_60618ec++;
}

void f_2dc54(void)
{
	g_60618ec = 0;
	g_6061932.x = 0x1c0;
	g_6061932.y = 0x200;
	g_6061932.b4 = 0;
	g_6061932.b6 = 0;
	g_6061932.b5 = 63;
	g_6061932.b7 = 63;
	g_6061932.b8 = 0;
	g_6061932.b9 = 0;
	g_6061932.w10 = 3;
	SPRITE_RAM[g_60618ec] = *&g_6061932;
	g_60618ec++;
	g_6061932.x = 0;
	g_6061932.b4 = 13;
	g_6061932.b6 = 49;
	SPRITE_RAM[g_60618ec] = *&g_6061932;
	g_60618ec++;
}

void f_2dcec(struct obj *o)
{
	g_6061932.x = o->x;
	g_6061932.y = o->y;
	g_6061932.b5 = 63;
	g_6061932.b7 = 63;
	g_6061932.b4 = o->def->b4;
	g_6061932.b6 = o->def->b6 | (o->w24 << 4);
	g_6061932.b8 = o->w26;
	g_6061932.b9 = o->def->b9;
	g_6061932.w10 = o->def->w10;
	SPRITE_RAM[g_60618ec] = *&g_6061932;
	g_60618ec++;
}

void f_2dd5c(void)
{
}

void f_2dd70(struct obj *o)
{
	g_6061932.x = o->w12;
	g_6061932.y = o->w14;
	g_6061932.b5 = o->b20;
	g_6061932.b7 = o->b21;
	g_6061932.b4 = (o->def->b4 & 0x4f) | o->w16;
	g_6061932.b6 = (o->def->b6 & 0x4f) | (o->w18 << 1);
	g_6061932.b6 |= (o->w24 & 3) << 4;
	g_6061932.b8 = o->w26 ? o->w26 : o->def->b8;
	g_6061932.b9 = o->def->b9;
	g_6061932.w10 = o->def->w10;
	SPRITE_RAM[g_60618ec] = *&g_6061932;
	g_60618ec++;
}

void f_2de12(struct obj *o)
{
	g_6061932.x = o->w12 & 0x3ff;
	g_6061932.y = o->w14 & 0x3ff;
	g_6061932.b4 = o->w16;
	g_6061932.b6 = o->w18;
	g_6061932.b8 = o->w26;
	g_6061932.b9 = o->b28;
	g_6061932.w10 = o->def->w10;
	g_6061932.b5 = o->b20;
	g_6061932.b7 = o->b21;
	SPRITE_RAM[g_60618ec] = *&g_6061932;
	g_60618ec++;
}

void f_2de7e(struct obj *o)
{
	struct frame *f = &o->def[o->frame];

	if (o->w32 > 3) {
		if (o->flags & 0x8000)
			g_6061932.x = (o->x - (char)f->dx - 16) & 0x3ff;
		else
			g_6061932.x = (o->x + f->dx) & 0x3ff;
		if (o->flags & 0x80)
			g_6061932.y = (o->y - (char)f->dy - 16) & 0x3ff;
		else
			g_6061932.y = (o->y + f->dy) & 0x3ff;
		g_6061932.b4 = o->flags >> 8;
		g_6061932.b6 = o->flags | 0x10;
	} else {
		g_6061932.x = (o->x + f->dx) & 0x3ff;
		g_6061932.y = (o->y + f->dy) & 0x3ff;
		g_6061932.b4 = 0;
		g_6061932.b6 = 0x10;
	}
	g_6061932.w10 = f->w10;
	g_6061932.b8 = f->b8;
	SPRITE_RAM[g_60618ec] = *&g_6061932;
	g_60618ec++;
}

void f_2df60(struct obj *o)
{
	g_6061932.x = (o->x + o->def->dx) & 0x3ff;
	g_6061932.y = (o->y + o->def->dy) & 0x3ff;
	g_6061932.b4 = o->def->b4;
	g_6061932.b6 = o->def->b6 | 0x10;
	g_6061932.b8 = o->def->b8;
	g_6061932.b9 = o->def->b9;
	g_6061932.w10 = o->def[o->frame].w10;
	SPRITE_RAM[g_60618ec] = *&g_6061932;
	g_60618ec++;
}

void f_2dfe2(struct obj *o)
{
	g_6061932.b5 = 63;
	g_6061932.b7 = 63;
	g_6061932.x = (o->x + o->def->dx) & 0x3ff;
	g_6061932.y = (o->y + o->def->dy) & 0x3ff;
	g_6061932.b4 = o->def->b4;
	g_6061932.b6 = o->def->b6 | 0x20;
	g_6061932.b8 = o->def->b8;
	g_6061932.b9 = o->def->b9;
	g_6061932.w10 = o->def->w10;
	SPRITE_RAM[g_60618ec] = *&g_6061932;
	g_60618ec++;
}
