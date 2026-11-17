/* rom: 0x26ff0 len: 0x320 func: f_26ff0 flags: -macsave=1 -optimize=1 -speed */
/* difftest-nonreturning: f_2722c (with a zero-length animation strip, `while (*row < 0) *row += l->l100;` never ends, in the original too) */
/* Non-matching, functionally equivalent C (not a matching decompilation).
 * f_26ff0: redraws the whole 32x16-tile layer bank k of field i's current
 * record from its animation strip, then wraps the pending scroll counters
 * modulo 16.
 * f_2722c: finds the animation entry holding source row y of field i's
 * current record (wrapping negative rows by the strip length), storing the
 * row within the entry and the rows remaining below it.
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

struct animent *f_2722c(long i, short y, short *row, short *remain);

/* The caller passes the field index and source row at full register width. */
typedef struct animent *(*find_fn)(long, long, short *, short *);
#define FIND ((find_fn)f_2722c)

void f_26ff0(long i, long cur, long k)
{
	struct field *f = REC(i);
	struct rec104 *l;
	struct animent *ae;
	short row, remain;
	long sx, sy, px, py, yy, xx, ix, dx, m;
	char *dest;

	remain = 0;
	if (f->a24[(short)cur] == -1)
		return;
	l = (struct rec104 *)((char *)g_60ad228 + (short)(f->a24[(short)cur] * 104));
	l->w86 = 1;
	sx = (-l->l24[(short)k] - 16) >> 4;
	sy = (-l->l32[(short)k] - 16) >> 4;
	row = 0;
	px = (short)(((-l->l24[(short)k] - 16) >> 4) + (-l->l76 >> 4));
	py = ((-l->l40[(short)k] - 16) >> 4) + (-l->l80 >> 4);
	for (yy = 0; yy < 32; yy++, py++) {
		dest = (char *)l->l4[(short)k] + ((sy + yy) & 31) * 4;
		ae = FIND(i, py, &row, &remain);
		if (ae->a->flags & 0x100000) {
			long *data = (long *)(ae->a + 1) + row * ae->a->w4;
			ix = sx;
			dx = px;
			for (xx = 0; xx < 16; xx++, ix++, dx++) {
				m = dx % ae->a->w4;
				if (m < 0)
					m += ae->a->w4;
				*(long *)(dest + ((ix & 15) << 7)) = data[m];
			}
		} else {
			unsigned short *data = (unsigned short *)(ae->a + 1) + row * ae->a->w4;
			ix = sx;
			dx = px;
			for (xx = 0; xx < 16; xx++, ix++, dx++) {
				m = dx % ae->a->w4;
				if (m < 0)
					m += ae->a->w4;
				*(long *)(dest + ((ix & 15) << 7)) = ae->a->flags | data[m];
			}
		}
	}
	l->w56[(short)k] %= 16;
	l->w60[(short)k] %= 16;
}

struct animent *f_2722c(long i, short y, short *row, short *remain)
{
	struct field *f = REC(i);
	struct rec104 *l;
	struct animent *ae, *first;
	unsigned short id;

	id = f->a24[(unsigned short)f->w6];
	l = (struct rec104 *)((char *)g_60ad228 + (short)(id * 104));
	ae = l->ae;
	first = ae;
	if (ae->a) {
		*row = y;
		while (*row < 0) {
			if (l->l96) {
				*row += l->l100;
				ae = (struct animent *)l->l96;
				first = ae;
			} else {
				*row += l->l20;
			}
		}
		goto test;
		do {
			*row -= ae->a->w6 * (short)ae->n;
			ae++;
			if (!ae->a) {
				if (l->l96)
					ae = (struct animent *)l->l96;
				else
					ae = first;
			}
test:
			;
		} while (ae->a->w6 * ae->n <= *row);
		*remain = ae->a->w6 - *row - 1;
		*row %= ae->a->w6;
	}
	return ae;
}
