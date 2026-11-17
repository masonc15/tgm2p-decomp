/* rom: 0x15104 len: 0x690 func: f_15104 flags: -macsave=1 -optimize=1 -speed */
/* Non-matching: functionally equivalent C for [0x15104, 0x15794) (f_15104, f_152d4),
 * linked only by the shiftable build (splits_nonmatching.txt). Not matching coverage.
 * From research/2026-10-16-15104-types/jC.c. */
struct field { char pad[0x308]; unsigned long flags308; unsigned short mode; char pad2[0x3b4 - 0x30e]; };
extern char g_60b161c[], g_60b1620[], g_60b1630;
extern unsigned char g_6064758, g_6064759;
extern long g_6064750, g_606487c, g_6065648, g_6079538;
extern unsigned long g_6060008;
extern char g_607953c;
extern short g_6066184;
extern short g_60661a6;
extern struct field g_6064898[2];
extern int f_2da90(int, int);
extern void f_29f94(unsigned long, unsigned long, unsigned char, unsigned char);
extern void f_2b1cc(void);
extern void f_247b0(long);
extern void f_23f10(void);
extern void f_2b31e(void);
extern void f_2ab56(unsigned short);
extern void f_2491c(void);
extern void f_1715c(void);
extern void f_2eeaa(int);
extern void f_1278(void);
extern void f_2d06(struct field *);
extern int f_2b4e8(void);
extern void f_2b196(void);
extern void f_2488c(void);
extern void f_29f70(void);
extern void f_247ee(void);
extern void f_175c4(void);
extern void f_2774a(short, int);
extern void f_29c18(short);
extern void f_29c50(long);
extern void f_277ac(short, long);
extern void f_29cc6(short, short, long, long);
extern void f_2af5e(int, int, void *);
extern int f_12fa8(int );
extern char d_65790[], d_657d0[], d_681d0[], d_67d90[], d_68590[];
struct frame { char pad[12]; };
extern struct frame d_aa928[], d_aa7a8[], d_aacb8[];
extern short g_606000a;
extern void f_185f0(struct frame *, int, int, int, int);
extern char g_6033748[], g_6033788[], d_67910[];
extern unsigned char g_606475e, g_606475f;
extern short g_606488e, g_6064890;
extern unsigned long g_6065680;
extern void f_2a576(int, char *, char *, int, int, int, int);
extern void f_e3b8(void);
extern void f_e750(int, int, char *, int);
extern void f_103d0(void);
extern void f_23828(void);
extern void f_23830(void);
extern char f_2d54(struct field *);
extern void f_10942(struct field *);
extern void f_2db32(int, int);
extern void f_2ecea(void);
extern void f_2edc2(void);
extern void f_10788(void);
extern struct { char pad[7]; char b7; char b8; char pad2[5]; short we; } g_6064884;
extern short g_606003c;
extern long g_60657a0;
extern char g_6079299;
extern unsigned long g_6064880;
extern char g_60341bf[], g_60337e7[], g_6034b97[], g_6033790[], g_60337d0[];
extern char d_67110[], d_67390[], d_67610[], d_68410[], d_63ed0[], d_63e90[];
extern struct frame d_aa790[];
extern void f_21206(void);
extern void f_2ed06(void);
extern void f_2ed3e(void);
extern void f_2eeaa(int);
extern void f_2ece0(void);
extern void f_2edb8(void);
extern void f_2f2cc(void);
extern void f_2f3dc(void);
extern void f_2ee38(void);
extern void f_85d8(void);
extern void f_20250(void);
extern void f_1340(int);
extern void f_2f7a(struct field *);
extern int f_2ba08(char *, char *);
extern void f_96ac(void);
extern int f_2beca(int);
extern int f_e39c(char *, int);
extern void f_e490(int, int, char *, int);
extern void f_1278(void);
int f_14bce(void);
int f_14d50(void);
int f_14f1c(void);
int f_15104(void);
int f_152d4(void);
int f_15794(void);

short f_149b4(void);

void f_14a08(void);

int f_14b10(void);

int f_14bce(void);

int f_14d50(void);

int f_14f1c(void);

int f_15104(void)
{
	short timer;
	int done;
	short i;
	short j;

	timer = 300;
	done = 0;
	f_2b1cc();
	if (f_2b4e8())
		return 11;
	f_2b31e();
	f_2ab56(500);
	f_29f94(0, 0, 0, 255);
	if (g_6064750) {
		do {
			if (f_2b4e8())
				return 11;
			if (f_149b4() == 1)
				return 12;
		} while (g_6064750);
	}
	f_29f70();
	f_247ee();
	f_175c4();
	f_2774a(g_60661a6, 0);
	f_2af5e(160, 16, d_68590);
	i = 0;
	do {
		if (f_2b4e8())
			return 11;
		i++;
	} while (i < 3);
	*(char *)0x2405ffe8 = 19;
	*(char *)0x2405ffe9 = 102;
	f_29c18(6);
	f_29c50(0);
	*(char *)0x2405ffe4 = 31;
	f_29cc6(0, 20, 0, 6);
	j = 0;
	do {
		if (f_2b4e8()) {
			f_247ee();
			return 11;
		}
		if (f_149b4()) {
			f_247ee();
			return 12;
		}
		f_185f0(d_aacb8, 0, 0, 160, 40);
		if (timer)
			timer--;
		else
			done = 1;
	} while (!done);
	if (g_6064750) {
		do {
			if (f_2b4e8())
				return 11;
		} while (g_6064750);
	}
	f_2b4e8();
	f_247ee();
	f_29cc6(2, 10, 0, 6);
	do {
		if (f_2b4e8()) {
			f_247ee();
			return 11;
		}
		j++;
	} while (j < 10);
	return 12;
}

int f_152d4(void)
{
	short timer;
	short done;
	short i;
	short k;
	short anim;
	char tog;
	short cnt;
	int sum;
	char *q;
	struct field *p0;
	struct field *p1;
	unsigned char *b0;
	unsigned char *b1;

	timer = 600;
	done = 0;
	g_606487c = 0;
	f_2b1cc();
	if (f_2b4e8())
		return 11;
	f_29cc6(4, 0, 0, 0);
	if (g_6064750) {
		do {
			if (f_2b4e8())
				return 11;
		} while (g_6064750);
	}
	f_29f70();
	f_247ee();
	f_175c4();
	g_6066184 = 0;
	f_1278();
	f_2af5e(160, 16, d_67d90);
	f_e3b8();
	q = g_6033748;
	f_2af5e(202, 1, q);
	f_2a576(159, q, d_67910, 1, 64, 1, 63);
	tog = 0;
	cnt = 0;
	f_29f94(0, 0, 0, 255);
	if (g_6064750) {
		do {
			if (f_2b4e8())
				return 11;
		} while (g_6064750);
	}
	f_2774a(g_60661a6, 0);
	i = 0;
	do {
		if (f_2b4e8())
			return 11;
		i++;
	} while (i < 3);
	f_2ecea();
	f_2edc2();
	f_2b196();
	f_29c18(6);
	f_29c50(0);
	f_29cc6(0, 20, 0, 6);
	anim = 0;
	f_23828();
	p0 = &g_6064898[0];
	p1 = &g_6064898[1];
	b0 = &g_606475e;
	b1 = &g_606475f;
	for (;;) {
		if (f_2b4e8()) {
			f_247ee();
			return 11;
		}
		if (!(g_6060008 & 1))
			anim++;
		anim &= 15;
		/* the title background's frames are 24 bytes apart (two records) */
		f_185f0(&d_aa7a8[(unsigned short)anim * 2], 0, 0, 160, 16);
		f_103d0();
		f_23830();
		if (f_2da90(0, 0) && f_2d54(p0))
			timer += 300;
		if (f_2da90(1, 0) && f_2d54(p1))
			timer += 300;
		f_10942(p0);
		f_10942(p1);
		sum = timer;
		if ((*b0 & 1) && !(g_6066184 & 1) && !(g_6066184 & 2)) {
			if (f_2da90(0, 0)) {
				f_2db32(0, 0);
				g_6066184 |= 1;
				g_606488e = p0->mode;
				g_6064890 = p1->mode;
				if (sum > 5)
					timer = 5;
			}
		} else if ((*b1 & 1) && !(g_6066184 & 2) && !(g_6066184 & 1) && f_2da90(1, 0)) {
			f_2db32(1, 0);
			g_6066184 |= 2;
			g_606488e = p0->mode;
			g_6064890 = p1->mode;
			if (sum > 5)
				timer = 5;
		}
		if (timer)
			timer--;
		else
			done = 1;
		f_e750(256, 160, g_6033788, 0);
		cnt++;
		if ((unsigned short)cnt % 64 == 0) {
			tog = !tog;
			if (tog) f_2a576(159, q, d_67910, 1, 68, 1, 63);
			else f_2a576(159, q, d_67910, 1, 64, 1, 63);
		}
		if (done) {
			if (g_6064750) {
				do {
					if (f_2b4e8())
						return 11;
				} while (g_6064750);
			}
			f_2b4e8();
			f_247ee();
			if (g_6066184) {
				f_29cc6(2, 10, 0, 6);
				k = 0;
				do {
					if (f_2b4e8()) {
						f_247ee();
						return 11;
					}
					k++;
				} while (k < 10);
				if (g_6065680 < g_6065648)
					g_6065680 = g_6065648;
				return 13;
			}
			g_607953c++;
			if ((unsigned char)g_607953c >= 7)
				g_607953c = 1;
			switch ((unsigned char)g_607953c) {
			case 1:
			case 4:
				return 4;
			case 2:
			case 5:
				return 5;
			case 3:
			case 6:
			case 7:
				return 6;
			default:
				return 4;
			}
		} else {
			f_10788();
		}
	}
}
int f_15794(void);
