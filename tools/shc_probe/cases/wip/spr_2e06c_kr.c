/* rom: 0x2e06c len: 0x14c func: sprite_add_box */
/* Sprite-list helpers: fill a 16-byte sprite command, then append it to
 * sprite RAM (0x24000000 = cache-through mirror of 0x04000000). */
struct sprite {
	short x;                   /* 0 */
	short y;                   /* 2 */
	char b4, b5, b6, b7, b8, b9;
	short w10;
	short w12;
	short w14;
};
struct obj {
	char pad0[12];
	short w12, w14, w16, w18;  /* 0x0c..0x12 */
	char b20, b21;             /* 0x14, 0x15 */
	char pad1[4];
	short w26;                 /* 0x1a */
	char b28;                  /* 0x1c */
	char pad2;
	short w30;                 /* 0x1e */
};
extern struct sprite spr_cmd;              /* 0x6061932 */
extern unsigned short spr_count;           /* 0x60618ec */
extern void (*tbl_603595c[])(long);
#define SPRITE_RAM ((struct sprite *)0x24000000)

void sprite_add_box(void)                  /* 0x2e06c */
{
	spr_cmd.x = 200;
	spr_cmd.y = 10;
	spr_cmd.b4 = 0;
	spr_cmd.b5 = 63;
	spr_cmd.b6 = 48;
	spr_cmd.b7 = 63;
	spr_cmd.b8 = 0;
	spr_cmd.b9 = 0;
	spr_cmd.w10 = 31;
	SPRITE_RAM[spr_count] = spr_cmd;
	spr_count++;
}

void sprite_add_obj15(struct obj *o)       /* 0x2e0cc */
{
	spr_cmd.x = o->w12;
	spr_cmd.y = o->w14;
	spr_cmd.b4 = o->w16;
	spr_cmd.b6 = o->w18;
	spr_cmd.b5 = 63;
	spr_cmd.b7 = 63;
	spr_cmd.b8 = o->w26;
	spr_cmd.b9 = 0;
	spr_cmd.w10 = 15;
	SPRITE_RAM[spr_count] = spr_cmd;
	spr_count++;
}

void sprite_add_obj(struct obj *o)         /* 0x2e128 */
{
	spr_cmd.x = o->w12;
	spr_cmd.y = o->w14;
	spr_cmd.b4 = o->w16;
	spr_cmd.b6 = o->w18;
	spr_cmd.b5 = o->b20;
	spr_cmd.b7 = o->b21;
	spr_cmd.b8 = o->w26;
	spr_cmd.b9 = o->b28;
	spr_cmd.w10 = o->w30;
	SPRITE_RAM[spr_count] = spr_cmd;
	spr_count++;
}

void sprite_dispatch(a, b)                 /* 0x2e18c */
short a;
long b;
{
	(*tbl_603595c[(unsigned char)a])(b);
}
