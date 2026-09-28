/* rom: 0xe750 len: 0xd8 func: f_e750 flags: -macsave=1 -optimize=1 -speed */
extern void f_1865e(long, short, short, short, short, short, short, short);
extern short g_6033014[];
extern long d_3ab1c[];

void f_e750(short x, int y, char *s, int big)
{
	long *font;
	short *w;
	short cx = x;

	while (*s) {
		if (*s == '\n') {
			cx = x;
			y += 12;
			s++;
		} else if (*s == ' ') {
			cx += 5;
			s++;
		} else {
			font = d_3ab1c;
			if (big) {
				f_1865e(font[*s], y, cx, 0x9f, 124, 63, 63, 0);
				cx += g_6033014[*s++];
			} else {
				f_1865e(font[*s], y, cx, 0x9f, 125, 95, 95, 0);
				w = g_6033014;
				cx += w[*s++] * 3 / 2;
			}
		}
	}
}
