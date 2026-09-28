/* rom: 0x2bc64 len: 0x1e4 func: f_2bc64 flags: -macsave=1 -optimize=1 -speed */
/* Angle helpers. Angles are bytes, 0x100 to a full turn, with 0 along +y and
 * 0x40 along +x. f_2bc64 gives the angle of (x, y) from a linear
 * approximation within each octant; f_2bd48 gives it by binary search of the
 * short table g_60357e4 for 1024 * minor / major. The quarter-wave table
 * lookups at 0x2be48 and 0x2be88 (sine and cosine, by their symmetry) match
 * after these in the same file. This is probably a file start: f_2bc64
 * matches from an empty file but not after the frame functions.
 * Matching notes: the early returns need their explicit `else`, and
 * `sign = -1` has to be set in each quadrant, not once up front, for the
 * register allocation to come out right. */
int f_2bc64(int x, int y)
{
	int x32, y32;
	unsigned char r;

	if (x == 0) {
		if (y >= 0)
			return 0;
		else
			return 0x80;
	} else if (y == 0) {
		if (x >= 0)
			return 0x40;
		else
			return 0xc0;
	}
	x32 = x << 5;
	y32 = y << 5;
	if (x > 0) {
		if (y > 0) {
			if (x < y)
				r = x32 / y + 2;
			else
				r = 62 - y32 / x;
		} else {
			if (x > -y)
				r = -y32 / x + 66;
			else
				r = 126 - x32 / -y;
		}
	} else {
		if (y < 0) {
			if (x > y)
				r = x32 / y + 0x82;
			else
				r = 0xbe - y32 / x;
		} else {
			if (-x > y)
				r = y32 / -x + 0xc2;
			else
				r = 0xfe - -x32 / y;
		}
	}
	return r;
}

extern short g_60357e4[];

int f_2bd48(int x, int y)
{
	short mid = 16, lo = 0, hi = 32;
	short sign, base;
	int t, q, r;

	if (x == 0) {
		if (y >= 0)
			return 0;
		else
			return 0x80;
	} else if (y == 0) {
		if (x >= 0)
			return 0x40;
		else
			return 0xc0;
	}
	if (x > 0) {
		if (y > 0) {
			sign = -1;
			if (x < y) {
				base = 0;
				sign = 1;
			} else {
				t = x;
				base = 0x40;
				x = y;
				y = t;
			}
		} else {
			sign = -1;
			if (x > -y) {
				t = x;
				base = 0x40;
				sign = 1;
				x = -y;
				y = t;
			} else {
				base = 0x80;
				y = -y;
			}
		}
	} else {
		if (y < 0) {
			sign = -1;
			if (x > y) {
				base = 0x80;
				sign = 1;
			} else {
				base = 0xc0;
				t = x;
				x = y;
				y = t;
			}
		} else {
			sign = -1;
			if (-x > y) {
				base = 0xc0;
				sign = 1;
				t = -x;
				x = y;
				y = t;
			} else {
				x = -x;
				base = 0x100;
			}
		}
	}
	q = (x << 10) / y;
	while (lo != mid && hi != mid) {
		if (g_60357e4[mid] > q)
			hi = mid;
		else
			lo = mid;
		mid = (lo + hi) >> 1;
	}
	r = (mid + 1) * sign + base;
	return (unsigned char)r;
}
