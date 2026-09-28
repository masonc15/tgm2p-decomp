/* rom: 0xe750 len: 0xd8 func: draw_text flags: -macsave=1 -optimize=1 -speed */
/* Draws a NUL-terminated string one glyph sprite at a time. A newline returns
 * to the starting x and moves down 12 pixels, and a space advances 5. Glyph
 * graphics come from the table at d_3ab1c and advance widths from g_6033014.
 * With unscaled clear the glyphs are drawn with zoom 95 instead of 63 and
 * advance by 1.5 times their width, which is the same 3:2 ratio. */
extern void f_1865e(long, short, short, short, short, short, short, short);
extern short g_6033014[];
extern long d_3ab1c[];

void draw_text(short x, int y, char *s, int unscaled)
{
	long *font;
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
			if (unscaled) {
				f_1865e(font[*s], y, cx, 0x9f, 124, 63, 63, 0);
				cx += g_6033014[*s++];
			} else {
				f_1865e(font[*s], y, cx, 0x9f, 125, 95, 95, 0);
				cx += (g_6033014[*s] * 3) / 2;
				s++;
			}
		}
	}
}
