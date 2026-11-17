/* rom: 0xb3c0 len: 0xa44 func: f_b3c0 flags: -macsave=1 -optimize=1 -speed */
/* difftest-nonreturning: f_b3c0 f_bc02 (why: the settings menu and its yes/no dialog loop until a button byte (g_6079304/5) selects the exit, and f_b3c0 first waits for g_6064750 to clear) */
/* difftest-inputs f_b3c0: 1/m6064750.4=0,m6079304.1=80,m6079305.1=8,m6060034.2=0 2/m6064750.4=0,m6079304.1=8,m6079305.1=80,m6060034.2=1 3/m6064750.4=0,m6079304.1=80,m6079305.1=c,m6060034.2=1 4/m6064750.4=0,m6079304.1=e,m6079305.1=80,m6060034.2=0 5/m6064750.4=0,m6079304.1=80,m6079305.1=88,m6060034.2=2 6/m6064750.4=0,m6079304.1=88,m6079305.1=80,m6060034.2=1 7/m6064750.4=0,m6079304.1=80,m6079305.1=18,m6060034.2=0 */
/* difftest-inputs f_bc02: 1/m6064750.4=0,m6079304.1=8,m6079305.1=0 2/m6064750.4=0,m6079304.1=0,m6079305.1=8 3/m6064750.4=0,m6079304.1=10,m6079305.1=8 4/m6064750.4=0,m6079304.1=8,m6079305.1=20 5/m6064750.4=0,m6079304.1=20,m6079305.1=8 6/m6064750.4=0,m6079304.1=18,m6079305.1=10 7/m6064750.4=0,m6079304.1=28,m6079305.1=0 8/m6064750.4=0,m6079304.1=ff,m6079305.1=ff 9/m6064750.4=0,m6079304.1=8,m6079305.1=8 10/m6064750.4=0,m6079304.1=c,m6079305.1=20 */
/* Non-matching: functionally equivalent C for [0xb3c0, 0xbd8a) (f_b3c0, f_b892, f_bc02),
 * linked only by the shiftable build (splits_nonmatching.txt). Not matching coverage.
 * From src/settings_reset_9c0c.c. */
/* the original strings */
extern char g_6032c28[];
extern char g_6032c34[];
extern char g_6032c3c[];
extern char g_6032c44[];
extern char g_6032c58[];
extern char g_6032c60[];
extern char g_6032c6c[];
extern char g_6032c78[];
extern char g_6032c84[];
extern char g_6032c90[];
extern char g_6032b54[];
extern char g_6032b5c[];
extern char g_6032c9c[];
extern char g_6032ca4[];
extern char g_6032cb4[];
extern char g_6032cbc[];
extern char g_6032cd0[];
extern char g_6032ce0[];
extern char g_6032cf8[];
extern char g_6032cfc[];
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
extern void f_21206(void);
/* the original tables, in the RAM data image */
extern const char *const g_6032b8c[30];
#define names g_6032b8c
extern const unsigned char g_6032c04[9];
#define off9 g_6032c04
extern const short g_6032c0e[12];
#define ypos g_6032c0e

void f_b3c0(void)
{
	short buf[19];
	int changed = 0;
	int run = 1;
	int dirty = 0;
	short cur = 0;
	int i, j, n;
	short k;
	short *cfg = g_6060034;

	while (g_6064750)
		f_2b4e8();
	f_29cc6(0, 4, 0, -0x7ffa);
	g_606006c[g_60792ec].w4 = 30;
	g_606006c[g_60792ec].w8 = 30;
	f_b2f4();
	for (i = 0; i < 11; i++)
		buf[i] = cfg[i];
	for (;;) {
		f_2b4e8();
		f_9ede();
		g_606006c[g_60792ec].w8 = 30;
		if ((g_6079304 == 0x80) | (g_6079305 == 0x80)) {
			cur--;
			if (cur < 0)
				cur = 10;
			if (cur == 3)
				cur = 2;
			if (buf[6] == 2) {
				if (cur == 8)
					cur = 6;
			} else if (buf[5] == 1) {
				if (cur == 8)
					cur--;
			}
		}
		if ((g_6079304 == 0x40) | (g_6079305 == 0x40)) {
			cur++;
			if (cur > 10)
				cur = 0;
			if (cur == 3)
				cur = 4;
			if (buf[6] == 2) {
				if (cur == 7)
					cur = 9;
			} else if (buf[5] == 1) {
				if (cur == 8)
					cur++;
			}
		}
		if ((g_6079304 == 0x10) | (g_6079305 == 0x10)) {
			if (cur < 9) {
				if (--buf[cur] < 0) {
					if ((cur == 7) | (cur == 8))
						buf[cur] = 0;
					else
						buf[cur] = g_6032b80[cur];
				}
				if (buf[6] == 1)
					buf[6] = 0;
			}
		}
		if ((g_6079304 == 0x20) | (g_6079305 == 0x20)) {
			if (cur < 9) {
				if (++buf[cur] > g_6032b80[cur]) {
					if ((cur == 7) | (cur == 8))
						buf[cur] = g_6032b80[cur];
					else
						buf[cur] = 0;
				}
				if (buf[6] == 1)
					buf[6] = 2;
			}
		}
		if (buf[0] == 1)
			*(unsigned char *)0x2405ffee |= 0xc0;
		else
			*(unsigned char *)0x2405ffee &= 0x3f;
		g_606006c[g_60792ec].w4 = ypos[cur];
		f_b892(buf, cur);
		f_e520(30, ypos[cur], g_6032960, 0);
		if ((g_6079304 & 8) | (g_6079305 & 8)) {
		switch (cur) {
		case 9:
			if (f_bc02(g_6032c28, buf, 1))
				break;
			for (i = 0; i < 10; i++)
				buf[i] = g_6032b6a[i];
			if (~IN3 & 0x40)
				dirty = 1;
			break;
		case 10:
			for (i = 0; i < 10; i++) {
				if (buf[i] != cfg[i])
					changed = 1;
			}
			if (changed == 0 && dirty == 0)
				;
			else if (f_bc02(g_6032c34, buf, 0) == 0) {
				f_2b4e8();
				f_b892(buf, 11);
				f_e3e0(200, g_6032c3c, 0);
				k = g_60792ec;
				g_606006c[k].w2c |= 0x8000;
				f_2b4e8();
				for (i = 0, j = 0; i < 10; i++)
					j += (short)(k = buf[i]), cfg[i] = k;
				cfg[10] = j;
				f_3036e();
				f_21206();
				f_b892(buf, 11);
				f_e3e0(200, g_6032c3c, 0);
				k = g_60792ec;
				g_606006c[k].w2c |= 0x8000;
				for (n = 20; n > 0; n--) {
					f_2b4e8();
					f_9ede();
					f_b892(buf, 11);
					f_e3e0(200, g_6032c3c, 0);
				}
			}
			run = 0;
			break;
		}
		}
		if (!run)
			break;
	}
}

void f_b892(register short *buf, short cur)
{
	short tmp[12];
	short unused[12];
	short j, idx;
	int f;

	f_e3e0(10, g_6032c44, 0);
	f_e412(50, ypos[0], g_6032c58, cur == 0 ? 0 : 1);
	f_e412(50, ypos[1], g_6032c60, cur == 1 ? 0 : 1);
	f_e412(50, ypos[2], g_6032c6c, cur == 2 ? 0 : 1);
	f_e412(50, ypos[4], g_6032c78, cur == 4 ? 0 : 1);
	f_e412(50, ypos[5], g_6032c84, cur == 5 ? 0 : 1);
	f_e412(50, ypos[6], g_6032c90, cur == 6 ? 0 : 1);
	if (buf[6] == 0) {
		if (buf[5] == 0) {
			f_e412(70, ypos[7], g_6032b54, cur == 7 ? 0 : 1);
			f_e412(70, ypos[8], g_6032b5c, cur == 8 ? 0 : 1);
		} else
			f_e412(70, ypos[7], g_6032c9c, cur == 7 ? 0 : 1);
	}
	f_e412(50, ypos[9], g_6032ca4, cur == 9 ? 0 : 1);
	f_e412(50, ypos[10], g_6032cb4, cur == 10 ? 0 : 1);
	if (cur <= 10) {
		g_606006c[tmp[cur]].b1c = 0;
		switch (cur) {
		case 9:
			f_e3e0(200, g_6032cbc, 0);
			break;
		case 10:
			f_e3e0(200, g_6032cd0, 0);
			break;
		default:
			f_e3e0(200, g_6032ce0, 0);
		}
	}
	for (j = 0; j < 6; j++) {
		idx = buf[j] + off9[j];
		f_e412(180, ypos[j], names[idx], cur == j ? 0 : 1);
	}
	if (buf[6] == 2) {
		idx = off9[6] + buf[6] - 1;
		f_e412(180, ypos[6], names[idx], cur == 6 ? 0 : 1);
	} else {
		idx = off9[6] + buf[6];
		f_e412(180, ypos[6], names[idx], cur == 6 ? 0 : 1);
	}
	if (buf[6] == 0) {
		f = (cur == 7 || cur == 6) ? 0 : 1;
		f_e412(160, ypos[7], names[off9[7] + buf[7]], f);
		if (buf[5] == 0) {
			f = (cur == 8 || cur == 6) ? 0 : 1;
			f_e412(160, ypos[8], names[off9[8] + buf[8]], f);
		}
	}
}

unsigned char f_bc02(char *title, short *buf, char sel)
{
	short *b = buf;
	short t[2];
	int run = 1;
	int x;

	do {
		f_2b4e8();
		f_9ede();
		f_b892(b, 11);
		f_e412(100, 200, title, 0);
		f_e412(200, 200, g_6032cf8, sel == 0 ? 0 : 1);
		f_e412(240, 200, g_6032cfc, sel == 1 ? 0 : 1);
		g_606006c[g_60792ec].w4 = 200;
		if ((g_6079304 == 0x10) | (g_6079305 == 0x10))
			sel = 0;
		if ((g_6079304 == 0x20) | (g_6079305 == 0x20))
			sel = 1;
		if ((g_6079304 & 8) | (g_6079305 & 8))
			run = 0;
		if (sel == 0) {
			g_606006c[t[0]].b1c = 0;
			g_606006c[t[1]].b1c = 64;
		} else {
			g_606006c[t[0]].b1c = 64;
			g_606006c[t[1]].b1c = 0;
		}
		g_606006c[g_60792ec].w8 = x = sel * 40 + 188;
		f_e520(x, 200, g_6032960, 0);
	} while (run);
	return sel;
}

extern void f_bd8a(void);

extern unsigned short d_3a8e8[];
extern void f_2f2cc(void);
extern void f_2f3dc(void);
extern void f_2e5aa(short);
extern void f_2ed3e(void);
extern void f_2e6fc(short);
extern void f_2ed06(void);
void f_e63a(int, short, int, short, long, unsigned char, char);

unsigned short f_be04(short v);

void f_be28(void);

extern void f_2ee38(void);
extern void f_2ee74(void);
extern char f_2ee0c(char);
extern void f_2edcc(char, char);

void f_c21a(void);


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

void f_cac8(void);

void f_cf68(void);

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
