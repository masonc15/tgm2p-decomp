/* rom: 0x9b90 len: 0x7c func: main_loop flags: -macsave=1 -optimize=1 -speed */
/* The game's top-level loop: wait for the frame, then run one step of the
 * current top-level state, whose handler returns the next state. */
extern void f_2b4e8(void);
extern long f_2beca(int);
extern short f_14b10(void);
extern short f_87b8(void);
extern short f_a226(void);
extern long g_60657a0;
extern short g_6060022;                 /* top-level state */

void main_loop(void)
{
	for (;;) {
		f_2b4e8();
		g_60657a0 += f_2beca(0x4a8) + 1;
		switch (g_6060022) {
		case 0:
			g_6060022 = f_14b10();
			break;
		case 1:
			g_6060022 = f_87b8();
			break;
		case 2:
			g_6060022 = f_a226();
		default:
			g_6060022 = 0;
			break;
		}
	}
}
