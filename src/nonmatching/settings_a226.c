/* rom: 0xa226 len: 0x10ce func: f_a226 flags: -macsave=1 -optimize=1 -speed */
/* difftest-nonreturning: f_a226 f_a608 f_ad80 f_af8c (why: the menus and test screens loop until g_6064750 is clear and a button byte (g_6079304/5, g_6064754/5) selects the exit; the tester never changes memory between stubbed calls, so on most random inputs the original never returns) */
/* difftest-inputs f_a226: 1/m6064750.4=0,m6079304.1=80,m6079305.1=8 2/m6064750.4=0,m6079304.1=8,m6079305.1=80 3/m6064750.4=0,m6079304.1=80,m6079305.1=4 4/m6064750.4=0,m6079304.1=2,m6079305.1=80 5/m6064750.4=0,m6079304.1=80,m6079305.1=e 6/m6064750.4=0,m6079304.1=8e,m6079305.1=80 7/m6064750.4=0,m6079304.1=80,m6079305.1=88 8/m6064750.4=0,m6079304.1=80,m6079305.1=2 */
/* difftest-inputs f_a608: 1/m6064750.4=0,m6064754.1=1,m6064755.1=0,m6079304.1=0,m6079305.1=0 2/m6064750.4=0,m6064754.1=81,m6064755.1=0,m6079304.1=2,m6079305.1=0 3/m6064750.4=0,m6064754.1=41,m6064755.1=10,m6079304.1=0,m6079305.1=2 4/m6064750.4=0,m6064754.1=11,m6064755.1=20,m6079304.1=2,m6079305.1=2 5/m6064750.4=0,m6064754.1=21,m6064755.1=80,m6079304.1=0,m6079305.1=0 6/m6064750.4=0,m6064754.1=ff,m6064755.1=ff,m6079304.1=ff,m6079305.1=ff 7/m6064750.4=0,m6064754.1=3,m6064755.1=40,m6079304.1=0,m6079305.1=0 8/m6064750.4=0,m6064754.1=9,m6064755.1=4,m6079304.1=2,m6079305.1=0 9/m6064750.4=0,m6064754.1=5,m6064755.1=8,m6079304.1=0,m6079305.1=2 10/m6064750.4=0,m6064754.1=d1,m6064755.1=22,m6079304.1=2,m6079305.1=3 */
/* difftest-inputs f_ad80: 1/m6064750.4=0,m6079304.1=1,m6079305.1=0 2/m6064750.4=0,m6079304.1=0,m6079305.1=2 3/m6064750.4=0,m6079304.1=10,m6079305.1=4 4/m6064750.4=0,m6079304.1=8,m6079305.1=10 5/m6064750.4=0,m6079304.1=20,m6079305.1=1 6/m6064750.4=0,m6079304.1=2,m6079305.1=20 7/m6064750.4=0,m6079304.1=f,m6079305.1=f 8/m6064750.4=0,m6079304.1=1,m6079305.1=10 9/m6064750.4=0,m6079304.1=30,m6079305.1=8 */
/* difftest-inputs f_af8c: 1/m6064750.4=0,m6064754.1=28,m6064755.1=0,m23000003.1=ff 2/m6064750.4=0,m6064754.1=ff,m6064755.1=ff,m23000003.1=0 3/m6064750.4=0,m6064754.1=29,m6064755.1=55,m23000003.1=aa 4/m6064750.4=0,m6064754.1=2a,m6064755.1=aa,m23000003.1=55 5/m6064750.4=0,m6064754.1=6c,m6064755.1=1,m23000003.1=fe 6/m6064750.4=0,m6064754.1=a8,m6064755.1=80,m23000003.1=7f 7/m6064750.4=0,m6064754.1=38,m6064755.1=f0,m23000003.1=f 8/m6064750.4=0,m6064754.1=2c,m6064755.1=f,m23000003.1=f0 */
/* Non-matching: functionally equivalent C for [0xa226, 0xb2f4) (f_a226, f_a608, f_acdc, f_ad80, f_af8c),
 * linked only by the shiftable build (splits_nonmatching.txt). Not matching coverage.
 * From src/settings_reset_9c0c.c. */
/* the original strings */
extern char g_6032ad4[];
extern char g_6032ae4[];
extern char g_6032b00[];
extern char g_6032b04[];
extern char g_6032b08[];
extern char g_6032b10[];
extern char g_6032b14[];
extern char g_6032b1c[];
extern char g_6032b24[];
extern char g_6032b2c[];
extern char g_6032b34[];
extern char g_6032b3c[];
extern char g_6032b44[];
extern char g_6032b4c[];
extern char g_6032b54[];
extern char g_6032b5c[];
extern char g_6032b64[];
extern char g_6032b68[];
extern char d_62d10[];
extern char d_62e50[];
extern char d_62f90[];
extern char d_63090[];
extern char d_678d0[];
extern char d_a79f4[];
extern char d_a7a0c[];
extern char d_a7a18[];
extern char d_a7a30[];
extern char d_2004001c[];
extern char d_20040030[];
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
#define HDR_COUNT (*(short *)((long)d_2004001c))
#define HDR_ANIM (*(struct anim **)((long)d_20040030))
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

void f_a226(void)
{
	short i = 0;
	int run = 1;
	unsigned short n = 6;
	short y;
	unsigned char a, b;

	g_6079307 = 0;
	g_6079306 = 0;
	g_6079308 = 0x200;
	f_9c0c();
	do {
		y = i * 12 + 55;
		g_606006c[g_60792ec].w4 = y;
		f_2b4e8();
		f_9ede();
		f_e520(90, y, g_6032960, 0);
		f_e3e0(15, g_6032964, 0);
		if (g_6079308 == 0x200) {
			f_e412(120, 55, g_6032974, i == 0 ? 0 : 1);
			f_e412(120, 67, g_6032980, i == 1 ? 0 : 1);
			f_e412(120, 79, g_603298c, i == 2 ? 0 : 1);
			f_e412(120, 91, g_6032998, i == 3 ? 0 : 1);
			f_e412(120, 103, g_60329a8, i == 4 ? 0 : 1);
			f_e412(120, 115, g_60329bc, i == 5 ? 0 : 1);
		} else {
			f_e412(120, 55, g_60329cc, i == 0 ? 0 : 1);
			f_e412(120, 67, g_60329d8, i == 1 ? 0 : 1);
			f_e412(120, 79, g_6032974, i == 2 ? 0 : 1);
			f_e412(120, 91, g_6032980, i == 3 ? 0 : 1);
			f_e412(120, 103, g_603298c, i == 4 ? 0 : 1);
			f_e412(120, 115, g_60329e0, i == 5 ? 0 : 1);
			f_e412(120, 127, g_6032998, i == 6 ? 0 : 1);
			f_e412(120, 139, g_60329a8, i == 7 ? 0 : 1);
			f_e412(120, 151, g_60329bc, i == 8 ? 0 : 1);
		}
		a = g_6079304;
		b = g_6079305;
		if ((a == 0x80) | (b == 0x80)) {
			if (--i < 0)
				i = n - 1;
		}
		if ((b == 0x40) | (a == 0x40)) {
			if (++i > n - 1)
				i = 0;
		}
		if (((a & 14) | (b & 14)) && !g_6064750) {
			f_29cc6(2, 2, 0, 6);
			if (i == n - 1)
				run = 0;
			else {
				if (g_6079308 == 0x200)
					g_603294c[i]();
				else
					g_603292c[i]();
				if (g_6079308 == 0x100 && n == 6) {
					n = 9;
					i = 9;
				}
			}
			if (run) {
				f_29cc6(4, 10, 0, 6);
				f_29cc6(2, 4, 0, 6);
				f_2b4e8();
				g_606006c[g_60792ec].w2c |= 0x8000;
				while (g_6064750)
					f_2b4e8();
				f_9c0c();
			}
		}
	} while (run);
	f_2c28c();
	f_2c326();
	g_606006c[g_60792ec].w2c |= 0x8000;
	f_2b4e8();
	f_29cc6(4, 10, 0, 6);
	f_29f94(0, 0, 0, 0xe0);
	g_6066198.b0 &= 0x7f;
	f_2774a(g_6066198.we, 1);
	f_2b4e8();
	f_2b4e8();
	((void (*)(void))0x686)();
}

void f_a608(void)
{
	short spr2;
	short sprite_id;
	short sel;
	struct anim *tbl;
	short total;
	short spr1;
	short x, y;
	unsigned char c2, c0, c1;
	char wait;
	long lastpos;
	short show;
	char rep1;
	int run;
	short last;
	unsigned char prev0, prev1, prev2, prev3;
	short cnt;
	char rep2, rep3, rep4;
	unsigned char bri0, bri1;
	char vis;
	short n, hold;
	long pos;
	unsigned long k, step;
	unsigned char a, b;
	int kk;

	pos = 0;
	tbl = HDR_ANIM;
	n = HDR_COUNT;
	run = 1;
	total = 0;
	sel = 0;
	y = 0;
	x = 0;
	vis = 0;
	wait = 0;
	show = 1;
	bri0 = 63;
	bri1 = 63;
	f_2c3d0(0);
	while (g_6064750)
		f_2b4e8();
	lastpos = 0;
	cnt = n;
	for (kk = 0; kk < cnt - 1; kk++)
		lastpos += tbl[lastpos].w2 >> 10;
	f_29cc6(0, 4, 0, -0x7ffa);
	f_2b3e8((short *)g_607926c[70]);
	if (n == 0) {
		rep1 = 120;
		while (--rep1) {
			f_e3e0(100, g_60329ec, 0);
			f_2b4e8();
		}
		run = 0;
	} else {
		g_606006c[g_60792ec].w2c |= 0x8000;
		spr1 = f_2528c();
		spr2 = f_2528c();
		f_253bc(spr1, 0, 0, tbl);
		f_253bc(spr2, 112, 160, (struct anim *)g_60792f0->l10);
		sprite_id = spr2;
		{
			short *sprite_arg = &sprite_id;
			g_606006c[*sprite_arg].b2a = 0;
		}
		sprite_id = spr2;
		{
			short *sprite_arg = &sprite_id;
			g_606006c[*sprite_arg].w18 &= 0xfffc;
		}
		g_606006c[spr2].b1c = 64;
		rep4 = 16;
		rep2 = 16;
		rep3 = 16;
		rep1 = 16;
		sprite_id = spr2;
		{
			short *sprite_arg = &sprite_id;
			g_606006c[*sprite_arg].w2c |= 0x8000;
		}
		g_606006c[spr1].w2c &= 0x7fff;
		g_6064870 = 34;
		g_6064872 = 48;
		g_6064874 = -48;
		f_acdc(&sel);
	}
	hold = last = n - 1;
	if (run) {
		do {
			f_2b4e8();
			f_9ede();
			if (wait < 18)
				wait++;
			if (wait > 16) {
				a = g_6064754;
				c0 = a;
				c2 = c1 = g_6064755;
				if (prev0 == (c0 & 15)) {
					if (--rep1 < 0)
						rep1 = 1;
					else
						c0 &= 0xf0;
					hold++; hold &= 0xfff;
				} else {
					hold = 0;
					prev0 = c0 & 15;
					rep1 = 16;
				}
				if (prev1 == (a & 15)) {
					if (--rep3 < 0)
						rep3 = 1;
					hold++; hold &= 0xfff;
				} else {
					hold = 0;
					prev1 = a & 15;
					rep3 = 16;
				}
				if (prev2 == (c2 & 15)) {
					if (--rep2 < 0)
						rep2 = 1;
					else
						c2 &= 0xf0;
				} else {
					prev2 = c2 & 15;
					rep2 = 16;
				}
				if (prev3 == (c1 & 15)) {
					if (--rep4 < 0)
						rep4 = 1;
					else
						c1 &= 0xf0;
				} else {
					prev3 = c1 & 15;
					rep4 = 16;
				}
			} else {
				c1 = 0;
				c2 = 0;
				c0 = 0;
			}
			if (show) {
				f_e3e0(10, g_60329fc, 0);
				f_e412(10, 20, g_6032a0c, 0);
				f_e412(10, 34, g_6032a18, 0);
				f_e63a(total, 20, 90, 1, 0, 1, 0);
				f_e63a(sel, 34, 90, 1, 0, 1, 0);
				f_e412(250, 20, g_6032a24, 0);
				f_e412(250, 34, g_6032a28, 0);
				f_e63a(x, 20, 275, 3, 0, 1, 0);
				f_e63a(y, 34, 275, 3, 0, 1, 0);
				f_e3e0(214, g_6032a2c, 0);
			}
			if (c0 & 8) {
				if (hold / 240 == 0)
					step = 1;
				else if (hold / 360 == 0)
					step = 2;
				else
					step = hold / 480 == 0 ? 5 : 5;
				total += step;
				for (k = 0; k < step; k++)
					pos += tbl[pos].w2 >> 10;
				if (total >= cnt) {
					total = 0;
					pos = 0;
					hold = 0;
				}
			}
			if (c0 & 4) {
				if (hold / 240 == 0)
					step = 1;
				else if (hold / 360 == 0)
					step = 2;
				else
					step = hold / 480 == 0 ? 5 : 5;
				pos--;
				total -= step;
				for (k = 0; k < step; k++)
					pos -= tbl[pos].w2 >> 10;
				pos++;
				if (total < 0) {
					hold = 0;
					total = last;
					pos = lastpos;
				}
			}
			if (c0 & 12)
				bri0 = bri1 = 63;
			b = g_6064754 | g_6064755;
			if (b & 0x80)
				y++;
			else if (b & 0x40)
				y--;
			if (b & 0x10)
				x--;
			else if (b & 0x20)
				x++;
			if (c2 & 8) {
				sel++;
				f_acdc(&sel);
			}
			if (c1 & 4) {
				sel--;
				f_acdc(&sel);
			}
			if (g_6079304 & 2) {
				if (vis) {
					long sprite_offset = spr2;
					sprite_offset *= 64;
					((struct spr *)((char *)g_606006c + sprite_offset))->w2c |= 0x8000;
					vis = 0;
				} else {
					long sprite_offset = spr2;
					sprite_offset *= 64;
					((struct spr *)((char *)g_606006c + sprite_offset))->w2c &= 0x7fff;
					vis = 1;
				}
			}
			if (g_6079305 & 2)
				show ^= 1;
			{
				long sprite_offset = spr1;
				struct spr *sprite;
				sprite_offset *= 64;
				sprite = (struct spr *)((char *)g_606006c + sprite_offset);
				sprite->b14 = bri0;
				sprite->b15 = bri1;
			}
			f_253bc(spr1, x, y, &tbl[pos]);
			if ((g_6064754 & 1) == 1)
				run = 0;
		} while (run);
	}
	g_6061944 = 125;
	f_2b4e8();
}

void f_acdc(short *sel)
{
	short *p;
	short a;
	long v;

	if (g_60792f0->n == 0) {
		*sel = 0;
		return;
	}
	if (*sel >= g_60792f0->n)
		*sel = 0;
	if (*sel < 0)
		*sel = g_60792f0->n - 1;
	p = g_60792f0->seq[*sel];
	while (*p == 0xa1) {
		p++;
		a = *p++;
		v = *p++ << 16;
		v |= (unsigned short)*p++;
		f_2b0ea(a, v);
		while (*p < 0)
			p++;
	}
}


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

void f_ad80(void)
{
    int run = 1;
    int page = 0;
    if (g_6064750) {
        do { f_2b4e8(); } while (g_6064750);
    }
    f_29f94(0, 0, 0, 224);
    f_29cc6(0, 4, 0, -0x7ffa);
    f_29bf8();
    g_606006c[g_60792ec].w2c |= 0x8000;
    f_2af5e(80, 1, (long *)((long)d_678d0));
    f_2af5e(32, 4, (long *)((long)d_63090));
    f_2af5e(48, 4, (long *)((long)d_62f90));
    f_2af5e(64, 4, (long *)((long)d_62d10));
    f_2af5e(16, 4, (long *)((long)d_62e50));
    do {
        f_2b4e8();
        f_9ede();
        f_e3e0(10, g_6032a3c, 0);
        f_e3e0(214, g_6032a4c, 0);
        f_e3e0(200, g_6032a5c, 0);
        if ((g_6079304 == 16) | (g_6079305 == 16)) {
            --page;
            if (page < 0) page = 2;
        }
        if ((g_6079304 == 32) | (g_6079305 == 32)) {
            ++page;
            if (page > 2) page = 0;
        }
        switch (page) {
        case 0:
            f_185f0((struct frame *)((long)d_a7a0c), 104, 32, 80, 125);
            f_185f0((struct frame *)((long)d_a79f4), 0, 0, 80, 125);
            break;
        case 1:
            f_185f0((struct frame *)((long)d_a7a30), 40, 32, 0, 125);
            break;
        case 2:
            f_185f0((struct frame *)((long)d_a7a18), 0, 0, 80, 125);
            break;
        }
        if ((g_6079304 & 15) | (g_6079305 & 15)) run = 0;
    } while (run);
    g_606006c[g_60792ec].w2c &= 0x7fff;
}

/* the input test screen's coordinate and bit tables, in the RAM data image */
struct shorts20 { short v[20]; };
struct bytes20 { unsigned char v[20]; };
extern const struct shorts20 g_6032a6e, g_6032a96;
extern const struct bytes20 g_6032abe;

void f_af8c(void)
{
	int run = 1;
	struct shorts20 xs, ys;
	struct bytes20 mask;
	short i, k;
	unsigned char v;

	/* local copies, as the original takes them */
	xs = g_6032a6e;
	ys = g_6032a96;
	mask = g_6032abe;

	while (g_6064750)
		f_2b4e8();
	f_29cc6(0, 4, 0, -0x7ffa);
	k = g_60792ec;
	g_606006c[k].w2c |= 0x8000;
	do {
		f_2b4e8();
		f_9ede();
		f_e3e0(10, g_6032ad4, 0);
		f_e3e0(200, g_6032ae4, 0);
		f_e520(110, 40, g_6032b00, 0);
		f_e520(150, 40, g_6032b04, 0);
		f_e412(30, 60, g_6032b08, 0);
		f_e412(30, 75, g_6032b10, 0);
		f_e412(30, 90, g_6032b14, 0);
		f_e412(30, 105, g_6032b1c, 0);
		f_e412(30, 120, g_6032b24, 0);
		f_e412(30, 135, g_6032b2c, 0);
		f_e412(30, 150, g_6032b34, 0);
		f_e412(30, 165, g_6032b3c, 0);
		f_e412(200, 120, g_6032b44, 0);
		f_e412(200, 135, g_6032b4c, 0);
		f_e412(200, 150, g_6032b54, 0);
		f_e412(200, 165, g_6032b5c, 0);
		for (i = 0; i < 20; i++) {
			if (i < 8)
				v = g_6064754;
			else if (i < 16)
				v = g_6064755;
			else
				v = ~IN3;
			if (mask.v[i] & v)
				f_e520(xs.v[i], ys.v[i], g_6032b64, 0);
			else
				f_e520(xs.v[i], ys.v[i], g_6032b68, 0);
		}
		if ((g_6064754 & 0x28) == 0x28)
			run = 0;
	} while (run);
}

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
