/* rom: 0xbe28 len: 0x1464 func: f_be28 flags: -macsave=1 -optimize=1 -speed */
/* difftest-nonreturning: f_be28 f_c21a f_cac8 f_cf68 (why: the sound tests, sprite viewer and bookkeeping screen wait for g_6064750 to clear and loop until a button byte selects the exit) */
/* difftest-inputs f_be28: 1/m6064750.4=0,m6079304.1=80,m6079305.1=8,m6064754.1=0,m6064755.1=0 2/m6064750.4=0,m6079304.1=8,m6079305.1=80,m6064754.1=10,m6064755.1=20 3/m6064750.4=0,m6079304.1=80,m6079305.1=8,m6064754.1=20,m6064755.1=10 4/m6064750.4=0,m6079304.1=8,m6079305.1=80,m6064754.1=30,m6064755.1=0 5/m6064750.4=0,m6079304.1=80,m6079305.1=8,m6064754.1=10,m6064755.1=10 6/m6064750.4=0,m6079304.1=8,m6079305.1=80,m6064754.1=0,m6064755.1=20 */
/* difftest-inputs f_c21a: 1/m6064750.4=0,m6079304.1=80,m6079305.1=8,m6064754.1=0,m6064755.1=0 2/m6064750.4=0,m6079304.1=8,m6079305.1=80,m6064754.1=10,m6064755.1=20 3/m6064750.4=0,m6079304.1=80,m6079305.1=8,m6064754.1=20,m6064755.1=10 4/m6064750.4=0,m6079304.1=8,m6079305.1=80,m6064754.1=30,m6064755.1=0 5/m6064750.4=0,m6079304.1=80,m6079305.1=8,m6064754.1=10,m6064755.1=10 6/m6064750.4=0,m6079304.1=8,m6079305.1=80,m6064754.1=0,m6064755.1=20 7/m6064750.4=0,m6079304.1=80,m6079305.1=8,m6064754.1=20,m6064755.1=20 */
/* difftest-inputs f_cac8: 1/m6064750.4=0,m6064754.1=1,m6064755.1=0,m6079304.1=0,m6079305.1=0 2/m6064750.4=0,m6064754.1=1,m6064755.1=0,m6079304.1=8,m6079305.1=0 3/m6064750.4=0,m6064754.1=1,m6064755.1=0,m6079304.1=4,m6079305.1=0 4/m6064750.4=0,m6064754.1=3,m6064755.1=4,m6079304.1=0,m6079305.1=8 5/m6064750.4=0,m6064754.1=81,m6064755.1=0,m6079304.1=0,m6079305.1=4 6/m6064750.4=0,m6064754.1=1,m6064755.1=8,m6079304.1=c,m6079305.1=2 7/m6064750.4=0,m6064754.1=ff,m6064755.1=ff,m6079304.1=ff,m6079305.1=ff 8/m6064750.4=0,m6064754.1=1,m6064755.1=0,m6079304.1=0,m6079305.1=e 9/m6064750.4=0,m6064754.1=1,m6064755.1=0,m6079304.1=8,m6079305.1=8 */
/* difftest-inputs f_cf68: 1/m6064750.4=0,m6079304.1=8,m6079305.1=0,m6065680.4=0,m6065684.4=0,m6065688.2=1,m606568a.2=1,m606568c.2=3,m6065640.4=1234 2/m6064750.4=0,m6079304.1=0,m6079305.1=8,m6065680.4=3b,m6065684.4=e10,m6065688.2=7,m606568a.2=1,m606568c.2=3,m6065640.4=120f 3/m6064750.4=0,m6079304.1=9,m6079305.1=0,m6065680.4=34bc0,m6065684.4=34bc1,m6065688.2=3,m606568a.2=2,m606568c.2=6,m6065640.4=359f4 4/m6064750.4=0,m6079304.1=ff,m6079305.1=0,m6065680.4=ffffffff,m6065684.4=80000000,m6065688.2=ffff,m606568a.2=0,m606568c.2=0,m6065640.4=ffffedcb 5/m6064750.4=0,m6079304.1=8,m6079305.1=8,m6065680.4=bc614e,m6065684.4=5397fb1,m6065688.2=d,m606568a.2=9,m606568c.2=1b,m6065640.4=bc737a 6/m6064750.4=0,m6079304.1=88,m6079305.1=0,m6065680.4=e0f,m6065684.4=3b,m6065688.2=1,m606568a.2=1,m606568c.2=3,m6065640.4=1c3b 7/m6064750.4=0,m6079304.1=0,m6079305.1=18,m6065680.4=34bbf,m6065684.4=0,m6065688.2=0,m606568a.2=0,m606568c.2=0,m6065640.4=3598b 8/m6064750.4=0,m6079304.1=8,m6079305.1=0,m6065680.4=7fffffff,m6065684.4=1,m6065688.2=8000,m606568a.2=10,m606568c.2=30,m6065640.4=7fffedcb */
/* difftest-rom-end: f_cac8=0xcf68 (the loop at 0xcb44, reached only from inside f_cac8, is part of it) */
/* Non-matching: functionally equivalent C for [0xbe28, 0xd28c) (f_be28, f_c21a, f_cac8, f_cf68),
 * linked only by the shiftable build (splits_nonmatching.txt). Not matching coverage.
 * From src/settings_reset_9c0c.c. */
/* the original strings */
extern char g_6032d00[];
extern char g_6032d10[];
extern char g_6032d18[];
extern char g_6032cb4[];
extern char g_6032cd0[];
extern char g_6032d20[];
extern char g_6032d30[];
extern char g_6032ce0[];
extern char g_6032d40[];
extern char g_6032d48[];
extern char g_6032d50[];
extern char g_6032b64[];
extern char g_6032b68[];
extern char g_6032d58[];
extern char g_6032d5c[];
extern char g_6032d70[];
extern char g_6032d84[];
extern char d_20040024[];
struct rom_hdr {
	char pad0[48];
	long l30;
};
extern struct rom_hdr *rom_hdr_ptr;        /* 0x2004002c */
extern short eeprom_shadow[];              /* 0x6060034 */
struct spr {
	struct desc *desc;
	short w4;
	short w6;
	short w8;
	char pad0[0x14 - 0xa];
	unsigned char b14;
	unsigned char b15;
	char pad1[0x18 - 0x16];
	short w18;
	short w1a;
	char b1c;
	char pad2[0x22 - 0x1d];
	short w22;
	char pad3[0x2a - 0x24];
	char b2a;
	char b2b;
	unsigned short w2c;
	char pad4[0x40 - 0x2e];
};
struct desc {
	short w0;
	short w2;
	char b4, b5, b6, b7, b8, b9;
	short w10;
};
extern struct spr g_606006c[];
extern struct desc g_60792e0;
extern struct font *g_60792f0;
extern short g_60792ec;
extern short g_6060022;
extern char **g_607926c;
extern char g_60b1819;
extern char g_6061945;
struct snd { unsigned char b0; char pad[13]; short we; };
extern struct snd g_6066198;
extern unsigned char g_6064754, g_6064755;
extern int g_6064750;
extern unsigned char g_6079304, g_6079305, g_6079306, g_6079307;
extern short g_6079308;
extern char g_6032960[], g_6032964[], g_6032974[], g_6032980[], g_603298c[];
extern char g_6032998[], g_60329a8[], g_60329bc[], g_60329cc[], g_60329d8[], g_60329e0[];
extern void (*g_603294c[])(void);
extern void (*g_603292c[])(void);

extern void f_2b1cc(void);
extern void f_2521e(void);
extern void f_23f10(void);
extern void f_2b31e(void);
extern void f_2491c(void);
extern void f_175c4(void);
extern void f_2488c(void);
extern void f_2b3e8(short *);
extern void f_2c3d0(unsigned char);
extern void f_2c03c(short);
extern short f_2528c(void);
void f_29cc6(short, short, long, long);
extern void f_bd8a(void);
extern void f_2b4e8(void);
extern void f_2f2cc(void);
extern void f_2f3dc(void);
extern void f_2ecea(void);
extern void f_2edc2(void);
extern void f_2eeaa(int);
void f_2774a(short, int);
extern void f_e3b8(void);
void f_e3e0(short, char *, char);
void f_e412(short, short, char *, char);
void f_e520(short, short, char *, int);

struct anim {
	short w0;
	unsigned short w2;
	long l4;
	long l8;
};
struct font {
	char pad0[16];
	long l10;
	long n;
	short *seq[1];
};
#define HDR_COUNT (*(short *)0x2004001c)
#define HDR_ANIM (*(struct anim **)0x20040030)
extern short g_6064870, g_6064872, g_6064874;
extern short g_6061944;
extern char g_60329ec[], g_60329fc[], g_6032a0c[], g_6032a18[], g_6032a24[], g_6032a28[], g_6032a2c[];
extern char g_6032a3c[], g_6032a4c[], g_6032a5c[];
extern void f_253bc();
extern void f_2b0ea(short, long);
void f_e63a(int, short, int, short, long, unsigned char, char);
void f_acdc(short *);
extern void f_2c28c(void);
extern void f_2c326(void);
extern void f_29f94(unsigned long, unsigned long, unsigned char, unsigned char);

#define REG(a) (*(unsigned char *)(a))

void f_9c0c(void);

void f_9e6a(unsigned char m);

void f_9ea4(unsigned char m);

void f_9ede(void);

void f_a226(void);

void f_a608(void);

void f_acdc(short *sel);


#define IN3 (*(unsigned char *)0x23000003)
extern short g_6060034[11];
extern void f_2f36e(void), f_b352(void), f_b36e(void);
extern void f_3036e(void);
/* Genuine screen-test predecessor reconstructed from ROM AD80-AF8C.
 * Private analysis source. All five drawing arguments include the pushed125. */
struct frame;
extern void f_29bf8(void);
void f_2af5e(unsigned char, unsigned char, long *);
extern void f_185f0(struct frame *, short, short, char, short);

void f_ad80(void);

void f_af8c(void);

extern const short g_6032b6a[11];
extern const unsigned char g_6032b80[10];


int f_b2f4(void);



void f_b352(void);


void f_b36e(void);


/* Genuine settings and sound-test drafts; no artificial stack objects. */

void f_b892(short *,short);
unsigned char f_bc02(char *,short *,char);
extern void f_20206(void);
/* the original tables, in the RAM data image */
extern const char *const g_6032b8c[30];
#define names g_6032b8c
extern const unsigned char g_6032c04[9];
#define off9 g_6032c04
extern const short g_6032c0e[12];
#define ypos g_6032c0e

void f_b3c0(void);

void f_b892(short *buf, short cur);

unsigned char f_bc02(char *title, short *buf, char sel);

void f_bd8a(void);

extern unsigned short d_3a8e8[];
extern void f_2f2cc(void);
extern void f_2f3dc(void);
extern void f_2e5aa(short);
extern void f_2ed3e(void);
extern void f_2e6fc(short);
extern void f_2ed06(void);
void f_e63a(int, short, int, short, long, unsigned char, char);

unsigned short f_be04(short v);

void f_be28(void)
{
	short effect = 0;
	short sel = 0;
	short music = 0;
	int run = 1;
	short cnt1 = 0;
	short prev2 = 0;
	short cnt2 = 0;
	short prev1 = 0;
	short rep1, rep2;
	int y;
	int k;

	while (g_6064750)
		f_2b4e8();
	f_29cc6(0, 4, 0, -0x7ffa);
	f_2f2cc();
	f_2f3dc();
	g_606006c[g_60792ec].w4 = 50;
	g_606006c[g_60792ec].w8 = 60;
	do {
		f_2b4e8();
		f_9ede();
		if (prev1 != g_6064754) {
			prev1 = g_6064754;
			cnt1 = 0;
			rep1 = prev1;
		} else {
			cnt1++;
			cnt1 &= 0x7fff;
			if (cnt1 > 30)
				rep1 = prev1;
			else
				rep1 = 0;
		}
		if (prev2 != g_6064755) {
			prev2 = g_6064755;
			cnt2 = 0;
			rep2 = prev2;
		} else {
			cnt2++;
			cnt2 &= 0x7fff;
			if (cnt2 > 30)
				rep2 = prev2;
			else
				rep2 = 0;
		}
		if ((g_6079304 == 0x40) | (g_6079305 == 0x40)) {
			if (++sel > 2)
				sel = 0;
		}
		if ((g_6079304 == 0x80) | (g_6079305 == 0x80)) {
			if (--sel < 0)
				sel = 2;
		}
		switch (sel) {
		case 0:
			if ((rep2 == 0x20) | (rep1 == 0x20)) {
				if (++music > 7)
					music = 0;
			}
			if ((rep2 == 0x10) | (rep1 == 0x10)) {
				if (--music < 0)
					music = 7;
			}
			if ((g_6079304 == 8) | (g_6079305 == 8))
				f_2e5aa(music);
			if ((g_6079304 == 4) | (g_6079305 == 4))
				f_2ed3e();
			break;
		case 1:
			if ((rep2 == 0x20) | (rep1 == 0x20)) {
				if (++effect > 40)
					effect = 0;
			}
			if ((rep2 == 0x10) | (rep1 == 0x10)) {
				if (--effect < 0)
					effect = 40;
			}
			if ((g_6079304 == 8) | (g_6079305 == 8)) {
				if (f_be04(effect) == 0)
					f_2e6fc(effect);
			}
			if ((g_6079304 == 4) | (g_6079305 == 4))
				f_2ed06();
			break;
		case 2:
			if ((g_6079304 == 8) | (g_6079305 == 8))
				run = 0;
			break;
		}
		f_e3e0(10, g_6032d00, 0);
		f_e412(80, 50, g_6032d10, sel == 0 ? 0 : 1);
		f_e412(80, 64, g_6032d18, sel == 1 ? 0 : 1);
		f_e412(80, 78, g_6032cb4, sel == 2 ? 0 : 1);
		g_606006c[g_60792ec].w4 = y = sel * 14 + 50;
		f_e520(60, y, g_6032960, 0);
		f_e63a(music, 50, 200, 2, 0, 1, sel == 0 ? 0 : 1);
		f_e63a(effect, 64, 200, 3, 0, 1, sel == 1 ? 0 : 1);
		if (sel == 2)
			f_e3e0(200, g_6032cd0, 0);
		else {
			f_e3e0(208, g_6032d20, 0);
			f_e3e0(194, g_6032d30, 0);
			f_e3e0(180, g_6032ce0, 0);
		}
	} while (run);
	f_2f2cc();
	f_2f3dc();
	f_2b4e8();
}

extern void f_2ee38(void);
extern void f_2ee74(void);
extern char f_2ee0c(char);
extern void f_2edcc(char, char);

void f_c21a(void)
{
	int run = 1;
	short sel = 0;
	short music = 0;
	short effect = 0;
	short prev1 = 0;
	short cnt1 = 0;
	short prev2 = 0;
	short cnt2 = 0;
	short voice = 0;
	short rep1, rep2;
	short i;

	while (g_6064750)
		f_2b4e8();
	f_29cc6(0, 4, 0, -0x7ffa);
	g_606006c[g_60792ec].w4 = 50;
	g_606006c[g_60792ec].w8 = 60;
	f_2f2cc();
	f_2f3dc();
	f_2ee38();
	do {
		f_2b4e8();
		f_9ede();
		if (prev1 != g_6064754) {
			prev1 = g_6064754;
			cnt1 = 0;
			rep1 = prev1;
		} else {
			cnt1++;
			cnt1 &= 0x7fff;
			if (cnt1 > 30)
				rep1 = prev1;
			else
				rep1 = 0;
		}
		if (prev2 != g_6064755) {
			prev2 = g_6064755;
			cnt2 = 0;
			rep2 = prev2;
		} else {
			cnt2++;
			cnt2 &= 0x7fff;
			if (cnt2 > 30)
				rep2 = prev2;
			else
				rep2 = 0;
		}
		if ((g_6079304 == 0x40) | (g_6079305 == 0x40)) {
			if (sel == 2) {
				if (voice < 8)
					voice += 8;
				else if (++sel > 5)
					sel = 0;
			} else if (++sel > 5)
				sel = 0;
		}
		if ((g_6079304 == 0x80) | (g_6079305 == 0x80)) {
			if (sel == 2) {
				if (voice > 7)
					voice -= 8;
				else if (--sel < 0)
					sel = 5;
			} else if (--sel < 0)
				sel = 5;
		}
		switch (sel) {
		case 0:
			if ((rep2 == 0x20) | (rep1 == 0x20)) {
				if (++music > 7)
					music = 0;
			}
			if ((rep2 == 0x10) | (rep1 == 0x10)) {
				if (--music < 0)
					music = 7;
			}
			if ((g_6079304 == 8) | (g_6079305 == 8))
				f_2e5aa(music);
			if ((g_6079304 == 4) | (g_6079305 == 4))
				f_2ed3e();
			g_606006c[g_60792ec].w4 = 50;
			f_e520(60, 50, g_6032960, 0);
			break;
		case 1:
			if ((rep2 == 0x20) | (rep1 == 0x20)) {
				if (++effect > 40)
					effect = 0;
			}
			if ((rep2 == 0x10) | (rep1 == 0x10)) {
				if (--effect < 0)
					effect = 40;
			}
			if ((g_6079304 == 8) | (g_6079305 == 8)) {
				if (f_be04(effect) == 0)
					f_2e6fc(effect);
			}
			if ((g_6079304 == 4) | (g_6079305 == 4))
				f_2ed06();
			g_606006c[g_60792ec].w4 = 64;
			f_e520(60, 64, g_6032960, 0);
			break;
		case 2:
			if ((g_6079304 == 0x20) | (g_6079305 == 0x20)) {
				if (++voice > 15)
					voice = 0;
			}
			if ((g_6079304 == 0x10) | (g_6079305 == 0x10)) {
				if (--voice < 0)
					voice = 15;
			}
			if ((g_6079304 == 8) | (g_6079305 == 8))
				f_2edcc(voice, (f_2ee0c(voice) + 1) & 1);
			g_606006c[g_60792ec].w4 = 78;
			f_e520(60, 78, g_6032960, 0);
			break;
		case 3:
			if ((g_6079304 == 8) | (g_6079305 == 8))
				f_2ee38();
			g_606006c[g_60792ec].w4 = 120;
			f_e520(60, 120, g_6032960, 0);
			break;
		case 4:
			if ((g_6079304 == 8) | (g_6079305 == 8))
				f_2ee74();
			g_606006c[g_60792ec].w4 = 134;
			f_e520(60, 134, g_6032960, 0);
			break;
		case 5:
			if ((g_6079304 == 8) | (g_6079305 == 8))
				run = 0;
			g_606006c[g_60792ec].w4 = 148;
			f_e520(60, 148, g_6032960, 0);
			break;
		}
		f_e3e0(10, g_6032d00, 0);
		f_e412(80, 50, g_6032d10, sel == 0 ? 0 : 1);
		f_e412(80, 64, g_6032d18, sel == 1 ? 0 : 1);
		f_e412(80, 78, g_6032d40, sel == 2 ? 0 : 1);
		f_e412(80, 120, g_6032d48, sel == 3 ? 0 : 1);
		f_e412(80, 134, g_6032d50, sel == 4 ? 0 : 1);
		f_e412(80, 148, g_6032cb4, sel == 5 ? 0 : 1);
		for (i = 0; i < 16; i++) {
			if (i < 8)
				f_e520(i * 17 + 114, 92, f_2ee0c(i) == 0 ? g_6032b64 : g_6032b68, 0);
			else
				f_e520((i - 8) * 17 + 114, 106, f_2ee0c(i) == 0 ? g_6032b64 : g_6032b68, 0);
		}
		if (sel == 2) {
			if (voice < 8)
				f_e520(voice * 17 + 114, 92, g_6032d58, 0);
			else
				f_e520((voice - 8) * 17 + 114, 106, g_6032d58, 0);
		}
		f_e63a(music, 50, 200, 2, 0, 1, sel == 0 ? 0 : 1);
		f_e63a(effect, 64, 200, 3, 0, 1, sel == 1 ? 0 : 1);
		f_e63a(voice, 78, 200, 2, 0, 1, sel == 2 ? 0 : 1);
		switch (sel) {
		case 2:
			f_e3e0(204, g_6032d5c, 0);
			f_e3e0(190, g_6032ce0, 0);
			break;
		case 3:
			f_e3e0(200, g_6032d70, 0);
			break;
		case 4:
			f_e3e0(200, g_6032d84, 0);
			break;
		case 5:
			f_e3e0(200, g_6032cd0, 0);
			break;
		default:
			f_e3e0(208, g_6032d20, 0);
			f_e3e0(194, g_6032d30, 0);
			f_e3e0(180, g_6032ce0, 0);
			break;
		}
	} while (run);
	f_2f2cc();
	f_2f3dc();
	f_2b4e8();
}


/* Genuine later menu and renderer drafts. */

struct st {
	long l0, l4, l8, lc;
	unsigned char b10, b11, b12, b13, b14;
	char pad;
	short w16[4];
	short w1e;
};
struct preview_sprite { char pad[4]; short w4; short w6; };
struct s2t { struct preview_sprite *spr; long b; long c; long d; };
struct rec {
	short w0;
	short w2;
	char s[32];
};
extern struct st g_6066188;
#define S g_6066188
extern unsigned char g_6064754, g_6064755, g_6079304, g_6079305;
extern short g_60792ec, g_60792de, g_60792dc;
extern short g_6033014[];
extern long d_3ab1c[];
extern char d_67910[];
extern void f_2b4e8(void);
void f_29cc6(short, short, long, long);
extern void f_9ede(void);
extern void f_acdc(short *);
extern void f_25658(short);
extern long f_25f6e(void);
extern void f_2616a(long, void *);
extern void f_26f64(long, long);
void f_2774a(short, int);
extern void f_25fde(long);
extern void f_2639e(long, short, short);
void f_1865e(long, short, short, short, short, short, short, short);
void f_2af5e(unsigned char, unsigned char, long *);
extern short f_2bf58(void *, short, short);
extern short f_2c0d0(void *);
extern void f_30eec(void);
extern void f_11fc4(void);
extern void f_11fe8(void);
extern void f_13378(void);
extern void f_b352(void);
extern void f_b36e(void);
extern void f_211a2(void);
extern void f_21206(void);
extern short f_2beca(int);
extern char g_6032da4[], g_6032db0[], g_6032a18[], g_6032a24[], g_6032a28[], g_6032db8[], g_6032dc0[], g_6032a2c[];
extern char g_6032dc8[], g_6032dd8[], g_6032de4[], g_6032df0[], g_6032dfc[], g_6032e0c[], g_6032e1c[], g_6032e20[], g_6032e30[];
extern char g_6032e4c[], g_6032e64[], g_6032e74[], g_6032e88[], g_6032e9c[], g_6032eb4[], g_6032eb8[], g_6032ecc[], g_6032ee0[];
extern char g_6032eec[], g_6032f04[], g_6032f1c[], g_6032f34[], g_6032f4c[];
extern char g_6032f64[], g_6032f84[], g_6032fa4[], g_6032fd4[], g_6032ff8[];
extern unsigned char g_603290c[4][5], g_6032925[];
struct pl { char pad[48]; unsigned long a48; unsigned long a52; unsigned short w56, w58, w60; };
extern struct pl g_6065650;
extern long g_6065640;
extern struct rec d_3a8f4[4], d_3a984[5], d_3aa9c[3];
extern void *d_3aa38[], *d_3aa80[], *d_3aa8c[], *d_3aa94[], *d_3ab08[], *d_3ab10[];
extern char d_a7afc[], d_a7b08[], d_a7b14[], d_a7b2c[], d_a7bec[], d_a7bf8[], d_a7c04[], d_a7c10[], d_a7da8[];
extern long d_3ad1c[];

void f_e3e0(short, char *, char);
void f_e412(short, short, char *, char);
void f_e63a(int, short, int, short, long, unsigned char, char);
void f_dbe8(void);
void f_e074(void);

void f_cac8(void)
{
	short s0;
	short t4;
	char cnt;
	struct preview_sprite **tbl;
	short show;
	signed char h20, h44;
	unsigned char h36, h40;
	short w24, w28;
	long run;
	struct s2t s2;
	struct s2t *p;
	short idx;
	long id;
	long hold;

	tbl = *(struct preview_sprite ***)((long)d_20040024);
	run = 1;
	s0 = 0;
	cnt = 0;
	w24 = 0;
	w28 = 0;
	show = 1;
	idx = 0;
	while (g_6064750)
		f_2b4e8();
	f_29cc6(0, 4, 0, -0x7ffa);
	f_acdc(&s0);
	{
		short v = g_60792ec;
		g_606006c[v].w2c |= 0x8000;
		f_25658(v);
	}
	for (;;) {
	
	p = &s2;
	p->spr = tbl[0];
	p->b = 1;
	p->c = 0;
	p->d = 1;
	S.b10 |= 1;
	S.b11 = 0;
	id = f_25f6e();
	f_2616a(id, p);
	f_26f64(id, 1);
	f_2774a(id, 1);
	
	while (run) {
		f_2b4e8();
		f_9ede();
		if (cnt < 18)
			cnt++;
		if (cnt > 16) {
			unsigned char a = g_6064754;
			unsigned char b = g_6064755;
			if (h40 == (a & 15)) {
				if (--h20 < 0)
					h20 = 1;
				hold = (hold + 1) & 0xfff;
			} else {
				h40 = a & 15;
				h20 = 16;
				hold = 0;
			}
			if (h36 == (b & 15)) {
				if (--h44 < 0)
					h44 = 1;
				hold = (hold + 1) & 0xfff;
			} else {
				h36 = b & 15;
				h44 = 16;
				hold = 0;
			}
		}
		if (g_6079304 & 8) {
			idx++;
			if (idx >= 10)
				idx = 0;
			S.b10 |= 1;
			S.b11 = idx;
			w24 = 0;
			w28 = 0;
			f_25fde(id);
			id = f_25f6e();
			p->spr = tbl[idx];
			f_2616a(id, p);
			f_26f64(id, 1);
			f_2774a(id, 1);
		}
		if (g_6079304 & 4) {
			idx--;
			if (idx < 0)
				idx = 9;
			S.b10 |= 1;
			S.b11 = idx;
			w24 = 0;
			w28 = 0;
			f_25fde(id);
			id = f_25f6e();
			p->spr = tbl[idx];
			f_2616a(id, p);
			f_26f64(id, 1);
			f_2774a(id, 1);
		}
		if (g_6079305 & 8) {
			s0++;
			f_acdc(&s0);
		}
		if (g_6079305 & 4) {
			s0--;
			f_acdc(&s0);
		}
		if (g_6079305 & 2)
			show ^= 1;
		if (show) {
			f_e3e0(10, g_6032da4, 0);
			f_e412(10, 20, g_6032db0, 0);
			f_e412(10, 34, g_6032a18, 0);
			f_e63a(idx, 20, 90, 1, 1, 1, 0);
			f_e63a(s0, 34, 90, 1, 0, 1, 0);
			f_e412(250, 20, g_6032a24, 0);
			f_e412(250, 34, g_6032a28, 0);
			f_e63a(t4 = -w24, 20, 275, 3, 1, 1, 0);
			f_e63a(t4 = -w28, 34, 275, 3, 1, 1, 0);
			f_e412(220, 48, g_6032db8, 0);
			f_e63a(t4 = p->spr->w4, 48, 280, 3, 1, 1, 0);
			f_e412(220, 62, g_6032dc0, 0);
			f_e63a(t4 = p->spr->w6, 62, 280, 3, 1, 1, 0);
			f_e3e0(214, g_6032a2c, 0);
		}
		if ((g_6064754 & 1) == 1)
			run = 0;
		f_2639e(id, w24, w28);
	}
	break;
	}
	*(short *)0x6061944 = 125;
	f_25658(0);
	f_2b4e8();
}

void f_cf68(void)
{
	unsigned short m, h, s;
	long run;
	unsigned long r, q;

	run = 1;
	while (g_6064750)
		f_2b4e8();
	f_29cc6(0, 4, 0, -0x7ffa);
	do {
		f_2b4e8();
		f_9ede();
		f_e3e0(20, g_6032dc8, 0);
		h = g_6065650.a48 / 216000;
		r = g_6065650.a48 - h * 216000;
		m = r / 3600;
		r = r - m * 3600;
		s = r / 60;
		f_e412(60, 60, g_6032dd8, 0);
		f_e412(60, 74, g_6032de4, 0);
		f_e412(60, 88, g_6032df0, 0);
		f_e412(60, 102, g_6032dfc, 0);
		f_e412(60, 130, g_6032e0c, 0);
		f_e412(200, 130, g_6032e1c, 0);
		f_e412(224, 130, g_6032e1c, 0);
		f_e63a(g_6065640, 60, 200, 6, 0, 2, 0);
		f_e63a(g_6065650.w56, 74, 200, 6, 0, 2, 0);
		f_e63a(g_6065650.w58, 88, 200, 6, 0, 2, 0);
		f_e63a(g_6065650.w60, 102, 200, 6, 0, 2, 0);
		f_e63a(h, 130, 160, 5, 0, 2, 0);
		f_e63a(m, 130, 208, 2, 1, 2, 0);
		f_e63a(s, 130, 232, 2, 1, 2, 0);
		q = g_6065650.a52 / g_6065650.w56;
		h = q / 216000;
		r = q - h * 216000;
		m = r / 3600;
		r = r - m * 3600;
		s = r / 60;
		f_e412(60, 144, g_6032e20, 0);
		f_e412(200, 144, g_6032e1c, 0);
		f_e412(224, 144, g_6032e1c, 0);
		f_e63a(h, 144, 160, 5, 0, 2, 0);
		f_e63a(m, 144, 208, 2, 1, 2, 0);
		f_e63a(s, 144, 232, 2, 1, 2, 0);
		f_e3e0(214, g_6032e30, 0);
		if ((g_6079304 & 8) || (g_6079305 & 8))
			run = 0;
	} while (run);
	f_2b4e8();
}

/* rom: 0xd28c len: 0x108 func: f_d28c flags: -macsave=1 -optimize=1 -speed */
void f_d28c(unsigned long t, char *s);

void f_d33a(unsigned short g, char *s);

void f_d37c(void);

void f_d394(void);

void f_d8e0(void);

void f_d962(void);

void f_dbe8(void);

void f_e074(void);

short f_e39c(char *s);

void f_e3b8(void);

void f_e3e0(short y, char *s, char flag);

void f_e412(short x, short y, char *s, char flag);

void f_e490(short x, short y, char *s, short w, char flag);

/* rom: 0xe520 len: 0x230 func: f_e520 flags: -macsave=1 -optimize=1 -speed */
void f_1865e(long, short, short, short, short, short, short, short);
extern short g_6033014[];
extern long d_3ad1c[];
extern char d_a7c04[], d_a7c10[], d_a7bf8[], d_a7bec[], d_a7afc[], d_a7b08[], d_a7b14[], d_a7b2c[], d_a7da8[];

void f_e520(short x, short y, char *s, int h);

void f_e63a(int v, short y, int x, short k, long flag, unsigned char mode, char h);
