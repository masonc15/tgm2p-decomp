/* rom: 0x8b86 len: 0x110 func: f_8b86 flags: -macsave=1 -optimize=1 -speed */
/* Non-matching: functionally equivalent C for [0x8b86, 0x8c96) (f_8b86),
 * linked only by the shiftable build (splits_nonmatching.txt). Not matching coverage.
 * From src/field_ready_8b20.c. */
extern char d_68ad0[];
extern char d_68b10[];
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
extern short g_6060036[1];
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
extern void f_88b4(struct field *);
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

short f_88fe(void);
short f_8c96(void);
void f_96ac(void);
void f_8b20(void);

unsigned char f_8b7c(void);

short f_8b86(unsigned char id)
{
	struct field *a = &g_6064898[0];
	struct field *b = &g_6064898[1];
	struct field *f = &g_6064898[*(&id)];

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
	f_2af5e(15, 1, ((long)d_68ad0));
	f_2af5e(14, 1, ((long)d_68b10));
	while (g_6064750)
		if (f_2b4e8())
			return 7;
	return 0;
}
