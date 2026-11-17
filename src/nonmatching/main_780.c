/* rom: 0x780 len: 0x988 func: f_780 flags: -macsave=1 -optimize=1 -speed */
/* Non-matching, functionally equivalent C for f_780, the program's entry
 * point (entered from the boot block). It initialises the subsystems and
 * the video registers (f_1108), reloads the save data, shows the
 * WORK RAM / VIDEO RAM / EEP-ROM check screen until its countdown ends
 * (or, once the test switch was seen, until f_30a44 reports done), then,
 * unless the service switch is held, one of the warning screens chosen by
 * g_6060004 followed by the title logo for up to 500 frames, and finally
 * tail-calls the top-level loop f_9b90 (main_loop). */

struct rom_hdr {
	char pad0[28];
	long l28;                  /* 0x1c */
};

struct hs {
	unsigned long l0;
	unsigned long l4;
};

struct obj {
	char pad0[0x2c];
	unsigned short w2c;        /* 0x2c */
	char pad2e[0x40 - 0x2e];
};

#define IN0     (*(volatile unsigned char *)0x23000000)
#define IN_SYS  (*(volatile unsigned char *)0x23000003)
#define IN_DIP  (*(volatile unsigned char *)0x23000004)
#define VREG_EE (*(volatile unsigned char *)0x2405ffee)

extern struct rom_hdr *rom_hdr_ptr;        /* 0x2004002c */

extern void f_2b430(void);
extern void f_2ef3e(void);
extern void f_2e6fc(short);
extern void f_23f10(void);
extern void f_2d2bc(void);
extern void f_1108(void);
extern void f_25658(void);
extern void f_1715c(void);
extern void f_175c4(void);
extern void f_2a08c(void);
extern void f_2491c(void);
extern void f_1260(void);
extern void f_120c0(void);
extern void f_2488c(void);
extern void f_2b3e8(long);
extern void f_2af5e(unsigned char, unsigned char, long *);
extern void f_2c3d0(unsigned char);
extern void f_29c50(long);
extern void f_29bf8(long);
extern void f_b352(void);
extern void f_211a2(void);
extern void f_11fe8(void);
extern void f_b36e(void);
extern void f_21206(void);
extern void f_13378(void);
extern void f_2140e(long);
extern void f_3003c(void);
extern int f_b2f4(void);
extern int f_212f8(void);
extern int f_13380(void);
extern int f_213da(long);
extern int f_30a44(void);
extern int f_2b4e8(void);
extern void f_2c28c(void);
extern void f_e3b8(void);
extern void f_e412(short, short, char *, char);
extern void f_e520(short, short, char *, int);
extern void f_e3e0(short, char *, char);
extern short f_2528c(void);
extern void f_253bc(short, short, short, long);
extern void f_29f94(unsigned long, unsigned long, unsigned char, unsigned char);
extern short f_2bf58(void *, short, short);
extern void f_185f0(void *, short, short, char, short);
extern void f_252f8(short);
extern void f_9b90(void);

extern long g_6060008;
extern unsigned short g_606434c;
extern long g_6065644;
extern short g_606564c;
extern short g_6060022;
extern long *g_607926c;
extern long g_60775a8[];
extern struct hs g_6065698[];
extern unsigned long g_6065750[];
extern unsigned long g_6065778[];
extern short g_6060034[];
extern long g_6079268;
extern long g_6060004;
extern unsigned char g_6064754;
extern unsigned char g_6064758;
extern unsigned char g_6064759;
extern struct obj g_606006c[];
extern long g_6079538;

/* palettes and strings in the initialised data */
extern char g_6030c7c[];
extern char g_6030cbc[];
extern char g_6030cfc[];                   /* "WORK RAM" */
extern char g_6030d08[];                   /* "O" */
extern char g_6030d0c[];                   /* "X" */
extern char g_6030d10[];                   /* "VIDEO RAM" */
extern char g_6030d1c[];                   /* "EEP-ROM" */
extern char g_6030d24[];                   /* "- Warning! -" */
extern char *g_6030f34[];
extern char *g_6030f50[];
extern char *g_6030f68[];
extern char *g_6030f7c[];
extern char *g_6030f90[];

/* ROM data */
extern char d_63b50[];
extern char d_68450[];
extern char d_68250[];
extern char d_68190[];
extern char d_67910[];
extern char d_678d0[];
extern char d_a7a0c[];
extern char d_a79f4[];

void f_780(void)
{
	long *hdr28;
	long *p;
	long cnt, flag, skip;
	short idx;
	unsigned long t, k;
	short y;
	char **s;

	skip = 0;
	hdr28 = (long *)rom_hdr_ptr->l28;
	f_2b430();
	g_6060008 = 0;
	g_606434c = 0;
	f_2ef3e();
	f_2e6fc(29);
	f_23f10();
	f_2d2bc();
	f_1108();
	f_25658();
	f_1715c();
	f_175c4();
	f_2a08c();
	f_2491c();
	f_1260();
	f_120c0();
	g_6065644 = 0;
	g_606564c = 0;
	f_2488c();
	g_6060022 = 0;
	f_2b3e8(g_607926c[70]);
	f_2af5e(1, 1, (long *)g_6030c7c);
	f_2c3d0(0);
	f_2af5e(1, 15, (long *)d_63b50);
	f_2af5e(0x9e, 1, (long *)d_68450);
	f_2af5e(0x95, 7, (long *)d_68250);
	f_2af5e(0x9c, 1, (long *)d_68190);
	f_29c50(0);
	f_29bf8(0);
	if (!(IN_SYS & 0x40) && !(IN0 & 8)) {
		f_b352();
		f_211a2();
		f_11fe8();
		f_b36e();
		f_21206();
		f_13378();
		f_2140e((unsigned short)g_60775a8[5]);
	}
	f_3003c();
	g_60775a8[4] = f_b2f4();
	g_60775a8[4] &= f_212f8();
	g_60775a8[4] &= f_13380();
	for (k = 0; k < 10; k++) {
		g_6065750[k] = g_6065698[k].l0 & 0xfffff;
		g_6065778[k] = 2520;
	}
	if (g_6060034[0] == 1)
		VREG_EE |= 0xc0;
	if (!f_213da((unsigned short)g_60775a8[5]))
		(void)IN_SYS;

	/* the RAM / EEPROM check screen */
	p = g_60775a8;
	if (p[0] & p[1] & p[2] & p[3] & p[4]) {
		cnt = 60;
	} else {
		cnt = 600;
		skip = 1;
	}
	g_6079268 = 0;
	if ((IN_DIP & 15) == 1)
		g_6079268 = 1;
	if (g_6060004 == 1 && !(IN_DIP & 15))
		g_6060004 = 0;
	f_2af5e(0, 1, (long *)g_6030cbc);
	f_2af5e(10, 1, (long *)g_6030c7c);
	f_2af5e(81, 1, (long *)d_67910);
	flag = 0;
	while (flag ? f_30a44() : cnt) {
		if ((g_6064754 & 1) && !(IN_SYS & 0x40))
			flag = 1;
		f_2b4e8();
		f_2c28c();
		f_e3b8();
		if (cnt)
			cnt--;
		if ((g_6064754 & 15) && skip)
			cnt = 0;
		f_e412(65, 20, g_6030cfc, 0);
		f_e520(146, 20, p[0] ? g_6030d08 : g_6030d0c, 0);
		f_e412(65, 40, g_6030d10, 0);
		f_e520(146, 40, (p[1] & p[2] & p[3]) ? g_6030d08 : g_6030d0c, 0);
		f_e412(65, 60, g_6030d1c, 0);
		f_e520(146, 60, p[4] ? g_6030d08 : g_6030d0c, 0);
	}
	f_29c50(0);
	f_29bf8(0);
	idx = f_2528c();
	f_253bc(idx, 60, 266, *hdr28);

	if (IN_SYS & 0x20) {
		switch (g_6060004) {
		case 1:
			if (IN_DIP & 15)
				break;
			g_6060004 = 0;
			/* fall through: the same screen as state 0 */
		case 0:
			f_29f94(0x7f00, 0x7f00, 0, 0xff);
			for (t = 0; t < 500; t++) {
				f_2b4e8();
				if ((((g_6064758 & 15) || (g_6064759 & 15)) && t > 60) || !(IN_SYS & 0x20))
					t = 600;
				f_e3e0(30, g_6030d24, 0);
				for (k = 0, y = 60, s = g_6030f34; k < 7; k++, y += 20, s++)
					f_e412(55, y, *s, 0);
			}
			break;
		case 2:
			f_29f94(0x7f00, 0x7f00, 0, 0xff);
			for (t = 0; t < 500; t++) {
				f_2b4e8();
				if ((((g_6064758 & 15) || (g_6064759 & 15)) && t > 60) || !(IN_SYS & 0x20))
					t = 600;
				f_e3e0(30, g_6030d24, 0);
				for (k = 0, y = 70, s = g_6030f68; k < 5; k++, y += 20, s++)
					f_e412(55, y, *s, 0);
			}
			break;
		case 3:
			f_29f94(0x7f00, 0x7f00, 0, 0xff);
			for (t = 0; t < 500; t++) {
				f_2b4e8();
				if ((((g_6064758 & 15) || (g_6064759 & 15)) && t > 60) || !(IN_SYS & 0x20))
					t = 600;
				switch (IN_DIP & 15) {
				case 1:
					g_606006c[idx].w2c &= 0x7fff;
					for (k = 0, y = 46, s = g_6030f50; k < 6; k++, y += 24, s++)
						f_2bf58(*s, y, 40);
					break;
				case 2:
					g_606006c[idx].w2c &= 0x7fff;
					for (k = 0, y = 46, s = g_6030f68; k < 5; k++, y += 24, s++)
						f_2bf58(*s, y, 40);
					break;
				case 4:
					g_606006c[idx].w2c &= 0x7fff;
					for (k = 0, y = 46, s = g_6030f7c; k < 5; k++, y += 24, s++)
						f_2bf58(*s, y, 40);
					break;
				case 8:
					g_606006c[idx].w2c &= 0x7fff;
					for (k = 0, y = 46, s = g_6030f90; k < 5; k++, y += 24, s++)
						f_2bf58(*s, y, 40);
					break;
				}
			}
			break;
		}
		g_606006c[idx].w2c |= 0x8000;
		f_29bf8(0);
		f_29f94(0, 0, 0, 0xff);
		f_2af5e(80, 1, (long *)d_678d0);
		for (t = 0; t < 500; t++) {
			f_2b4e8();
			f_185f0(d_a7a0c, 104, 32, 80, 125);
			f_185f0(d_a79f4, 0, 0, 80, 125);
			if ((g_6064758 & 15) || (g_6064759 & 15) || !(IN_SYS & 0x20))
				t = 600;
		}
		f_252f8(idx);
		f_2b4e8();
	}
	g_6079538 = 1;
	f_9b90();
}
