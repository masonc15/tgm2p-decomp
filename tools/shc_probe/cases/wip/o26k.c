/* rom: 0x26f64 len: 0x8c func: f_26f64 flags: -macsave=1 -optimize=1 -speed */
struct rec_a {                     /* 0xe50 bytes, array at 0x60ad6a0 */
	char pad0[6];
	short cur;                 /* 0x06 */
	char pad1[24 - 8];
	short list[(0xe50 - 24) / 2];  /* 0x18 */
};
struct rec_b {                     /* 104 bytes, array at 0x60ad27c */
	short w0;
	char pad[102];
};
extern struct rec_a g_60ad6a0[];
extern struct rec_b g_60ad27c[];
extern short g_60b13ae;

void f_26ff0(short i, short cur, short w);

/* The ROM truncates these byte offsets to 16 bits (muls.w, exts.w). */
#define REC_A(i) ((struct rec_a *)((char *)g_60ad6a0 + (short)((i) * (short)sizeof(struct rec_a))))
#define REC_B(k) ((struct rec_b *)((char *)g_60ad27c + (short)((k) * (short)sizeof(struct rec_b))))

void f_26f64(short i)
{
	short cur = REC_A(i)->cur;
	short k = REC_A(i)->list[cur];

	if (k >= 0)
		f_26ff0(i, cur, REC_B(k)->w0);
}

void f_26faa(short i, short cur, short w, int lock)
{
	if (lock) {
		g_60b13ae = 1;
		f_26ff0(i, cur, w);
		g_60b13ae = 0;
	} else {
		f_26ff0(i, cur, w);
	}
}

void f_26ff0(short i, short cur, short w)
{
}
