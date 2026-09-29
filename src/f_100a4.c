/* rom: 0x100a4 len: 0x114 func: f_100a4 flags: -macsave=1 -optimize=1 -speed */
struct frame { char pad[12]; };
struct field {
	char pad0[0x314];
	short x;        /* 0x314 */
	short y;        /* 0x316 */
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
extern void f_1159c(long, long, long, long, long);
#define FIELD(p) ((struct field *)((char *)g_6064898 + (short)((p) * 0x3b4)))
#define FB(p) ((struct field *)((char *)fb + (short)((p) * 0x3b4)))
void f_100a4(unsigned char on_field)
{
	unsigned char c, p;
	short n, div, d;
	int y;
	int x;
	struct field *fb;
	c = g_6064884.count;
	if (c > 0) {
		if (p = g_6064884.player, on_field != 0) {
			fb = g_6064898;
			x = !p ? FB(p)->x + 6 : FB(p)->x - 50;
			y = FB(p)->y + 2;
		} else {
			if (p == 0)
				x = 108;
			else
				x = 268;
			y = 118;
		}
		if (0) y = 10; /* dead code: steers SHC register ranking */
		for (div = 10, n = c; div; div /= 10) {
			d = n / div;
			n -= d * div;
			if (div != 10 || d)
				f_1159c((long)&d_aac34[d], y, x, 0, 40);
			x = x + 8;
		}
		f_1159c((long)d_aacac, y, x + 4, 0, 40);
	}
}
