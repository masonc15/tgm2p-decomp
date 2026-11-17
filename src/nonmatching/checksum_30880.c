/* rom: 0x30880 len: 0x1c4 func: f_30880 flags: -macsave=1 -optimize=1 -speed */
/* difftest-nonreturning: f_30880 (it busy-waits on the sound chip status bit and steps through 0x400000 sound ROM reads; random inputs make both endless or longer than the step limit) */
/* Non-matching: functionally equivalent C for [0x30880, 0x30a44) (f_30880),
 * linked only by the shiftable build (splits_nonmatching.txt). Not matching coverage.
 * From tools/shc_probe/cases/wip/chk_30880.c. */
extern char d_20040040[];
struct chk {
	unsigned short flags;
	short w2;
	short w4;
	short w6;
};
#define ROM_TABLE (*(struct chk **)((long)d_20040040))
#define OPL_STAT (*(volatile unsigned char *)0x23100000)
#define OPL_REG (*(volatile unsigned char *)0x23100004)
#define OPL_DATA (*(volatile unsigned char *)0x23100005)
extern struct chk *g_60b19a0;
extern struct chk *g_60b19a4;
extern unsigned long g_60b19a8;
extern long g_60b19ac;
extern struct chk g_60b1880[];
extern short g_6035b28[];
extern char g_6035b48[], g_6035b50[], g_6035b54[], g_6035b58[], g_6035b5c[];
extern long d_3b9fc[];
void f_2f3ec(void);
void f_2c3d0(int);
void f_2c3d8(int);
void f_e490(int, int, char *, int, int);
void f_1865e(long, int, int, int, int, int, int, int);
void f_30c2e(long v, short x, short y, short n, int lz, unsigned char mode, unsigned short color, unsigned char pri);

int f_30880(void)
{
	struct chk *p;
	struct chk *q;
	unsigned long *r;
	unsigned long n;
	unsigned short i;
	unsigned long v;
	long j;
	long k;
	unsigned long lo, hi;
	int ok;
	unsigned char c;

	if (g_60b19a0->flags != 0) {
		if (g_60b19a0->flags & 0x8000) {
			f_2f3ec();
			OPL_REG = 2;
			f_2f3ec();
			OPL_DATA = 1;
			f_2f3ec();
			OPL_REG = 3;
			f_2f3ec();
			OPL_DATA = 0;
			f_2f3ec();
			OPL_REG = 4;
			f_2f3ec();
			OPL_DATA = 0;
			f_2f3ec();
			OPL_REG = 5;
			f_2f3ec();
			OPL_DATA = 0;
			f_2f3ec();
			OPL_REG = 6;
			g_60b19a4->w6 = 0;
			for (k = 0x400000; k != 0; k--) {
				while (OPL_STAT & 1)
					;
				g_60b19a4->w6 += OPL_DATA;
			}
			f_2f3ec();
			OPL_REG = 2;
			f_2f3ec();
			OPL_DATA = 0;
		} else {
			hi = 0;
			lo = 0;
			n = ((unsigned long)g_60b19a0->flags << 20) / 8 / 0x20000;
			for (i = 0; i < n; i++) {
				r = (unsigned long *)0x24060000;
				if (g_60b19a8 & 0x100)
					*(volatile unsigned char *)0x2405fff2 = ((g_60b19a8 & 0x300) >> 8) | 0x30;
				*(volatile unsigned char *)0x2405fff3 = g_60b19a8++;
				for (j = 0; j < 0x8000; j++) {
					v = *r++;
					hi += v >> 16;
					lo += v;
				}
			}
			g_60b19a4->w6 = lo + hi;
			g_60b19a4->w4 = hi;
			g_60b19a4->w2 = lo;
		}
		g_60b19a4++;
		g_60b19a0++;
	} else {
		p = ROM_TABLE;
		q = g_60b1880;
		ok = 1;
		while (p->flags) {
			if (p->flags & 0x8000) {
				if (p->w6 != q->w6)
					ok = 0;
			} else if (p->w2 != q->w2 || p->w4 != q->w4 || p->w6 != q->w6)
				ok = 0;
			p++;
			q++;
		}
		return ok;
	}
	return 1;
}

int f_30a44(void);

void f_30c2e(long v, short x, short y, short n, int lz, unsigned char mode, unsigned short color, unsigned char pri);
