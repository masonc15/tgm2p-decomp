/* rom: 0x2657c len: 0x9e8 func: f_2657c flags: -macsave=1 -optimize=1 -speed */
/* Non-matching, functionally equivalent C (not a matching decompilation).
 * f_2657c: per-frame board scroll update for field i: redraws the tile rows
 * (f_26e18) and columns (f_26cb0) that scrolled into view of the current
 * bank's layer, or the whole layer (f_26ff0) after a large jump, then wraps
 * the pending scroll counters modulo 16.
 * f_26cb0: writes one tile column (n+4 tiles) of a layer bank.
 * f_26e18: writes one tile row (16 tiles) of a layer bank.
 */
struct field {
	short w0;
	short w2;
	short w4;
	short w6;
	short w8;
	short w10;
	short w12;
	short w14;
	short w16;
	char pad18[2];
	long l20;
	short a24[11];
	char pad46[2];
	void (*cb48)();
	void (*cb52)();
	short a56[4];
	long l64;
	long l68;
	short w72;
	char pad74[2];
	long l76;
	long x80[224];
	long x976[224];
	char pad1872[0xe50 - 1872];
};
struct anim {
	long flags;
	short w4;
	short w6;
};
struct animent {
	struct anim *a;
	long n;
};
struct rec104 {
	short w0;
	char pad2[2];
	long *l4[2];
	short w12[2];
	struct animent *ae;
	long l20;
	long l24[2];
	long l32[2];
	long l40[2];
	short w48[2];
	short w52[2];
	short w56[2];
	short w60[2];
	short w64[2];
	short w68;
	short w70[2];
	short w74;
	long l76;
	long l80;
	short w84;
	short w86;
	short w88;
	short w90[2];
	char pad94[2];
	long l96;
	long l100;
};
extern struct field g_60ad6a0[4];
extern struct rec104 g_60ad228[11];
#define REC(i) ((struct field *)((char *)g_60ad6a0 + (short)((i) * (short)sizeof(struct field))))

struct animent *f_2722c(long, long, short *, short *);
void f_26ff0(long, long, long);
void f_26cb0(long, struct rec104 *, long, long, short);
void f_26e18(long, struct animent *, struct rec104 *, long, short, short, short, short);

/* The callers pass full-width registers/stack words; the callees narrow. */
typedef void (*row_fn)(long, struct animent *, struct rec104 *, long, long, long, long, long);
typedef void (*col_fn)(long, struct rec104 *, long, long, long);
#define ROW ((row_fn)f_26e18)
#define COL ((col_fn)f_26cb0)

void f_2657c(long i)
{
	struct field *f;
	struct rec104 *l;
	struct animent *ae;
	short w6, k;
	short px, row, remain;
	long x;
	unsigned long n, n16;

	f = REC(i);
	w6 = f->w6;
	if (f->a24[w6] < 0)
		return;
	l = (struct rec104 *)((char *)g_60ad228 + (short)(f->a24[w6] * 104));
	k = l->w84;
	px = (-l->l24[k] >> 4) + (-l->l76 >> 4);
	x = -l->l24[k] >> 4;
	n = f->w16 ? 28 : 20;
	if (n > 30)
		n = 30;
	if (l->w74 && l->w70[k] == -1) {
		long py = (-l->l32[k] - 16) >> 4;
		ae = f_2722c(i, ((-l->l40[k] - 16) >> 4) + (-l->l80 >> 4), &row, &remain);
		ROW(i, ae, l, k, px, row, py, x);
		l->w60[k] += 16;
	}
	n16 = n << 4;
	if (l->w74 && l->w70[k] == 1) {
		unsigned long py = (-l->l32[k] + n16) >> 4;
		ae = f_2722c(i, ((-l->l40[k] + n16) >> 4) + (-l->l80 >> 4), &row, &remain);
		ROW(i, ae, l, k, px, row, py, x);
		l->w60[k] -= 16;
	}
	if (l->w90[k] == 1) {
		unsigned long py = (-l->l32[k] + l->l40[k] + n16) >> 4;
		ae = f_2722c(i, (n16 >> 4) + (-l->l80 >> 4), &row, &remain);
		ROW(i, ae, l, k, px, row, py, x);
		l->w90[k] = 0;
	}
	if (l->w90[k] == -1) {
		long py = (l->l40[k] - l->l32[k] - 16) >> 4;
		ae = f_2722c(i, (-l->l80 >> 4) - 1, &row, &remain);
		ROW(i, ae, l, k, px, row, py, x);
		l->w90[k] = 0;
	}

	if (l->w60[k] < 0) {
		if ((l->w60[k] >> 4) < -20) {
			f_26ff0(i, w6, k);
		} else {
			short px2, x2, w, c;
			unsigned long py, y;
			px2 = (-l->l24[k] >> 4) + (-l->l76 >> 4);
			x2 = -l->l24[k] >> 4;
			w = l->w60[k];
			py = (-l->l32[k] + n16 + w + 16) >> 4;
			y = ((-l->l40[k] + n16 + w + 16) >> 4) + (-l->l80 >> 4);
			for (c = w >> 4; c < 0; c++, y++, py++) {
				ae = f_2722c(i, y, &row, &remain);
				ROW(i, ae, l, k, px2, row, py, x2);
			}
		}
		l->w60[k] %= 16;
	}
	if (l->w60[k] > 0) {
		if ((l->w60[k] >> 4) > 20) {
			f_26ff0(i, w6, k);
		} else {
			short px2, x2, c;
			long w, py, y;
			px2 = (-l->l24[k] >> 4) + (-l->l76 >> 4);
			x2 = -l->l24[k] >> 4;
			w = l->w60[k];
			py = ((-l->l32[k] + w) >> 4) - 2;
			y = ((-l->l40[k] + w) >> 4) + (-l->l80 >> 4) - 2;
			for (c = w >> 4; c > 0; c--, py--, y--) {
				ae = f_2722c(i, y, &row, &remain);
				ROW(i, ae, l, k, px2, row, py, x2);
			}
		}
		l->w60[k] %= 16;
	}

	if (l->w68 && l->w64[k] == -1) {
		COL(i, l, k, (-l->l24[k] >> 4) + (-l->l76 >> 4) - 1, (-l->l24[k] >> 4) - 1);
		l->w56[k] += 16;
	}
	if (l->w68 && l->w64[k] == 1) {
		COL(i, l, k, ((-l->l24[k] + 224) >> 4) + (-l->l76 >> 4), (-l->l24[k] + 224) >> 4);
		l->w56[k] -= 16;
	}
	if (l->w56[k] < 0) {
		if ((l->w56[k] >> 4) < -14) {
			f_26ff0(i, w6, k);
		} else {
			short w, cx, cc, c;
			w = l->w56[k];
			cx = ((-l->l24[k] + w + 224) >> 4) + (-l->l76 >> 4) + 1;
			cc = ((-l->l24[k] + w + 224) >> 4) + 1;
			for (c = w / 16; c < 0; c++, cx++, cc++)
				COL(i, l, k, cx, cc);
		}
		l->w56[k] %= 16;
	}
	if (l->w56[k] > 0) {
		if ((l->w56[k] >> 4) > 14) {
			f_26ff0(i, w6, k);
		} else {
			short w, c;
			long cx, cc;
			w = l->w56[k];
			cx = ((-l->l24[k] + w) >> 4) + (-l->l76 >> 4) - 2;
			cc = ((-l->l24[k] + w) >> 4) - 2;
			for (c = w / 16; c > 0; c--, cx--, cc--)
				COL(i, l, k, cx, cc);
		}
		l->w56[k] %= 16;
	}
}

void f_26cb0(long i, struct rec104 *l, long k, long x, short col)
{
	struct animent *ae;
	char *base;
	long r, y, j, n, m, cx;
	short row, remain;

	l->w86 = 1;
	r = (-l->l32[(short)k] - 32) >> 4;
	y = ((-l->l40[(short)k] - 32) >> 4) + (-l->l80 >> 4);
	n = REC(i)->w16 ? 28 : 20;
	if (n > 28)
		n = 28;
	base = (char *)l->l4[(short)k] + ((col & 15) << 7);
	cx = (short)x;
	for (j = -2; j < n + 2; j++, r++, y++) {
		ae = f_2722c(i, y, &row, &remain);
		if (ae->a->flags & 0x100000) {
			long *data = (long *)(ae->a + 1) + row * ae->a->w4;
			m = cx % ae->a->w4;
			if (m < 0)
				m += ae->a->w4;
			*(long *)(base + (r & 31) * 4) = data[m];
		} else {
			unsigned short *data = (unsigned short *)(ae->a + 1) + row * ae->a->w4;
			m = cx % ae->a->w4;
			if (m < 0)
				m += ae->a->w4;
			*(long *)(base + (r & 31) * 4) = ae->a->flags | data[m];
		}
	}
}

void f_26e18(long i, struct animent *ae, struct rec104 *l, long k, short px, short row, short py, short x)
{
	char *base;
	short n;
	long j, c, s, m;

	l->w86 = 1;
	base = (char *)l->l4[(short)k];
	n = REC(i)->w16 ? 16 : 14;
	if (n > 14)
		n = 14;
	base += (py & 31) * 4;
	c = x - 1;
	s = px - 1;
	if (ae->a->flags & 0x100000) {
		long *data = (long *)(ae->a + 1) + row * ae->a->w4;
		for (j = -1; j < n + 1; j++, c++, s++) {
			m = s % ae->a->w4;
			if (m < 0)
				m += ae->a->w4;
			*(long *)(base + ((c & 15) << 7)) = data[m];
		}
	} else {
		unsigned short *data = (unsigned short *)(ae->a + 1) + row * ae->a->w4;
		for (j = -1; j < n + 1; j++, c++, s++) {
			m = s % ae->a->w4;
			if (m < 0)
				m += ae->a->w4;
			*(long *)(base + ((c & 15) << 7)) = ae->a->flags | data[m];
		}
	}
}
