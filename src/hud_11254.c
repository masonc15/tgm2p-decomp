/* rom: 0x11254 len: 0x2c4 func: f_11254 flags: -macsave=1 -optimize=1 -speed */
/* Digit drawing routines at the end of the HUD source file (0xf958-0x11518): f_11254 and f_11424 draw
 * with f_1159c (8 and 16 pixel digit spacing), f_11336 with f_185f0.  mode 2 keeps the spacing for
 * suppressed leading zeros. */
struct frame { char pad[12]; };
extern long d_3ad44[];
extern struct frame *d_3adbc[];
extern void f_185f0(struct frame *, short, short, char, short);
extern void f_1159c(long, short, short, unsigned char, short);

void f_11254(long val, short a, int x, char c, short e, short digits, long z, unsigned char mode)
{
	long div;
	int d;
	int n;
	long fr;

	n = digits;
	div = 1;
	for (digits--; digits; digits--)
		div *= 10;
	while (n) {
		d = val / div;
		val -= div * d;
		if (d > 9)
			d %= 10;
		if (d == 0) {
			if (z || n == 1)
				fr = (long)d_3adbc[0];
			else
				fr = 0;
		} else {
			fr = (long)d_3adbc[d];
			z = 1;
		}
		if (fr) {
			f_1159c(fr, a, x, c, e);
			x += 8;
		} else if (mode == 2)
			x += 8;
		n--;
		div /= 10;
	}
}

void f_11336(long val, short a, int x, short digits, long z, unsigned char mode)
{
	long div;
	int d;
	int n;
	struct frame *fr;

	n = digits;
	div = 1;
	for (digits--; digits; digits--)
		div *= 10;
	while (n) {
		d = val / div;
		val -= div * d;
		if (d > 9)
			d %= 10;
		if (d == 0) {
			if (z || n == 1)
				fr = d_3adbc[0];
			else
				fr = 0;
		} else {
			fr = d_3adbc[d];
			z = 1;
		}
		if (fr) {
			f_185f0(fr, a, x, 0, 40);
			x += 8;
		} else if (mode == 2)
			x += 8;
		n--;
		div /= 10;
	}
}

void f_11424(long val, short a, int x, char c, short e, short digits, long z, unsigned char mode)
{
	long div;
	int d;
	int n;
	long fr;

	n = digits;
	div = 1;
	for (digits--; digits; digits--)
		div *= 10;
	while (n) {
		d = val / div;
		val -= div * d;
		if (d > 9)
			d %= 10;
		if (d == 0) {
			if (z || n == 1)
				fr = d_3ad44[0];
			else
				fr = 0;
		} else {
			fr = d_3ad44[d];
			z = 1;
		}
		if (fr) {
			f_1159c(fr, a, x, c, e);
			x += 16;
		} else if (mode == 2)
			x += 16;
		n--;
		div /= 10;
	}
}
