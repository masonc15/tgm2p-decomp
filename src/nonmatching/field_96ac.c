/* rom: 0x96ac len: 0x4e4 func: f_96ac flags: -macsave=1 -optimize=1 -speed */
/* Non-matching, functionally equivalent C for ROM 0x96ac-0x9b90 (f_96ac):
 * choose the background music track from both players' states and run the
 * fade-out / track switch / fade-in volume state machine. */
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

short f_8b86(unsigned char id);

short f_8c96(void);

void f_907c(void);

short f_9098(void);

int f_9180(void);

short f_95b8(short x);

short f_9648(short x);

short f_966c(short x);

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
		f_2eeaa((signed char)g_6079296);
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
		f_2eeaa((signed char)g_6079296);
		break;
	}
}
