/* flags: -macsave=1 -optimize=1 -speed */
/* Example mod: new code, a new variable and a new initialized string.
 * mod_frame() is called from main_loop() once per frame and draws a banner
 * with a frame counter in the top left corner of every screen. */
extern void draw_text(short x, int y, char *s, int unscaled);

unsigned long mod_frames;               /* new variable, cleared at boot */
static char banner[] = "MOD 00000000";  /* new initialized data, in RAM */

void mod_frame(void)
{
	static const char hex[] = "0123456789ABCDEF";
	unsigned long v = mod_frames++;
	short i;

	for (i = 11; i >= 4; i--) {
		banner[i] = hex[v & 15];
		v >>= 4;
	}
	draw_text(8, 8, banner, 1);
}
