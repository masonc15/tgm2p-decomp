/* rom: 0x8c96 len: 0x3e6 func: f_8c96 flags: -macsave=1 -optimize=1 -speed */
/* Non-matching, functionally equivalent C for ROM 0x8c96-0x907c (f_8c96):
 * VS-mode round judge: decide the round winner from level / time, count
 * round wins, then after the result pause pick the next round or match end. */
struct field {
	char pad0[0x2f4];
	struct field *other;       /* 0x2f4 */
	char pad2f8[0x304 - 0x2f8];
	short w304;                /* 0x304 */
	char pad306[2];
	unsigned long l308;        /* 0x308 */
	unsigned short w30c;       /* 0x30c */
	unsigned char b30e;        /* 0x30e */
	char pad30f[0x322 - 0x30f];
	unsigned short w322;       /* 0x322 */
	char pad324[0x350 - 0x324];
	unsigned long l350;        /* 0x350 */
	char pad354[0x35c - 0x354];
	short w35c;                /* 0x35c */
	char pad35e[0x3b4 - 0x35e];
};

struct gs {
	unsigned char b0;          /* 0x6064884 */
	char pad1;
	short w2;
	unsigned char b4[2];
	unsigned char b6;
	unsigned char b7;
	unsigned char b8;
	char pad9;
	unsigned short w10[2];     /* 0x606488e */
	unsigned short w14;        /* 0x6064892 */
};

struct snd {
	char pad0[0x10];
	unsigned char b10;
	char pad11;
	unsigned char b12;
};

struct hs {
	unsigned long l0;
	unsigned long l4;
};

extern struct field g_6064898[2];
extern struct gs g_6064884;
extern struct snd g_6066188;
extern unsigned long g_6064880;
extern long g_6064750;
extern long g_6064878;
extern unsigned long g_6060008;
extern long g_6079538;
extern long g_606487c;
extern short g_6079294;
extern unsigned short g_6079290;
extern unsigned short g_6079292;
extern unsigned char g_6079296;
extern unsigned char g_6079297;
extern signed char g_6079298;
extern signed char g_6079299;
extern unsigned char g_607929a;
extern unsigned char g_606475a[2];
extern unsigned char g_606475e[2];
extern unsigned char g_606488a;
extern short g_6060036;
extern short g_6060038;
extern unsigned char d_3a8cc[];
extern unsigned char d_3a8cf[][2];
extern short d_36314[];
extern unsigned long g_6065750[];
extern struct hs g_6065698[];
extern unsigned short g_6066184;
extern short g_60661a6;
extern long g_60657a0;
extern char g_6032884[], g_60328c4[], g_6033748[], g_6032904[];

extern void f_2b1cc(void);
extern void f_29f94(int, int, int, int);
struct task;
extern void f_247b0(struct task *);
extern void f_2491c(void);
extern void f_2b31e(void);
extern void f_2ab56(int);
extern void f_2ed3e(void);
extern void f_1278(void);
extern void f_19074(void);
extern void f_2af5e(int, int, long);
extern void f_18fec(void);
extern int f_2b4e8(void);
extern void f_2ecea(void);
extern void f_b2f4(void);
extern void f_331a(struct field *);
extern void f_29cc6(int, int, int, int);
extern void f_20250(void);
extern void f_3510(struct field *);
extern int f_2b816(void);
extern void f_29f70(void);
extern void f_247ee(void);
extern void f_2f2cc(void);
extern void f_2f3dc(void);
extern void f_2ee38(void);
extern void f_2ed06(void);
extern void f_2eeaa(int);
extern void f_175c4(void);
extern void f_1340(unsigned char);
extern void f_1715c(void);
extern void f_2774a(short, int);
extern void f_2a576(int, char *, long, int, int, int, int);
extern long f_2beca(int);
extern void f_e750(int, int, char *, int);
extern short f_149b4(void);
extern void f_1326c(void);
extern void f_2e5aa(int);


int f_881a(void);
int f_9180(void);
short f_8c96(void);
void f_96ac(void);

void f_85d8(void);

short f_87b8(void);

int f_881a(void);

void f_88b4(struct field *f);

short f_88fe(void);

void f_8b20(void);

unsigned char f_8b7c(void);

short f_8b86(int id);

#define FLD(i) ((struct field *)((char *)g_6064898 + (short)((i) * 0x3b4)))
short f_8c96(void)
{
	short r = 0;
	unsigned short w1 = 0;
	unsigned short w2 = 0;
	unsigned char cur = g_6064884.b8;
	unsigned char oth = cur == 0;
	short lim = d_36314[g_6060038];
	unsigned short s0, s1;
	unsigned char need;
	unsigned short k;

	switch (g_6064884.b0) {
	case 0:
		if (g_6064880 & 0x300)
			goto judged;
		g_6064880 &= 0xfff3ffff;
		if ((g_6064898[0].w322 >= (unsigned short)lim) | (g_6064898[1].w322 >= (unsigned short)lim)) {
			s0 = FLD(cur)->w322;
			s1 = FLD(oth)->w322;
			if (g_6064898[0].w322 >= (unsigned short)lim)
				g_6064898[0].w322 = lim;
			if (g_6064898[1].w322 >= (unsigned short)lim)
				g_6064898[1].w322 = lim;
			if (s0 >= s1)
				g_6064880 |= cur == 0 ? 0x100 : 0x200;
			else
				g_6064880 |= oth == 0 ? 0x100 : 0x200;
		} else if (!(g_6064898[0].l350 > 0)) {
			s1 = FLD(cur)->w322;
			s0 = FLD(oth)->w322;
			if (s1 >= s0)
				g_6064880 |= cur == 0 ? 0x100 : 0x200;
			else
				g_6064880 |= oth == 0 ? 0x100 : 0x200;
			if (s1 == s0)
				g_6064880 |= 0x80000;
			else
				g_6064880 |= 0x40000;
		}
	judged:
		if (!(g_6064880 & 0x300))
			break;
		g_6064884.b0 = 1;
		g_6064884.w2 = 300;
		if (g_6064880 & 0x100)
			g_6064884.b4[0]++;
		if (g_6064880 & 0x200)
			g_6064884.b4[1]++;
		f_3510(&g_6064898[0]);
		f_3510(&g_6064898[1]);
		f_19074();
		if (!(g_6064880 & 0x100))
			break;
		if (!(g_6064880 & 0x200))
			break;
		g_6064884.w2 = 120;
		g_6064880 &= ~0x300;
		g_6064880 |= 0x400;
		g_6064880 &= 0xfff3ffff;
		break;
	case 1:
		if (((g_606475e[0] & 1) || (g_606475e[1] & 1)) && g_6064884.w2 > 10)
			g_6064884.w2 = 10;
		if (--g_6064884.w2 != 0)
			break;
		need = d_3a8cc[g_6060036];
		if (g_6064884.b4[cur] >= need)
			w1 = 1;
		need = d_3a8cc[g_6060036];
		if (g_6064884.b4[oth] >= need)
			w2 = 1;
		if (g_6064880 & 0x400) {
			if (w1 || w2) {
				g_6064880 &= ~0x400;
				if (w1)
					g_6064880 |= cur == 0 ? 0x100 : 0x200;
				else
					g_6064880 |= cur == 0 ? 0x200 : 0x100;
				g_6064884.w2 = 300;
			} else {
				g_6064880 |= 0x800;
				g_6064880 |= 0x2000;
			}
			break;
		}
		g_6064880 |= 0x800;
		for (k = 0; k < 10; k++)
			g_6065750[k] = g_6065698[k].l0 & 0xfffff;
		if (w1 || w2) {
			if (w1) {
				if (g_6064884.b7 < 99)
					g_6064884.b7++;
				else
					g_6064884.b7 = 99;
				FLD(cur)->w30c = g_6064884.w10[cur];
				r = f_8b86(oth);
			} else {
				g_6064884.b7 = 1;
				g_6064884.b8 = oth;
				FLD(oth)->w30c = g_6064884.w10[cur];
				r = f_8b86((signed char)cur);
			}
		} else {
			g_6064880 |= 0x800;
			g_6064880 |= 0x2000;
		}
		break;
	}
	return r;
}

void f_907c(void);

short f_9098(void);

int f_9180(void);

short f_95b8(short x);

short f_9648(short x);

short f_966c(short x);

void f_96ac(void);
