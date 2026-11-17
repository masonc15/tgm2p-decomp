/* rom: 0x15794 len: 0x77c func: f_15794 flags: -macsave=1 -optimize=1 -speed */
/* Non-matching, functionally equivalent C (shiftable build only).
 * f_15794: demo screen for g_6079538 modes 4-6: sets up the screen, both
 * fields and the mode's table, then loops polling f_2ba08 (on the table at
 * g_6060008) and f_149b4, scrolling the d_aa790 banner and blinking the
 * prompt for about 1140 frames; returns 11 or 12 when those fire, else the
 * mode's next state (7, 12, 10).
 * The stores to g_6079299 come before those to g_6064880 only so SHC leaves
 * the same dead values in r5/r6 at the first f_1340 call as the ROM: the
 * tester's arity scan over-counts f_1340's arguments and compares them. */
struct field {
	char pad[0x308];
	unsigned long flags308;    /* 0x308 */
	unsigned short mode;       /* 0x30c */
	char pad2[0x3b4 - 0x30e];
};
struct frame { char pad[12]; };
/* The demo table and counters live in the frame, as in the original: the
 * mode switch leaves them unset for other modes. */
struct demo {
	char *tbl;
	short cnt;
	short fr;
};

extern long g_6064750, g_606487c, g_6079538;
extern unsigned long g_6060008;
extern char g_607953c;
extern short g_60661a6;
extern struct field g_6064898[2];
extern struct { char pad[7]; char b7; char b8; char pad2[5]; short we; } g_6064884;
extern short g_606003c;
extern long g_60657a0;
extern char g_6079299;
extern unsigned long g_6064880;
extern char g_60341bf[], g_60337e7[], g_6034b97[], g_6033790[], g_60337d0[];
extern char g_6033748[], g_6033788[], d_67910[];
extern char d_67110[], d_67390[], d_67610[], d_68410[], d_63ed0[], d_63e90[];
extern struct frame d_aa790[];
extern void f_2b1cc(void);
extern int f_2b4e8(void);
extern void f_21206(void);
extern void f_29cc6(short, short, long, long);
extern short f_149b4(void);
extern void f_29f70(void);
extern void f_2ed06(void);
extern void f_2ed3e(void);
extern void f_2eeaa(int);
extern void f_2ece0(void);
extern void f_2edb8(void);
extern void f_2f2cc(void);
extern void f_2f3dc(void);
extern void f_2ee38(void);
extern void f_175c4(void);
extern void f_85d8(void);
extern void f_2af5e(int, int, void *);
extern void f_1715c(void);
extern void f_20250(void);
extern void f_1340(int);
extern void f_2f7a(struct field *);
extern void f_2a576(int, char *, char *, int, int, int, int);
extern void f_2774a(short, int);
extern void f_29c18(short);
extern int f_2ba08(char *, char *);
extern void f_1278(void);
extern void f_185f0(struct frame *, int, int, int, int);
extern void f_96ac(void);
extern void f_e750(int, int, char *, int);
extern int f_2beca(int);
extern void f_103d0(void);
extern int f_e39c(char *);
extern void f_e490(int, int, char *, int, int);
extern void f_247ee(void);

#define LEAVE() (f_2b1cc(), f_1278(), g_606487c = 0, f_2ed3e(), f_2ed06())

int f_15794(void)
{
	struct demo d;
	short code;
	short tog;
	short side;
	short n20;
	unsigned long fr;
	struct field *p0;
	struct field *p1;
	long save;
	short xx;
	int off, w;
	unsigned short ucnt;
	char *p;

	code = 0;
	g_606487c = 1;
	*(char *)0x2405ffe8 = 19;
	*(char *)0x2405ffe9 = 100;
	f_2b1cc();
	if (f_2b4e8())
		return 11;
	f_21206();
	f_29cc6(4, 0, 0, 0);
	while (g_6064750) {
		if (f_2b4e8())
			return 11;
		if (f_149b4() == 1) {
			g_606487c = 0;
			return 12;
		}
	}
	f_29f70();
	f_2ed06();
	f_2ed3e();
	f_2eeaa(7);
	f_2ece0();
	if (g_606003c == 0)
		f_2edb8();
	f_2f2cc();
	f_2f3dc();
	f_2ee38();
	f_175c4();
	f_85d8();
	g_6064884.b7 = 0;
	g_6064884.b8 = 0;
	f_2af5e(226, 10, d_67110);
	f_2af5e(236, 10, d_67390);
	f_2af5e(246, 10, d_67610);
	f_2af5e(157, 1, d_68410);
	f_1715c();
	p0 = &g_6064898[0];
	p1 = &g_6064898[1];
	p0->flags308 = 0x1004;
	p1->flags308 = 0x1004;
	g_6064884.we = -1;
	switch (g_6079538) {
	case 4:
		d.tbl = g_60337e7;
		d.cnt = 7;
		d.fr = 615;
		g_6079299 = 1;
		g_6064880 = 1;
		p0->mode = (p0->mode & 0xfff0) | 2;
		p1->mode = (p1->mode & 0xfff0) | 1;
		break;
	case 5:
		d.tbl = g_6034b97;
		d.cnt = 555;
		d.fr = 555;
		g_6079299 = 10;
		g_6064880 = 2;
		p0->mode = (p0->mode & 0xfff0) | 10;
		p1->mode = (p1->mode & 0xfff0) | 9;
		f_2af5e(15, 1, d_63ed0);
		f_2af5e(14, 1, d_63e90);
		f_20250();
		break;
	case 6:
		d.tbl = g_60341bf;
		d.cnt = 7;
		d.fr = 615;
		g_6079299 = 1;
		g_6064880 = 4;
		p0->mode = (p0->mode & 0xfff0) | 4;
		p1->mode = (p1->mode & 0xfff0) | 4;
		break;
	}
	save = g_60657a0;
	g_60657a0 = d.cnt;
	f_1340(0);
	g_60657a0 = d.fr;
	f_1340(1);
	g_60657a0 = save;
	f_2f7a(p0);
	f_2f7a(p1);
	f_2af5e(179, 1, d_67910);
	f_2af5e(202, 1, g_6033790);
	f_2a576(159, g_6033748, d_67910, 1, 64, 1, 63);
	tog = 0;
	n20 = 0;
	f_2774a(g_60661a6, 1);
	f_29c18(6);
	f_29cc6(0, 20, 0, 6);
	g_6060008 = 0;
	fr = 0;
	d.cnt = 0;
	for (;;) {
		if (p = d.tbl + g_6060008, f_2ba08(p, p + 1260)) {
			code = 11;
			fr = 1141;
		}
		if (f_149b4() == 1) {
			LEAVE();
			return 12;
		}
		d.cnt++;
		if (d.cnt >= 10 && g_6064750 == 0)
			break;
	}
	f_29f70();
	f_185f0(d_aa790, 108, 164, 157, 115);
	d.cnt = 0;
	side = 0;
	for (;;) {
		if (p = d.tbl + g_6060008, f_2ba08(p, p + 1260)) {
			code = 11;
			fr = 1141;
		}
		fr++;
		f_96ac();
		f_e750(285, 218, g_6033788, 1);
		n20++;
		if ((unsigned short)n20 % 64 == 0) {
			tog = tog == 0;
			if ((unsigned short)tog)
				f_2a576(159, g_6033748, d_67910, 1, 68, 1, 63);
			else
				f_2a576(159, g_6033748, d_67910, 1, 64, 1, 63);
		}
		if (f_149b4()) {
			code = 12;
			fr = 1141;
		}
		g_60657a0 += f_2beca(357) + 1;
		f_103d0();
		off = 164 - d.cnt;
		xx = 484 - d.cnt;
		ucnt = d.cnt;
		if (side) {
			f_185f0(d_aa790, 108, off, 202, 115);
			f_185f0(d_aa790, 108, xx, 157, 115);
			w = f_e39c(g_60337d0);
			f_e490(160 - w / 2 - ucnt, 112, g_60337d0, 179, 0);
		} else {
			f_185f0(d_aa790, 108, off, 157, 115);
			f_185f0(d_aa790, 108, xx, 202, 115);
			w = f_e39c(g_60337d0);
			f_e490(480 - w / 2 - ucnt, 112, g_60337d0, 179, 0);
		}
		d.cnt++;
		if ((unsigned short)d.cnt > 320) {
			side ^= 1;
			d.cnt -= 320;
		}
		if (fr <= 1140)
			continue;
		p0->flags308 |= 0x80000000;
		p1->flags308 |= 0x80000000;
		if (code) {
			g_60657a0 += f_2beca(1192) + 1;
			LEAVE();
			return code;
		}
		g_60657a0 += f_2beca(1192) + 1;
		while (g_6064750) {
			if (p = d.tbl + g_6060008, f_2ba08(p, p + 1260)) {
				LEAVE();
				return 11;
			}
		}
		f_247ee();
		f_29cc6(2, 10, 0, 6);
		while (g_6064750) {
			if (p = d.tbl + g_6060008, f_2ba08(p, p + 1260)) {
				LEAVE();
				return 11;
			}
		}
		LEAVE();
		switch (g_6079538) {
		case 4:
			return 7;
		case 5:
			return 12;
		case 6:
			g_607953c = 0;
			return 10;
		}
	}
}
