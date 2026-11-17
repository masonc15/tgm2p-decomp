/* rom: 0xe3e0 len: 0x140 func: f_e3e0 flags: -macsave=1 -optimize=1 -speed */
/* Non-matching: functionally equivalent C for [0xe3e0, 0xe520), linked only
 * by the shiftable build (splits_nonmatching.txt). Not matching coverage.
 * Text helpers: draw a string centred on the screen, and draw a string at
 * (x, y) in palette 81 or a given palette, one glyph frame per character
 * from the font table d_3ab1c, advancing by the width table g_6033014. */
extern long d_3ab1c[];
extern short g_6033014[];
extern short f_e39c(char *s);
extern void f_1865e(long, short, short, short, short, short, short, char);

void f_e412(short x, short y, char *s, char flag);

void f_e3e0(short y, char *s, char flag)
{
	f_e412((320 - f_e39c(s)) / 2, y, s, flag);
}

void f_e412(short x, short y, char *s, char flag)
{
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
			f_1865e(d_3ab1c[*s], y, cx, 81, 125, 63, 63, flag);
			cx += g_6033014[*s];
			s++;
		}
	}
}

void f_e490(short x, short y, char *s, short w, char flag)
{
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
			f_1865e(d_3ab1c[*s], y, cx, w, 124, 63, 63, flag);
			cx += g_6033014[*s];
			s++;
		}
	}
}
