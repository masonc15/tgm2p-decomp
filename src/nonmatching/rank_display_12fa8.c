/* rom: 0x12fa8 len: 0x240 func: f_12fa8 flags: -macsave=1 -optimize=1 -speed */
/* Non-matching: functionally equivalent C for [0x12fa8, 0x131e8) (f_12fa8),
 * linked only by the shiftable build (splits_nonmatching.txt). Not matching coverage.
 * From src/rank_display_12ad4.c. */
extern char d_a761c[];
extern char d_a7634[];
extern char d_a764c[];
extern char d_a7664[];
extern char d_a767c[];
struct frame { char pad[12]; };
struct pair { long a, b; };
struct quad { short a, b, c, d; };
extern struct pair g_6079454[3];
extern short g_607946c[3];
extern struct pair g_6079474[3];
extern struct pair g_607948c[3];
extern struct quad g_60794a4[3];
extern struct pair d_3b064[3];
extern struct pair d_3b07c[3];
extern struct pair d_3b094[3];
extern struct quad d_3b0ac[3];
extern struct frame *d_3b13c[];
extern struct frame *d_3b0c4[];
extern void f_1159c(struct frame *, short, short, short, short);
extern void f_185f0(struct frame *, short, short, char, short);

void f_120c0(void);

void f_121fe(unsigned char *s, short x, short y, char c, short e);

void f_12252(unsigned long n, short x, short y);

void f_122cc(unsigned long n, short x, short y);

void f_12380(unsigned long t, short x, short y);

void f_124a8(short x, short y);

void f_12530(short x, short y);

void f_12570(short x, short y);

void f_125c8(short x, short y);

struct rec { char name[4]; unsigned long v; };
struct ent { char name[4]; unsigned short a, b; };
struct data {
	struct rec *r;             /* 0x14 */
	void *b;                   /* 0x18 */
	short x;                   /* 0x1c */
	short y;                   /* 0x1e */
	short wc;                  /* 0x20 */
	short rank;                /* 0x22 */
};
struct task {
	char pad0[6];
	short tick;                /* 0x06 */
	short mode;                /* 0x08 */
	short wait;                /* 0x0a */
	short delay;               /* 0x0c */
	short pad1;
	void (*func)(struct task *); /* 0x10 */
	struct data d;             /* 0x14 */
};
extern struct task *f_17614(void);
extern void f_11254(long, short, short, char, short, short, long, short);
extern void f_11680(struct frame *, short, short, short, short, short, short, short);
extern void f_216d0(short, short, short);
extern struct frame *d_3b0ec[];
extern short d_38f58[];
extern char g_6033644[];

struct state {
	char pad0[0x44];
	struct pair t44[19];
	struct quad tdc[3];
	short tf4[3];
};
extern struct state g_6065650;

void f_12608(struct task *t);

void f_12a58(void *r, void *b, short x, char c, short rank, short mode, short wait, short delay);

#define E g_6065650

void f_12ad4(signed char m);

extern void f_2164e();
extern long f_2b4e8(void);
extern void f_247ee(void);
extern void f_29cc6(long, long, long, long);
extern long f_149b4(void);
extern void f_175c4(void);
extern void f_124a8(short, short);
extern void f_12530(short, short);
extern void f_12570(short, short);
extern void f_125c8(short, short);
extern long g_6064750;
extern unsigned char g_606475e[2];
extern long g_6079538;
extern short g_6060022;

long f_12fa8(signed char m)
{
	short t, done;

	f_2164e((done = 0, t = 0));

	f_12ad4(m);
	for (;;) {
		if (f_2b4e8() != 0) {
			f_247ee();
			return 11;
		}
		t++;
		if (g_6064750 == 0) {
			if (done) {
				switch (g_6079538) {
				case 7:
					return 8;
				case 8:
					return 9;
				case 9:
				case 10:
				default:
					return 3;
				}
			}
			if (t >= 360) {
				f_29cc6(2, 20, 0, 6);
				done = 1;
			}
		}
		if (g_6060022 == 0 && f_149b4() != 0) {
			f_247ee();
			return 12;
		}
		if (g_6064750 == 0) {
			if ((g_606475e[0] & 16) || (g_606475e[1] & 16)) {
				m = (m + 3) % 4;
				f_175c4();
				f_12ad4(m);
				f_29cc6(0, 20, 0, 6);
				t = 0;
			} else if ((g_606475e[0] & 32) || (g_606475e[1] & 32)) {
				m = (m + 1) % 4;
				f_175c4();
				f_12ad4(m);
				f_29cc6(0, 20, 0, 6);
				t = 0;
			}
		}
		switch (m) {
		case 0:
			f_185f0((struct frame *)((long)d_a761c), 14, 10, 0, 110);
			f_124a8(35, 0);
			f_185f0((struct frame *)((long)d_a767c), 134, 10, 0, 110);
			f_124a8(155, 0);
			break;
		case 1:
			f_185f0((struct frame *)((long)d_a7634), 14, 10, 0, 110);
			f_12530(35, 0);
			f_185f0((struct frame *)((long)d_a767c), 134, 10, 0, 110);
			f_12530(155, 0);
			break;
		case 2:
			f_185f0((struct frame *)((long)d_a764c), 14, 10, 0, 110);
			f_12570(37, 0);
			break;
		case 3:
			f_185f0((struct frame *)((long)d_a7664), 14, 10, 0, 110);
			f_125c8(35, 0);
			f_185f0((struct frame *)((long)d_a767c), 134, 10, 0, 110);
			f_125c8(155, 0);
			break;
		}
	}
}
