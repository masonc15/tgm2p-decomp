/* rom: 0x9098 len: 0xe8 func: f_9098 context-rom: 0x8b20 flags: -macsave=1 -optimize=1 -speed */
/* Only f_9098 is retained. All other bodies are excluded compiler context.
 * The f_8c96 loop uses typed context carriers for 0x06065750, 0x000fffff, and
 * 0x06065698 so SHC emits the predecessor-owned pool in authentic ROM order.
 * f_9098 references none of those carriers; c_prefix verifies every retained byte. */
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
extern struct hs g_6065750[];
extern unsigned long d_fffff[];
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

#define SPR0 ((volatile unsigned char *)0x2405ffe0)

short f_8c96(void);
void f_96ac(void);
void f_8b20(void)
{
	struct field *a = &g_6064898[0];
	struct field *b = &g_6064898[1];

	if ((a->l308 & 2) && (b->l308 & 2)) {
		g_6064884.b0 = 0;
		g_6064880 &= ~0x4001;
		g_6064880 = (g_6064880 | 0x802) & ~0x700;
		a->l308 = 0x13004;
		b->l308 = 0x13004;
		a->l308 &= 0xfff9bfff;
		b->l308 &= 0xfff9bfff;
		f_19074();
		f_20250();
	}
}

unsigned char f_8b7c(void)
{
	return d_3a8cc[g_6060036];
}

#pragma inline(select_field)
static struct field *select_field(unsigned char *id) { return (struct field *)((char *)g_6064898 + (short)(*id * 0x3b4)); }
short f_8b86(unsigned char id)
{
	struct field *a = &g_6064898[0];
	struct field *b = &g_6064898[1];
	struct field *f = select_field(&id);

	f_331a(f);
	f->w30c &= 0x1083;
	a->w30c &= 0xfff7;
	b->w30c &= 0xfff7;
	if (!(a->l308 & 0x40))
		a->l308 |= 0x2000;
	if (!(0x40 & b->l308))
		b->l308 |= 0x2000;
	g_6064880 = g_6064880 & ~0xc6 | 1;
	g_606488a = 0;
	f_2af5e(15, 1, 0x68ad0);
	f_2af5e(14, 1, 0x68b10);
	while (g_6064750)
		if (f_2b4e8())
			return 7;
	return 0;
}

short f_8c96(void)
{
	short r = 0;
	unsigned short w1 = 0;
	unsigned short w2 = 0;
	unsigned char cur = g_6064884.b8;
	unsigned char oth = cur == 0;
	short lim = d_36314[g_6060038];
	unsigned short s0, s1;
	unsigned short need;
	unsigned short k;

	switch (g_6064884.b0) {
	case 0:
		if (g_6064880 & 0x300)
			break;
		g_6064880 &= 0xfff3ffff;
		if ((g_6064898[0].w322 >= (unsigned short)lim) | (g_6064898[1].w322 >= (unsigned short)lim)) {
			s0 = g_6064898[cur].w322;
			s1 = g_6064898[oth].w322;
			if (g_6064898[0].w322 >= (unsigned short)lim)
				g_6064898[0].w322 = lim;
			if (g_6064898[1].w322 >= (unsigned short)lim)
				g_6064898[1].w322 = lim;
			if (s0 >= s1)
				g_6064880 |= cur == 0 ? 0x100 : 0x200;
			else
				g_6064880 |= oth == 0 ? 0x100 : 0x200;
		} else if (!(g_6064898[0].l350 > 0)) {
			s1 = g_6064898[cur].w322;
			s0 = g_6064898[oth].w322;
			if (s1 >= s0)
				g_6064880 |= cur == 0 ? 0x100 : 0x200;
			else
				g_6064880 |= oth == 0 ? 0x100 : 0x200;
			if (s1 == s0)
				g_6064880 |= 0x80000;
			else
				g_6064880 |= 0x40000;
		}
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
		need = f_8b7c();
		if (g_6064884.b4[cur] >= need)
			w1 = 1;
		need = f_8b7c();
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
			d_fffff[k] = g_6065750[k].l0 & 0x06065698;
		if (w1 || w2) {
			if (w1) {
				if (g_6064884.b7 < 99) g_6064884.b7++; else g_6064884.b7 = 99;
				g_6064898[cur].w30c = g_6064884.w10[cur];
			} else {
				g_6064884.b7 = 1;
				g_6064884.b8 = oth;
				g_6064898[oth].w30c = g_6064884.w10[cur];
				oth = cur;
			}
			r = f_8b86(oth);
		} else {
			g_6064880 |= 0x800;
			g_6064880 |= 0x2000;
		}
		break;
	}
	return r;
}

void f_907c(void)
{
	g_6064880 &= ~0x2000;
	g_6064880 |= 0x4000;
	{struct byte_counter {unsigned char value;}; ((struct byte_counter *)(&g_6064884.b6))->value += 1U;}
}

short f_9098(void)
{
	switch (g_6064884.b0) {
	case 0:
		g_6064884.b0++;
		f_29cc6(2, 20, 0, 6);
	case 1:
		while (g_6064750)
			if (f_2b816())
				return 7;
		g_6064884.b0++;
		f_29cc6(0, 10, 0, 6);
	case 2:
		g_6064880 &= ~0x20000;
		g_6064898[0].l308 &= 0x7fffffff;
		g_6064898[1].l308 &= 0x7fffffff;
		g_6064898[0].l308 = 0x3004;
		g_6064898[1].l308 = 0x3004;
		g_6064898[1].w30c = 4;
		g_6064898[0].w30c = 4;
		g_6064880 = 4;
		g_6064884.b0 = 0;
	}
	return 0;
}

short f_9180(void)
{
	struct field *a, *b;
	short r = 0;
	short i;
	unsigned short blink, cnt;

	f_2b1cc();
	if (f_2b4e8())
		return 7;
	f_29cc6(4, 0, 0, 0);
	while (g_6064750)
		if (f_2b4e8())
			return 7;
	f_29f70();
	f_247ee();
	f_2f2cc();
	f_2f3dc();
	f_2ee38();
	f_2ed06();
	f_2ed3e();
	f_2eeaa(5);
	g_6064884.w14 = 0xffff;
	f_175c4();
	for (i = 0; i <= 1; i++) {
		g_6064898[i].w30c |= g_6064884.w10[i] & 0xffe0;
		if ((1 << i) & g_6066184) {
			g_6064898[i].l308 = 0x1001;
			f_1340(i);
		}
	}
	g_6064884.b8 = (g_6064898[0].l308 & 0x1000) ? 0 : 1;
	while (g_6064750)
		if (f_2b4e8())
			return 7;
	f_1715c();
	f_2774a(g_60661a6, 1);
	f_2af5e(0xca, 1, (long)g_60328c4);
	f_2a576(0x9f, g_6033748, 0x67910, 1, 64, 1, 63);
	blink = 0;
	cnt = 0;
	f_29cc6(0, 10, 0, 6);
	a = &g_6064898[0];
	b = &g_6064898[1];
	g_6060008 = 0;
	do {
		if (f_2b816()) {
			f_247ee();
			return 7;
		}
		f_96ac();
		f_e750(0x11d, 0xda, g_6032904, 1);
		cnt++;
		if (cnt % 64 == 0) {
			blink = !blink;
			if (blink)
				f_2a576(0x9f, g_6033748, 0x67910, 1, 68, 1, 63);
			else
				f_2a576(0x9f, g_6033748, 0x67910, 1, 64, 1, 63);
		}
		g_60657a0 = f_2beca(0x37e);
		if (g_6064880 & 0x20000) {
			r = f_9098();
		} else {
			if (g_6064880 & 0x10)
				f_88b4(a);
			else if (g_6064880 & 0x20)
				f_88b4(b);
			if ((g_6064880 &= ~0x30) & 0x80)
				r = f_88fe();
			if (g_6064880 & 0x4000)
				f_8b20();
			if ((g_6064880 & 2) && !(g_6064880 & 0x800))
				r = f_8c96();
			if ((g_6064880 & 2) && (g_6064880 & 0x2000))
				f_907c();
			if (!(g_6064880 & 2) && (g_6064880 & 4) && (!(a->l308 & 0x1000) || !(0x1000 & b->l308))) {
				g_6064880 &= ~0xc6;
				g_6064880 = 1;
				a->l308 |= 0x2000;
				b->l308 |= 0x2000;
				f_29cc6(0, 10, 0, 6);
			}
		}
		if ((a->l308 & 0x10000000) && (0x10000000 & b->l308)) {
			g_6079299 = 2;
			if ((unsigned short)++g_6079294 >= 180)
				r = 2;
			if (f_149b4())
				g_6079294 = 0;
		} else {
			g_6079294 = 0;
		}
	} while (r == 0);
	g_60657a0 += f_2beca(0x4a8) + 1;
	if (r == 2)
		f_29cc6(2, 30, 72, 6);
	while (g_6064750)
		if (f_2b4e8()) {
			f_247ee();
			return 7;
		}
	f_1326c();
	f_247ee();
	f_2ed06();
	if (r == 2)
		;
	return 2;
}

short f_95b8(short x)
{
	if (x < 495)
		return 1;
	if (x >= 495 && x < 500)
		return 2;
	if (x >= 500 && x < 695)
		return 3;
	if (x >= 695 && x < 700)
		return 4;
	if (x >= 700 && x < 880)
		return 5;
	if (x >= 880 && x < 900)
		return 6;
	if (x >= 900)
		return 7;
}

short f_9648(short x)
{
	if (x < 280)
		return 1;
	if (x >= 280 && x < 300)
		return 2;
	if (x >= 300)
		return 3;
}

short f_966c(short x)
{
	if (x < 280)
		return 3;
	if (x >= 280 && x < 300)
		return 2;
	if (x >= 300 && x < 480)
		return 5;
	if (x >= 480 && x < 500)
		return 2;
	if (x >= 500)
		return 7;
}

void f_96ac(void)
{
	struct field *a = &g_6064898[0];
	struct field *b = &g_6064898[1];
	short s1, s2;
	short st = a->w35c;
	unsigned long m = g_6064880 & 1;
	short v;
	signed char want;
	unsigned char vol, tgt;
	unsigned long t;
	short trk;

	if (!(a->l308 & 0x1000) || (a->w30c & 8))
		goto no_track_a;
	else if ((a->l308 & 1) && (!(b->l308 & 4) || (b->l308 & 0x40)))
		s1 = 0;
	else if ((a->l308 & 1) && (b->l308 & 4) && !(b->l308 & 0x40))
		goto no_track_a;
	else if (a->l308 & 0x100)
		s1 = 8;
	else if ((a->l308 & 0x20) && (a->w30c & 2) && (g_6064880 & 1))
		s1 = 9;
	else if ((a->l308 & 0x20) && (a->w30c & 1) && (g_6064880 & 1))
		s1 = 3;
	else if ((a->l308 & 0x20) && (a->w30c & 0x1080) && (g_6064880 & 1))
		s1 = 7;
	else if (a->l308 & 2)
		goto no_track_a;
	else if ((unsigned char)st == 10 && !(b->l308 & 4))
		s1 = 2;
	else if ((a->l308 & 0x40) && a->w304)
		goto no_track_a;
	else if (a->l308 & 0x40)
		s1 = 2;
	else if (m && (a->w30c & 0x1000)) {
		v = a->w322;
		if (v > 500)
			v = 500;
		s1 = f_966c(v);
	} else if ((m && (a->w30c & 1)) || (a->w30c & 4))
		s1 = f_9648(a->w322);
	else
		s1 = f_95b8(a->w322);

	goto track_a_done;
no_track_a:
	s1 = -1;
track_a_done:
	;
	st = b->w35c;
	if (!(b->l308 & 0x1000) || (b->w30c & 8))
		s2 = -1;
	else if ((b->l308 & 1) && (!(a->l308 & 4) || (a->l308 & 0x40)))
		s2 = 0;
	else if ((b->l308 & 1) && (a->l308 & 4) && !(a->l308 & 0x40))
		s2 = -1;
	else if (b->l308 & 0x100)
		s2 = 8;
	else if ((b->l308 & 0x20) && (b->w30c & 2) && (g_6064880 & 1))
		s2 = 9;
	else if ((b->l308 & 0x20) && (b->w30c & 1) && (g_6064880 & 1))
		s2 = 3;
	else if ((0x20 & b->l308) && (b->w30c & 0x1080) && (g_6064880 & 1))
		s2 = 7;
	else if (b->l308 & 2)
		s2 = -1;
	else if ((unsigned char)st == 10 && !(a->l308 & 4))
		s2 = 2;
	else if ((b->l308 & 0x40) && b->w304)
		s2 = -1;
	else if (b->l308 & 0x40)
		s2 = 2;
	else {
		m = g_6064880 & 1;
		if (m && (b->w30c & 0x1000)) {
			v = b->w322;
			if (v > 500)
				v = 500;
			s2 = f_966c(v);
		} else if ((m && (b->w30c & 1)) || (b->w30c & 4))
			s2 = f_9648(b->w322);
		else
			s2 = f_95b8(b->w322);
	}

	if (s1 >= 0 && s2 >= 0) {
		if (s1 < s2)
			s1 = s2;
		if (g_6064884.w14 != s1)
			g_6079298 = s1;
	} else if (s1 > -1 || s2 > -1) {
		if (s1 == -1)
			s1 = s2;
		if (g_6064884.w14 != s1)
			g_6079298 = s1;
	}
	if (g_6079299 > -1) {
		g_6079298 = g_6079299;
		g_6079299 = -1;
	}
	want = g_6079298;
	vol = g_6079296;
	tgt = g_6079297;
	t = g_6060008;
	switch (g_607929a) {
	case 0:
		if (want > -1) {
			if (want == g_6064884.w14) {
				g_6079298 = -1;
			} else {
				g_607929a = 1;
				g_6079297 = 0;
			}
		}
		break;
	case 1:
		if (vol > tgt) {
			if (d_3a8cf[want][0]) {
				if (!(t % d_3a8cf[want][0]))
					g_6079296--;
			} else {
				g_6079296 = tgt;
			}
		} else {
			g_607929a++;
			g_6079297 = 5;
			switch (g_6079298) {
			case 1:
			default:
				trk = 0;
				break;
			case 3:
				trk = 1;
				break;
			case 5:
				trk = 2;
				break;
			case 7:
				trk = 3;
				break;
			case 0:
				trk = 7;
				break;
			case 8:
				trk = 6;
				break;
			case 10:
				trk = 4;
				break;
			case 9:
				trk = 5;
				break;
			case 2:
			case 4:
			case 6:
				trk = -1;
				break;
			}
			if (trk > -1)
				f_2e5aa(trk);
			else
				f_2ed3e();
			g_6064884.w14 = g_6079298;
		}
		f_2eeaa(g_6079296);
		break;
	case 2:
		if (vol < tgt) {
			if (d_3a8cf[want][1]) {
				if (!(t % d_3a8cf[want][1]))
					g_6079296++;
			} else {
				g_6079296 = tgt;
			}
		} else {
			g_6079298 = -1;
			g_607929a = 0;
		}
		f_2eeaa(g_6079296);
		break;
	}
}
