/* rom: 0xe3b8 len: 0x28 func: f_e3b8 context-rom: 0xe074 literal-tail-calls: 1 flags: -macsave=1 -optimize=1 -speed */
/* Genuine menu predecessor and width helper are excluded context.
 * Only palette setup and its complete shared pool [e3b8,e3e0) are retained.
 * The buttons pointer is consumed at both original input reads.
 */
struct obj {
	char pad0[4];
	short x;                   /* 0x04 */
	char pad6[2];
	short y;                   /* 0x08 */
	char pad10[0x1c - 0xa];
	unsigned char b28;         /* 0x1c */
	char pad29[0x2c - 0x1d];
	unsigned short w44;        /* 0x2c */
	char pad46[0x40 - 0x2e];
};
struct st {
	long l0, l4, l8, lc;
	unsigned char b10, b11, b12, b13, b14;
	char pad;
	short w16[4];
	short w1e;
};
struct spr { char pad[4]; short w4; short w6; };
struct s2t { struct spr *spr; long b; long c; long d; };
struct rec {
	short w0;
	short w2;
	char s[32];
};
extern struct obj g_606006c[];
extern struct st g_6066188;
#define S g_6066188
extern long g_6064750;
extern unsigned char g_6064754, g_6064755, g_6079304, g_6079305;
extern short g_60792ec, g_60792de, g_60792dc;
extern short g_6033014[];
extern long d_3ab1c[];
extern char d_67910[];
extern void f_2b4e8(void);
extern void f_29cc6(long, long, long, long);
extern void f_9ede(void);
extern void f_acdc(short *);
extern void f_25658(short);
extern long f_25f6e(void);
extern void f_2616a(long, void *);
extern void f_26f64(long, long);
extern void f_2774a(long, long);
extern void f_25fde(long);
extern void f_2639e(long, short, short);
extern void f_1865e(long, short, short, short, short, short, short, char);
extern void f_2af5e(int, int, void *);
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

void f_e3e0(short y, char *s, char flag);
void f_e412(short x, short y, char *s, char flag);
void f_e63a(long v, short y, long x, short n, long z, char m, char flag);
void f_dbe8(void);
void f_e074(void);

void f_cac8(void);
void f_cf68(void);
void f_d28c(unsigned long t, char *s);
void f_d33a(unsigned short x, char *s);
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
void f_e520(short x, short y, char *s, char flag);
void f_e63a(long v, short y, long x, short n, long z, char m, char flag);
void f_e074(void)
{
	short h[2];
	int i;
short j;
int sel;
short c;
short k;
	long run;
	unsigned char b;
	register unsigned char *buttons;

	run = 1;
	while (g_6064750)
		f_2b4e8();
	f_29cc6(0, 4, 0, -0x7ffa);
	g_606006c[g_60792ec].y = d_3aa9c[0].w0 - 20;
	g_606006c[g_60792ec].x = d_3aa9c[0].w2;
	sel = 0;
	while (run) {
		f_2b4e8();
		f_9ede();
		f_2bf58(g_6032ff8, 10, (320 - f_2c0d0(g_6032ff8)) >> 1);
		for (i = 0; i < 3; i++) {
			k = f_2bf58(d_3aa9c[i].s, d_3aa9c[i].w2, d_3aa9c[i].w0);
			if (i != sel)
				g_606006c[k].b28 = 64;
		}
		h[0] = f_2bf58(d_3ab08[g_60792dc & 1], d_3aa9c[0].w2, 180);
		h[1] = f_2bf58(d_3ab10[(g_60792dc >> 1) & 3], d_3aa9c[1].w2, 180);
		for (j = 0; j < 2; j++) {
			if (j != sel)
				g_606006c[h[j]].b28 = 64;
		}
		f_2bf58(g_6032f84, 200, (320 - f_2c0d0(g_6032f84)) >> 1);
		if (sel == 2)
			f_2bf58(g_6032fa4, 210, (320 - f_2c0d0(g_6032fa4)) >> 1);
		else
			f_2bf58(g_6032fd4, 210, (320 - f_2c0d0(g_6032fd4)) >> 1);
		g_606006c[g_60792ec].x = d_3aa9c[sel].w2;
		buttons = &g_6079304;
		b = *buttons;
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
		if ((*buttons & 14) && sel == 2)
			run = 0;
	}
}

short f_e39c(char *s)
{
	short w = 0;

	while (*s)
		w += g_6033014[*s++];
	return w;
}

void f_e3b8(void)
{
	f_2af5e(81, 1, d_67910);
	f_2af5e(82, 1, d_67910);
}

