/* rom: 0xd394 len: 0x1008 func: f_d394 flags: -macsave=1 -optimize=1 -speed */
/* difftest-nonreturning: f_d394 f_d8e0 f_d962 f_dbe8 f_e074 (why: the code-entry, test and option menus wait for g_6064750 to clear and loop until a button byte (g_6079304/5) selects the exit) */
/* difftest-inputs f_d394: 1/m6064750.4=0,m6079304.1=1,m6079305.1=0 2/m6064750.4=0,m6079304.1=0,m6079305.1=1 3/m6064750.4=0,m6079304.1=81,m6079305.1=0 4/m6064750.4=0,m6079304.1=41,m6079305.1=20 5/m6064750.4=0,m6079304.1=11,m6079305.1=10 6/m6064750.4=0,m6032e40.4=0,m6032e44.4=0,m6032e48.2=0,m603290c.1=4,m603290d.1=1,m603290e.1=7,m603290f.1=6,m6032910.1=5,m6032911.1=4,m6032912.1=1,m6032913.1=5,m6032914.1=7,m6032915.1=3,m6032916.1=1,m6032917.1=3,m6032918.1=5,m6032919.1=7,m603291a.1=9,m603291b.1=0,m603291c.1=0,m603291d.1=0,m603291e.1=0,m603291f.1=0,m6032925.1=5,m6032926.1=0,m6032927.1=2,m6032928.1=1,m6079304.1=9,m6079305.1=0 7/m6064750.4=0,m6032e40.4=0,m6032e44.4=0,m6032e48.2=0,m603290c.1=4,m603290d.1=1,m603290e.1=7,m603290f.1=6,m6032910.1=5,m6032911.1=4,m6032912.1=1,m6032913.1=5,m6032914.1=7,m6032915.1=3,m6032916.1=1,m6032917.1=3,m6032918.1=5,m6032919.1=7,m603291a.1=9,m603291b.1=0,m603291c.1=0,m603291d.1=0,m603291e.1=0,m603291f.1=0,m6032925.1=5,m6032926.1=0,m6032927.1=2,m6032928.1=1,m6079304.1=1,m6079305.1=8 8/m6064750.4=0,m6032e40.4=40001,m6032e44.4=70006,m6032e48.2=5,m603290c.1=4,m603290d.1=1,m603290e.1=7,m603290f.1=6,m6032910.1=5,m6032911.1=4,m6032912.1=1,m6032913.1=5,m6032914.1=7,m6032915.1=3,m6032916.1=1,m6032917.1=3,m6032918.1=5,m6032919.1=7,m603291a.1=9,m603291b.1=0,m603291c.1=0,m603291d.1=0,m603291e.1=0,m603291f.1=0,m6032925.1=5,m6032926.1=0,m6032927.1=2,m6032928.1=1,m6079304.1=9,m6079305.1=0 9/m6064750.4=0,m6032e40.4=0,m6032e44.4=0,m6032e48.2=0,m6079304.1=89,m6079305.1=0 10/m6064750.4=0,m6079304.1=ff,m6079305.1=ff */
/* difftest-inputs f_d8e0: 1/m6064750.4=0,m6079304.1=8,m6079305.1=0 2/m6064750.4=0,m6079304.1=0,m6079305.1=8 3/m6064750.4=0,m6079304.1=ff,m6079305.1=0 4/m6064750.4=0,m6079304.1=18,m6079305.1=8 5/m6064750.4=0,m6079304.1=9,m6079305.1=80 */
/* difftest-inputs f_d962: 1/m6064750.4=0,m6079304.1=82 2/m6064750.4=0,m6079304.1=84 3/m6064750.4=0,m6079304.1=88 4/m6064750.4=0,m6079304.1=8e 5/m6064750.4=0,m6079304.1=b2 6/m6064750.4=0,m6079304.1=9e 7/m6064750.4=0,m6079304.1=aa 8/m6064750.4=0,m6079304.1=8c */
/* difftest-inputs f_dbe8: 1/m6064750.4=0,m6079304.1=82,m60792de.2=0 2/m6064750.4=0,m6079304.1=94,m60792de.2=1ff 3/m6064750.4=0,m6079304.1=a8,m60792de.2=11 4/m6064750.4=0,m6079304.1=86,m60792de.2=180 5/m6064750.4=0,m6079304.1=be,m60792de.2=ffff 6/m6064750.4=0,m6079304.1=8a,m60792de.2=7f 7/m6064750.4=0,m6079304.1=9c,m60792de.2=8000 8/m6064750.4=0,m6079304.1=a2,m60792de.2=3e3 */
/* difftest-inputs f_e074: 1/m6064750.4=0,m6079304.1=82,m60792dc.2=0 2/m6064750.4=0,m6079304.1=94,m60792dc.2=1 3/m6064750.4=0,m6079304.1=a8,m60792dc.2=6 4/m6064750.4=0,m6079304.1=86,m60792dc.2=7 5/m6064750.4=0,m6079304.1=be,m60792dc.2=ffff 6/m6064750.4=0,m6079304.1=8a,m60792dc.2=2 7/m6064750.4=0,m6079304.1=9c,m60792dc.2=8000 8/m6064750.4=0,m6079304.1=a2,m60792dc.2=5 */
/* difftest-returns f_d394: f_2beca=0..4 (why: its only call is f_2beca(5), and f_2beca(n) returns a value % n (src/f_2bc64.c); the result is the cursor column, which indexes the five digits on the stack) */
/* difftest-returns f_dbe8: f_2bf58=0..63 (why: f_2bf58 returns a slot of the 64-entry sprite table g_606006c, taken from the free list g_606106c (src/gfx_2bf58.c); the result indexes that table) */
/* Non-matching: functionally equivalent C for [0xd394, 0xe39c) (f_d394, f_d8e0, f_d962, f_dbe8, f_e074),
 * linked only by the shiftable build (splits_nonmatching.txt). Not matching coverage.
 * From src/settings_reset_9c0c.c. */
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

/* the code-entry digits start from the original's initializer for them: five
 * zero shorts in the RAM data image, copied as the ROM copies them (two longs
 * and a short) */
extern const long g_6032e40[];

void f_d394(void)
{
	union { long l[3]; short v[6]; } t5;
	short *tab;
	long run;
	short w4, w8, mode;
	short i, x, row, col, found;
	long neg;

	run = 1;
	t5.l[0] = g_6032e40[0];
	t5.l[1] = g_6032e40[1];
	t5.v[4] = ((const short *)g_6032e40)[4];
	tab = t5.v;
	neg = 0;
	while (g_6064750)
		f_2b4e8();
	f_29cc6(0, 4, 0, -0x7ffa);
	g_606006c[g_60792ec].w2c |= 0x8000;
	w4 = 0;
	mode = 128;
	w8 = 0;
	while (run) {
		f_2b4e8();
		f_9ede();
		f_e3e0(10, g_6032e4c, 0);
		f_e3e0(202, g_6032e64, 0);
		f_e3e0(188, g_6032e74, 0);
		f_e3e0(174, g_6032e88, 0);
		f_e3e0(160, g_6032e9c, 0);
		x = 135;
		for (i = 0; i < 5; i++) {
			f_e63a(tab[i], 80, x, 1, 0, 0, 0);
			if (i == w4)
				f_e412(x, 84, g_6032eb4, 0);
			x += 10;
		}
		switch (mode) {
		case 128:
			if ((g_6079304 & 0x80) || (g_6079305 & 0x80)) {
				tab[w4]++;
				if (tab[w4] > 9)
					tab[w4] = 0;
			}
			if ((g_6079304 & 0x40) || (g_6079305 & 0x40)) {
				tab[w4]--;
				if (tab[w4] < 0)
					tab[w4] = 9;
			}
			if ((g_6079304 & 0x20) || (g_6079305 & 0x20)) {
				w4++;
				if (w4 > 4)
					w4 = 0;
			}
			if ((g_6079304 & 0x10) || (g_6079305 & 0x10)) {
				w4--;
				if (w4 < 0)
					w4 = 4;
			}
			if ((g_6079304 & 8) || (g_6079305 & 8)) {
				for (row = 0; row < 4; row++) {
					found = row;
					for (col = 0; col < 5; col++)
						if (tab[col] != g_603290c[row][col])
							found = -1;
					if (found != -1)
						break;
				}
				if (found != -1)
					mode = g_6032925[found];
				else
					mode = 100;
			}
			if ((g_6079304 & 1) || (g_6079305 & 1))
				run = 0;
			break;
		case 0:
			f_e3e0(120, g_6032eb8, 0);
			if (w8 == 1) {
				f_b352();
				f_211a2();
				f_11fe8();
				f_b36e();
				f_21206();
				f_13378();
			} else if (w8 < 150) {
				if ((g_6079304 & 8) || (g_6079305 & 8))
					run = 0;
			} else
				run = 0;
			w8++;
			break;
		case 5:
			f_e3e0(120, g_6032ecc, 0);
			if (w8 == 2) {
				f_11fe8();
				f_13378();
			} else if (w8 < 180) {
				if ((g_6079304 & 8) || (g_6079305 & 8))
					run = 0;
			} else
				run = 0;
			w8++;
			break;
		case 1:
		case 2:
		case 100:
			f_e3e0(120, g_6032ee0, 0);
			if (w8 < 120)
				w8++;
			else {
				w8 = 0;
				f_2beca(5);
				if (neg)
					w4 = -f_2beca(5);
				else
					w4 = f_2beca(5);
				mode = 128;
				for (i = 0; i < 5; i++)
					tab[i] = 0;
			}
			break;
		}
	}
	f_2b4e8();
}

void f_d8e0(void)
{
	long run = 1;

	while (g_6064750)
		f_2b4e8();
	f_29cc6(0, 4, 0, -0x7ffa);
	do {
		f_2b4e8();
		f_9ede();
		f_11fc4();
		f_e3e0(214, g_6032e30, 0);
		if ((g_6079304 & 8) || (g_6079305 & 8))
			run = 0;
	} while (run);
	f_2b4e8();
}

void f_d962(void)
{
	short i, sel, k;
	long run;
	unsigned char b;

	run = 1;
	while (g_6064750)
		f_2b4e8();
	f_29cc6(0, 4, 0, -0x7ffa);
	g_606006c[g_60792ec].w8 = d_3a8f4[0].w0 - 20;
	g_606006c[g_60792ec].w4 = d_3a8f4[0].w2;
	sel = 0;
	while (run) {
		f_2b4e8();
		f_9ede();
		f_2bf58(g_6032eec, 10, (320 - f_2c0d0(g_6032eec)) >> 1);
		for (i = 0; i < 4; i++) {
			k = f_2bf58(d_3a8f4[i].s, d_3a8f4[i].w2, d_3a8f4[i].w0);
			if (i != sel)
				g_606006c[k].b1c = 64;
		}
		f_2bf58(g_6032f04, 200, (320 - f_2c0d0(g_6032f04)) >> 1);
		switch (sel) {
		case 2:
			f_2bf58(g_6032f1c, 210, (320 - f_2c0d0(g_6032f1c)) >> 1);
			break;
		case 3:
			f_2bf58(g_6032f34, 210, (320 - f_2c0d0(g_6032f34)) >> 1);
			break;
		default:
			f_2bf58(g_6032f4c, 210, (320 - f_2c0d0(g_6032f4c)) >> 1);
			break;
		}
		g_606006c[g_60792ec].w4 = d_3a8f4[sel].w2;
		b = g_6079304;
		if (b & 0x80) {
			if (sel-- == 0)
				sel = 3;
		}
		if (b & 0x40) {
			if (sel++ == 3)
				sel = 0;
		}
		if (b & 0xe) {
			switch (sel) {
			case 0:
				f_dbe8();
				break;
			case 1:
				f_e074();
				break;
			case 2:
				g_60792de = 0x180;
				g_60792dc = 1;
				break;
			case 3:
				run = 0;
				break;
			}
			g_606006c[g_60792ec].w8 = d_3a8f4[0].w0 - 20;
		}
	}
}

void f_dbe8(void)
{
	short h[4];
	short i, j, sel, c, k;
	long run;
	unsigned char b;

	run = 1;
	while (g_6064750)
		f_2b4e8();
	f_29cc6(0, 4, 0, -0x7ffa);
	g_606006c[g_60792ec].w8 = d_3a984[0].w0 - 20;
	g_606006c[g_60792ec].w4 = d_3a984[0].w2;
	sel = 0;
	while (run) {
		f_2b4e8();
		f_9ede();
		f_2bf58(g_6032f64, 10, (320 - f_2c0d0(g_6032f64)) >> 1);
		for (i = 0; i < 5; i++) {
			k = f_2bf58(d_3a984[i].s, d_3a984[i].w2, d_3a984[i].w0);
			if (i != sel)
				g_606006c[k].b1c = 64;
		}
		h[0] = f_2bf58(d_3aa38[g_60792de & 31], d_3a984[0].w2, 170);
		h[1] = f_2bf58(d_3aa80[(g_60792de >> 5) & 3], d_3a984[1].w2, 170);
		h[2] = f_2bf58(d_3aa8c[(g_60792de >> 7) & 1], d_3a984[2].w2, 170);
		h[3] = f_2bf58(d_3aa94[(g_60792de >> 8) & 1], d_3a984[3].w2, 170);
		for (j = 0; j < 4; j++) {
			if (j != sel)
				g_606006c[h[j]].b1c = 64;
		}
		f_2bf58(g_6032f84, 200, (320 - f_2c0d0(g_6032f84)) >> 1);
		if (sel == 4)
			f_2bf58(g_6032fa4, 210, (320 - f_2c0d0(g_6032fa4)) >> 1);
		else
			f_2bf58(g_6032fd4, 210, (320 - f_2c0d0(g_6032fd4)) >> 1);
		g_606006c[g_60792ec].w4 = d_3a984[sel].w2;
		b = g_6079304;
		if (b & 0x80) {
			if (sel-- == 0)
				sel = 4;
		}
		if (b & 0x40) {
			if (sel++ == 4)
				sel = 0;
		}
		if (b & 0x10) {
			switch (sel) {
			case 0:
				c = (g_60792de & 31) - 1;
				if (c < 0)
					c = 17;
				g_60792de &= ~31;
				g_60792de |= c;
				break;
			case 1:
				c = ((g_60792de >> 5) & 3) - 1;
				if (c < 0)
					c = 2;
				g_60792de &= ~0x60;
				g_60792de |= c << 5;
				break;
			case 2:
				c = ((g_60792de >> 7) & 1) - 1;
				if (c < 0)
					c = 1;
				g_60792de &= ~0x80;
				g_60792de |= c << 7;
				break;
			case 3:
				c = ((g_60792de >> 8) & 1) - 1;
				if (c < 0)
					c = 1;
				g_60792de &= ~0x100;
				g_60792de |= c << 8;
				break;
			}
		} else if (b & 0x20) {
			switch (sel) {
			case 0:
				c = ((g_60792de & 31) + 1) % 18;
				g_60792de &= ~31;
				g_60792de |= c;
				break;
			case 1:
				c = (((g_60792de >> 5) & 3) + 1) % 3;
				g_60792de &= ~0x60;
				g_60792de |= c << 5;
				break;
			case 2:
				c = (((g_60792de >> 7) & 1) + 1) % 2;
				g_60792de &= ~0x80;
				g_60792de |= c << 7;
				break;
			case 3:
				c = (((g_60792de >> 8) & 1) + 1) % 2;
				g_60792de &= ~0x100;
				g_60792de |= c << 8;
				break;
			}
		}
		if ((g_6079304 & 14) && sel == 4)
			run = 0;
	}
}

void f_e074(void)
{
	short h[2];
	short i, j, sel, c, k;
	long run;
	unsigned char b;

	run = 1;
	while (g_6064750)
		f_2b4e8();
	f_29cc6(0, 4, 0, -0x7ffa);
	g_606006c[g_60792ec].w8 = d_3aa9c[0].w0 - 20;
	g_606006c[g_60792ec].w4 = d_3aa9c[0].w2;
	sel = 0;
	while (run) {
		f_2b4e8();
		f_9ede();
		f_2bf58(g_6032ff8, 10, (320 - f_2c0d0(g_6032ff8)) >> 1);
		for (i = 0; i < 3; i++) {
			k = f_2bf58(d_3aa9c[i].s, d_3aa9c[i].w2, d_3aa9c[i].w0);
			if (i != sel)
				g_606006c[k].b1c = 64;
		}
		h[0] = f_2bf58(d_3ab08[g_60792dc & 1], d_3aa9c[0].w2, 180);
		h[1] = f_2bf58(d_3ab10[(g_60792dc >> 1) & 3], d_3aa9c[1].w2, 180);
		for (j = 0; j < 2; j++) {
			if (j != sel)
				g_606006c[h[j]].b1c = 64;
		}
		f_2bf58(g_6032f84, 200, (320 - f_2c0d0(g_6032f84)) >> 1);
		if (sel == 2)
			f_2bf58(g_6032fa4, 210, (320 - f_2c0d0(g_6032fa4)) >> 1);
		else
			f_2bf58(g_6032fd4, 210, (320 - f_2c0d0(g_6032fd4)) >> 1);
		g_606006c[g_60792ec].w4 = d_3aa9c[sel].w2;
		b = g_6079304;
		if (b & 0x80) {
			if (sel-- == 0)
				sel = 2;
		}
		if (b & 0x40) {
			if (sel++ == 2)
				sel = 0;
		}
		if (b & 0x10) {
			switch (sel) {
			case 0:
				c = (g_60792dc & 1) - 1;
				if (c < 0)
					c = 1;
				g_60792dc &= ~1;
				g_60792dc |= c;
				break;
			case 1:
				c = ((g_60792dc >> 1) & 3) - 1;
				if (c < 0)
					c = 2;
				g_60792dc &= ~6;
				g_60792dc |= c << 1;
				break;
			}
		} else if (b & 0x20) {
			switch (sel) {
			case 0:
				c = ((g_60792dc & 1) + 1) % 2;
				g_60792dc &= ~1;
				g_60792dc |= c;
				break;
			case 1:
				c = (((g_60792dc >> 1) & 3) + 1) % 3;
				g_60792dc &= ~6;
				g_60792dc |= c << 1;
				break;
			}
		}
		if ((g_6079304 & 14) && sel == 2)
			run = 0;
	}
}

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
