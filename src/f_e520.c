/* rom: 0xe520 len: 0x230 func: f_e520 flags: -macsave=1 -optimize=1 -speed */
extern void f_1865e(long, short, short, short, short, short, short, short);
extern short g_6033014[];
extern long d_3ad1c[];
extern char d_a7c04[], d_a7c10[], d_a7bf8[], d_a7bec[], d_a7afc[], d_a7b08[], d_a7b14[], d_a7b2c[], d_a7da8[];

void f_e520(short x, short y, char *s, int h)
{
	while (*s != 0) {
		switch (*s) {
		case 'Y':
			f_1865e((long)d_a7c04, y, x, 82, 125, 63, 63, h);
			break;
		case 'N':
			f_1865e((long)d_a7c10, y, x, 82, 125, 63, 63, h);
			break;
		case 'O':
			f_1865e((long)d_a7bf8, y, x, 82, 125, 63, 63, h);
			break;
		case 'X':
			f_1865e((long)d_a7bec, y, x, 82, 125, 63, 63, h);
			break;
		case '1':
			f_1865e((long)d_a7afc, y, x, 82, 125, 63, 63, h);
			break;
		case '2':
			f_1865e((long)d_a7b08, y, x, 82, 125, 63, 63, h);
			break;
		case 'W':
			f_1865e((long)d_a7b14, y - 2, x, 82, 125, 63, 63, h);
			break;
		case '>':
			f_1865e((long)d_a7b2c, y, x, 82, 125, 63, 63, h);
			break;
		default:
			f_1865e((long)d_a7da8, y, x, 82, 125, 63, 63, h);
			break;
		}
		x += g_6033014[*s];
		s++;
	}
}

void f_e63a(int v, short y, int x, short k, long flag, unsigned char mode, char h)
{
	int div = 1;
	int n = k;
	int d;
	long g;


	for (k--; k; k--)
		div *= 10;
	while (n != 0) {
		d = v / div;
		v -= d * div;
		if (d > 9)
			d %= 10;
		if (d == 0) {
			if (flag != 0 || n == 1)
				g = d_3ad1c[0];
			else
				g = 0;
		} else {
			g = d_3ad1c[d];
			flag = 1;
		}
		if (g != 0) {
			f_1865e(g, y, x, 81, 125, 63, 63, h);
			x += 8;
		} else if (mode == 2) {
			x += 8;
		}
		n--;
		div /= 10;
	}
}
