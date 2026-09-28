/* rom: 0x1715c len: 0x468 func: f_1715c flags: -macsave=1 -optimize=1 -speed */
struct field {
	char pad0[0x308];
	unsigned long flags308;    /* 0x308 */
	unsigned short mode;       /* 0x30c */
	char pad1[0x35c - 0x30e];
	unsigned char b35c;        /* 0x35c */
	unsigned char b35d;        /* 0x35d */
	char pad2[0x38c - 0x35e];
	short w38c;                /* 0x38c */
	char pad3[0x3b4 - 0x38e];
};
/* Sound/BGM state at 0x06066188, just below the task pool. */
struct st {
	long l0, l4, l8, lc;
	unsigned char b10;
	unsigned char b11;
	unsigned char b12;
	unsigned char b13;
	unsigned char b14;
	char pad;
	short w16[4];
	short w1e;
};
extern struct st g_6066188;
#define S g_6066188
extern short g_6060022;
extern long g_6079538;
extern unsigned char g_6079540;
extern long g_606487c;
extern unsigned char g_606619b;
extern unsigned long g_6064880;
extern struct field g_6064898[2];
extern void *g_603558c[];
extern long d_3b3c8[];
extern void (*g_6035570[])(void);
extern void f_25658(void);
extern short f_25f6e(void);
extern void f_2775c(short, int);
extern void f_277ac(short, long);
extern void f_26f64(short, int);
extern void f_2631a(short, long *, long, long);
extern long f_2beca(int);
extern void f_2af5e(int, int, void *);
extern unsigned short f_24894(void);

void f_1715c(void)
{
	int i;

	S.b10 = 1;
	S.b11 = 6;
	S.b13 = 1;
	for (i = 0; i < 4; i++)
		S.w16[i] = 0;
	f_25658();
	S.w1e = f_25f6e();
	f_2775c(S.w1e, 1);
}

void f_171b0(unsigned char a)
{
	int i;
	unsigned char t;

	if (0)
		S.b12 = t;
	for (i = 0; i < 4; i++)
		S.w16[i] = 0;
	if (a) {
		S.b13 = 2;
		return;
	}
	S.b13 = 1;
	S.w16[0] = 2;
	if (g_6060022 == 0) {
		if (g_6079538 == 5)
			S.b11 = 10;
		else if (g_6079538 == 4)
			S.b11 = f_2beca(9);
		else
			S.b11 = g_6079540;
	} else
		S.b11 = S.b12;
	t = S.b11;
	f_2af5e(0xa0, 16, g_603558c[t]);
	S.l0 = d_3b3c8[t];
	S.l4 = 1;
	S.l8 = 0;
	S.lc = 1;
	f_277ac(S.w1e, 0);
	f_26f64(S.w1e, 1);
}

void f_172c0(void)
{
	unsigned char t;

	f_277ac(S.w1e, S.w16[1]);
	S.w16[1] += 6;
	if (S.w16[1] >= 64) {
		S.w16[1] = 63;
		f_277ac(S.w1e, 63);
		S.b13 = 3;
		S.w16[0] = 2;
		if (g_6060022 == 0)
			S.b11 = g_6079540;
		else
			S.b11 = S.b12;
		t = S.b11;
		f_2af5e(0xa0, 16, g_603558c[t]);
		S.l0 = d_3b3c8[t];
		S.l4 = 1;
		S.l8 = 0;
		S.lc = 1;
		f_26f64(S.w1e, S.lc);
	}
}

void f_17394(void);

void f_1735e(void)
{
	f_277ac(S.w1e, S.w16[1]);
	S.w16[1] -= 6;
	if (S.w16[1] <= 0) {
		S.w16[1] = 0;
		f_277ac(S.w1e, 0);
		S.b13 = 1;
	}
	f_17394();
}

void f_17394(void)
{
	if (--S.w16[0] == 0) {
		S.b14 = (S.b14 + 1) % 32;
		S.w16[0] = 2;
		f_2631a(S.w1e, &S.l0, -(S.b14 / 4 * 240), -(S.b14 % 4 * 320));
		f_26f64(S.w1e, 1);
	}
}

void f_17446(void)
{
	char a, b;

	if (S.b10 & 0x80)
		return;
	if (f_24894() >= 40)
		return;
	if (S.b10 & 1)
		f_171b0(0);
	else if (g_606487c == 0 && g_606619b == 1 && g_6060022 != 2 && !(g_6064880 & 2)) {
		b = a = -1;
		if (0)
			b = 0x1000;
		if ((g_6064898[0].flags308 & 0x1000) && g_6064898[0].b35d != 7) {
			b = g_6064898[0].w38c;
			if ((g_6064898[0].mode & 0x1000) && !(g_6064898[0].flags308 & 1)) {
				if (b > 9)
					b = 9;
			}
		}
		if ((g_6064898[1].flags308 & 0x1000) && g_6064898[1].b35d != 7) {
			a = g_6064898[1].w38c;
			if ((g_6064898[1].mode & 0x1000) && !(g_6064898[1].flags308 & 1)) {
				if (a > 9)
					a = 9;
			}
		}
		if (b > -1 || a > -1) {
			if (b < a)
				b = a;
			if (S.b11 != b) {
				S.b12 = b;
				f_171b0(1);
			}
			if (g_6079540 < b)
				g_6079540 = b;
		}
	}
	S.b10 &= 0xf8;
	if (S.b13)
		g_6035570[S.b13]();
}
