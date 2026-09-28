/* rom: 0x100a4 len: 0x114 func: f_100a4 flags: -macsave=1 -optimize=1 -speed */
/* WIP (11%): draws a two-digit count (digits d_aac34, then d_aacac) at a
 * player's playfield or a fixed spot. The ROM recomputes the field address
 * (short)(p * 0x3b4) in each branch instead of hoisting it, and divides with
 * the runtime helper at RAM 0x603071c (probably unsigned). */
struct frame { char pad[12]; };
struct point { short x, y; };
struct field {
	char pad0[0x314];
	struct point shake;        /* 0x314 */
	char pad1[0x3b4 - 0x318];
};
struct counter {
	char pad0[7];
	unsigned char count;       /* 0x7 */
	unsigned char player;      /* 0x8 */
};
extern struct counter g_6064884;
extern struct field g_6064898[];
extern struct frame d_aac34[], d_aacac[];
extern void f_1159c(struct frame *, int, int, int, int);

#define FIELD(p) ((struct field *)((char *)g_6064898 + (short)((p) * 0x3b4)))

void f_100a4(unsigned char on_field)
{
	unsigned char n, p;
	short x, y, div, d;

	n = g_6064884.count;
	if (n > 0) {
		p = g_6064884.player;
		if (on_field) {
			if (p == 0)
				x = FIELD(p)->shake.x + 6;
			else
				x = FIELD(p)->shake.x - 50;
			y = FIELD(p)->shake.y + 2;
		} else {
			if (p == 0)
				x = 108;
			else
				x = 268;
			y = 118;
		}
		for (div = 10; div != 0; div /= 10) {
			d = n / div;
			n -= d * div;
			if (div != 10 || d != 0)
				f_1159c(&d_aac34[d], y, x, 0, 40);
			x += 8;
		}
		f_1159c(d_aacac, y, x + 4, 0, 40);
	}
}
