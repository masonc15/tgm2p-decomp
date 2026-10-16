/* rom: 0x20a9c len: 0x88 func: f_20a9c flags: -macsave=1 -optimize=1 -speed */
/* Non-matching: functionally equivalent C for [0x20a9c, 0x20b24) (f_20a9c),
 * linked only by the shiftable build (splits_nonmatching.txt). Not matching coverage.
 * From src/effect_209a4.c. */
struct frame { char pad[12]; };
struct sprite {
	struct frame *frames;      /* 0x18 */
	short w4;                  /* 0x1c */
	short y;                   /* 0x1e */
	short x;                   /* 0x20 */
};
struct task {
	char pad0[6];
	short tick;                /* 0x06 */
	short n;                   /* 0x08 */
	char pad1[16 - 10];
	void (*func)(struct task *); /* 0x10 */
	struct field *owner;       /* 0x14 */
	struct sprite spr;         /* 0x18 */
};
struct point { short x, y; };
struct field {
	char pad0[0xde];
	unsigned char height;      /* 0x0de */
	unsigned char width;       /* 0x0df */
	struct point pos;          /* 0x0e0 */
	char pad1[0x314 - 0xe4];
	struct point shake;        /* 0x314 */
};
extern struct task *f_17614(void);
extern void f_17638(struct task *);
extern void f_1159c(struct frame *, int, int, int, int);
extern unsigned short g_6060000;
extern struct frame d_a887c[], d_a8bdc[];

void f_20a32(struct task *t);

void f_209a4(struct field *f, short y);

void f_20a32(struct task *t);

void f_20a9c(struct field *f, short x)
{
	struct point *d = &f->shake;
	short sx;
	int sy;

	sx = x * 8 + d->x - (f->width / 2) * 8;
	sy = (f->height - 20 - 1) * 8 + d->y; sy -= (f->height - 1) * 8; sy -= 6;
	f_1159c(d_a8bdc, sy, sx, 0xba, 115);
}
