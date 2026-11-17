/* rom: 0x1050e len: 0xac func: f_1050e flags: -macsave=1 -optimize=1 -speed */
/* Non-matching: functionally equivalent C for [0x1050e, 0x105ba) (f_1050e),
 * linked only by the shiftable build (splits_nonmatching.txt). Not matching coverage.
 * From src/hud_ranges_f958.c. */
extern char d_a6ecc[];
extern char d_a6ed8[];
extern char d_a6efc[];
extern char d_a6f20[];
struct frame { char pad[12]; };
struct pl {
	char pad0[0xdf];
	unsigned char bdf;         /* 0xdf */
	char pad1[0x2f4 - 0xe0];
	struct pl *owner;          /* 0x2f4 */
	char pad1b[0x308 - 0x2f8];
	unsigned long l308;        /* 0x308 */
	unsigned short w30c;       /* 0x30c */
	unsigned char b30e;        /* 0x30e */
	char pad2[0x314 - 0x30f];
	short pos[2];              /* 0x314 */
	short w318, w31a, w31c, w31e, w320;
	unsigned short w322;
	char pad3[0x330 - 0x324];
	long l330;                 /* 0x330 */
	char pad4[0x339 - 0x334];
	unsigned char b339;        /* 0x339 */
	char pad5[0x350 - 0x33a];
	unsigned long l350;        /* 0x350 */
	char pad6[0x358 - 0x354];
	unsigned long l358;        /* 0x358 */
	char pad7[0x379 - 0x35c];
	unsigned char b379;        /* 0x379 */
	char pad8[0x37e - 0x37a];
	unsigned char b37e;        /* 0x37e */
	char b37f;
	char b380;                 /* 0x380 */
	char pad9[0x38a - 0x381];
	unsigned short w38a;       /* 0x38a */
	unsigned short w38c;       /* 0x38c */
	unsigned short w38e;       /* 0x38e */
	unsigned short w390;       /* 0x390 */
	char padA[0x3b4 - 0x392];
};
struct ent {
	char pad0;
	char b1;
	char pad1[0x80c - 2];
	struct pl *owner;          /* 0x80c */
	long pad2;
	struct ent *next;          /* 0x814 */
};
struct counter {
	char pad0[7];
	unsigned char count;       /* 0x7 */
	unsigned char player;      /* 0x8 */
};
struct pair { short a, b; };
extern long g_6060008, g_606487c, g_6064880;
extern short g_6060038, g_6060022, g_6060040, g_606003e;
extern short g_6060034[];
struct gs { short pad[5]; short w10, w12, w14, w16; };
extern struct gs gs_;
extern unsigned short g_6060000;
extern unsigned char g_6064888[], g_6079374[];
extern char g_6064760, g_6064761, g_6064762, g_6064763, g_6064764;
extern char g_6033620[], g_6033628[], g_6033630[], g_6033638[];
extern struct counter g_6064884;
extern struct pl g_6064898[];
extern short d_36314[], d_38f58[], d_3ae6c[];
extern struct pair d_36304[];
extern long d_3ade4[], d_3ad6c[];
extern long d_3ad44[];
extern struct frame *d_3adbc[], *d_3ae38[], *d_3ae58[];
extern unsigned short d_3ae4c[];
extern char d_a6cd4[], d_a6ce0[], d_a6cec[], d_a71b4[];
extern char d_a79ac[], d_a78f8[], d_a78ec[], d_a78e0[], d_a79b8[];
extern struct frame d_aa934[], d_aac34[], d_aacac[], d_a6d4c;
extern unsigned char f_8b7c(void);
extern char f_18ed4(unsigned short);
extern int f_2da90(long, long);
extern int f_e39c(char *);
extern void f_e490(int, int, char *, short, char);
extern void f_185f0(struct frame *, short, short, char, short);
extern void f_11680(long, short, short, short, short, short, short, short);
extern void f_1159c(long, short, short, unsigned char, short);
extern void f_214ec(struct pl *);
extern void f_f958(struct pl *, short);
extern void f_fa78(struct pl *);
extern void f_fac8(struct pl *, short, short);
extern void f_fbdc(struct pl *, unsigned short, long, long, char);
extern void f_fc8c(unsigned long, short, long);
extern void f_fdd8(struct pl *, long);
extern void f_fef6(struct pl *);
extern void f_ffee(struct pl *);
extern void f_100a4(unsigned char);
extern void f_101b8(struct pl *);
extern void f_10942(struct pl *);
extern void f_10d94(struct pl *, char *);
extern void f_11254(long, short, int, char, short, short, long, unsigned char);
#define FIELD(p) ((struct pl *)((char *)g_6064898 + (short)((p) * 0x3b4)))
#define FB(p) ((struct pl *)((char *)fb + (short)((p) * 0x3b4)))

void f_f958(struct pl *p, short b);

void f_fa78(struct pl *p);

void f_fac8(struct pl *p, short b, short c);

void f_fbdc(struct pl *f, unsigned short b, long c, long d, char e);

void f_fc8c(unsigned long t, short x, long y);

void f_fdd8(struct pl *p, long y);

void f_fef6(struct pl *p);

void f_ffee(struct pl *p);

void f_100a4(unsigned char on_field);

void f_101b8(struct pl *p);

void f_10228(struct ent *p);

void f_103d0(void);

void f_1050e(struct pl *p)
{
	int a, b, v;

	if (!(g_6060008 & 0x60))
		return;
	if (f_2da90(p->b30e, 0) || g_6060034[6] == 2) {
		if (p->b30e == 0)
			f_1159c((long)((struct frame *)((long)d_a6ecc)), 100, p->pos[0], 0, 125);
		else
			f_1159c((long)((struct frame *)((long)d_a6ed8)), 100, p->pos[0], 0, 125);
		return;
	}
	a = g_6064760;
	b = g_6064761;
	if (g_6060034[5] == 0)
		v = a + b;
	else if (!p->b30e)
		v = a;
	else
		v = b;
	if ((unsigned char)v)
		f_1159c((long)((struct frame *)((long)d_a6f20)), 100, p->pos[0], 0, 125);
	else
		f_1159c((long)((struct frame *)((long)d_a6efc)), 100, p->pos[0], 0, 125);
}

void f_105ba(long arg0, short x);

void f_1065e(unsigned char a, short b, short c);

void f_106a6(unsigned char a, short b, short c);

void f_106ee(unsigned char a, char b, short c, short d, short e);

void f_10788(void);

void f_10942(struct pl *p);

void f_10b28(struct pl *p);

void f_10d94(struct pl *p, char *out);

void f_10de6(void);

void f_11254(long val, short a, int x, char c, short e, short digits, long z, unsigned char mode);

void f_11336(long val, short a, int x, short digits, long z, unsigned char mode);

void f_11424(long val, short a, int x, char c, short e, short digits, long z, unsigned char mode);

