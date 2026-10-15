/* rom: 0xb2f4 len: 0x7a func: f_b2f4 context-rom: 0x9c0c literal-tail-calls: 1 function-code-range: 0xb36e flags: -macsave=1 -optimize=1 -speed */
/* Only f_b2f4 and f_b352 are retained. The other genuine bodies preserve the original optimized compiler state and remain unlinked context. */
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

void f_9c0c(void)
{
	f_2b1cc();
	g_60792f0 = (struct font *)rom_hdr_ptr->l30;
	if (eeprom_shadow[0] == 1)
		REG(0x2405ffee) = REG(0x2405ffee) | 0xc0;
	else
		REG(0x2405ffee) = REG(0x2405ffee) & 0x3f;
	REG(0x2405ffe8) = 53;
	REG(0x2405ffe9) = 102;
	f_2521e();
	f_23f10();
	f_2b31e();
	f_2491c();
	f_175c4();
	g_60b1819 = 0;
	REG(0x2405ffe0) = 0;
	REG(0x2405ffe4) = 36;
	REG(0x2405ffe1) = 0;
	REG(0x2405ffe2) = 0;
	f_2488c();
	g_6060022 = 2;
	f_2b3e8((short *)g_607926c[70]);
	f_2c3d0(0);
	f_2c03c(19);
	g_60792e0.w0 = 0;
	g_60792e0.w2 = 0x400;
	g_60792e0.b6 = 0;
	g_60792e0.b4 = 0;
	g_60792e0.b7 = 0;
	g_60792e0.b5 = 0;
	g_60792e0.b8 = 0;
	g_60792e0.b9 = 0;
	g_60792e0.w10 = 10;
	g_60792ec = f_2528c();
	g_606006c[g_60792ec].desc = &g_60792e0;
	g_606006c[g_60792ec].w8 = 90;
	g_606006c[g_60792ec].w4 = 55;
	g_606006c[g_60792ec].w18 = 0;
	g_606006c[g_60792ec].w22 = 0;
	g_606006c[g_60792ec].b14 = 63;
	g_606006c[g_60792ec].b15 = 63;
	g_606006c[g_60792ec].b2a = g_6061945;
	g_606006c[g_60792ec].w1a = 0;
	g_606006c[g_60792ec].b1c = 0;
	g_606006c[g_60792ec].b2b = 2;
	g_606006c[g_60792ec].w2c = 0xa030;
	g_606006c[g_60792ec].w2c |= 0x8000;
	f_29cc6(4, 10, 0, 6);
	f_29cc6(0, 4, 0, -0x7ffa);
	f_bd8a();
	f_2b4e8();
	f_2b4e8();
	f_2f2cc();
	f_2f3dc();
	f_2ecea();
	f_2edc2();
	f_2eeaa(7);
	g_6066198.b0 |= 0x80;
	f_2774a(g_6066198.we, 0);
	f_e3b8();
}

#pragma inline(f_9e6a, f_9ea4)
void f_9e6a(unsigned char m)
{
	if (g_6079306 & m) {
		if (g_6079304 & m)
			g_6079304 &= ~m;
		else
			g_6079306 &= ~m;
	} else if (g_6079304 & m)
		g_6079306 |= m;
}

void f_9ea4(unsigned char m)
{
	if (g_6079307 & m) {
		if (g_6079305 & m)
			g_6079305 &= ~m;
		else
			g_6079307 &= ~m;
	} else if (g_6079305 & m)
		g_6079307 |= m;
}

void f_9ede(void)
{
	g_6079304 = g_6064754;
	g_6079305 = g_6064755;
	f_9e6a(0x80);
	f_9e6a(0x40);
	f_9e6a(0x20);
	f_9e6a(0x10);
	f_9ea4(0x80);
	f_9ea4(0x40);
	f_9ea4(0x20);
	f_9ea4(0x10);
	f_9e6a(0x08);
	f_9e6a(0x04);
	f_9e6a(0x02);
	f_9e6a(0x01);
	f_9ea4(0x08);
	f_9ea4(0x04);
	f_9ea4(0x02);
	f_9ea4(0x01);
}

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
    f_2af5e(80, 1, (long *)0x678d0);
    f_2af5e(32, 4, (long *)0x63090);
    f_2af5e(48, 4, (long *)0x62f90);
    f_2af5e(64, 4, (long *)0x62d10);
    f_2af5e(16, 4, (long *)0x62e50);
    do {
        f_2b4e8();
        f_9ede();
        f_e3e0(10, "- Screen Test -", 0);
        f_e3e0(214, "Next = Right", 0);
        f_e3e0(200, "Exit = Any Button", 0);
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
            f_185f0((struct frame *)0xa7a0c, 104, 32, 80, 125);
            f_185f0((struct frame *)0xa79f4, 0, 0, 80, 125);
            break;
        case 1:
            f_185f0((struct frame *)0xa7a30, 40, 32, 0, 125);
            break;
        case 2:
            f_185f0((struct frame *)0xa7a18, 0, 0, 80, 125);
            break;
        }
        if ((g_6079304 & 15) | (g_6079305 & 15)) run = 0;
    } while (run);
    g_606006c[g_60792ec].w2c &= 0x7fff;
}

void f_af8c(void)
{
	int run = 1;
	short xs[20] = {110,110,110,110,110,110,110,110,150,150,150,150,150,150,150,150,270,270,270,270};
	short ys[20] = {60,75,90,105,120,135,150,165,60,75,90,105,120,135,150,165,120,135,150,165};
	unsigned char mask[20] = {1,128,64,16,32,8,4,2,1,128,64,16,32,8,4,2,32,16,1,2};
	short i, k;
	unsigned char v;

	while (g_6064750)
		f_2b4e8();
	f_29cc6(0, 4, 0, -0x7ffa);
	k = g_60792ec;
	g_606006c[k].w2c |= 0x8000;
	do {
		f_2b4e8();
		f_9ede();
		f_e3e0(10, "- Input Test -", 0);
		f_e3e0(200, "Exit = 1P-Right + Button1", 0);
		f_e520(110, 40, "1", 0);
		f_e520(150, 40, "2", 0);
		f_e412(30, 60, "Start", 0);
		f_e412(30, 75, "Up", 0);
		f_e412(30, 90, "Down", 0);
		f_e412(30, 105, "Left", 0);
		f_e412(30, 120, "Right", 0);
		f_e412(30, 135, "Button1", 0);
		f_e412(30, 150, "Button2", 0);
		f_e412(30, 165, "Button3", 0);
		f_e412(200, 120, "Test", 0);
		f_e412(200, 135, "Service", 0);
		f_e412(200, 150, "Coin-1", 0);
		f_e412(200, 165, "Coin-2", 0);
		for (i = 0; i < 20; i++) {
			if (i < 8)
				v = g_6064754;
			else if (i < 16)
				v = g_6064755;
			else
				v = ~IN3;
			if (mask[i] & v)
				f_e520(xs[i], ys[i], "Y", 0);
			else
				f_e520(xs[i], ys[i], "N", 0);
		}
		if ((g_6064754 & 0x28) == 0x28)
			run = 0;
	} while (run);
}

extern const short g_6032b6a[11];
extern const unsigned char g_6032b80[10];


int f_b2f4(void)
{
	int ok = 1;
	int i, sum;

	for (i = 0, sum = 0; i < 10; i++)
		sum += g_6060034[i];
	if (sum == g_6060034[10]) {
		for (i = 0; i < 10; i++) {
			if (g_6060034[i] > g_6032b80[i]) {
				ok = 0;
				break;
			}
		}
	} else
		ok = 0;
	if (!ok) {
		f_b352();
		f_b36e();
	}
	return ok;
}



void f_b352(void)
{
	int i;

	for (i = 0; i <= 10; i++)
		g_6060034[i] = g_6032b6a[i];
}


void f_b36e(void)
{
	int i, sum;

	for (i = 0, sum = 0; i < 10; i++)
		sum += g_6060034[i];
	g_6060034[i] = sum;
	f_3036e();
}


/* Genuine settings and sound-test drafts; no artificial stack objects. */

void f_b892(short *,short);
unsigned char f_bc02(char *,short *,char);
extern void f_20206(void);
static const char *const names[30] = {
	"Normal", "Reverse", "1 Round", "3 Rounds", "5 Rounds",
	"Lv.100", "Lv.200", "Lv.300", "Lv.400", "Lv.500", "Lv.600", "Lv.700", "Lv.800", "Lv.900", "Lv.999",
	"Off", "On", "Same", "Individual", "Normal", "Free Play",
	"1Coin  = 1Credit", "2Coins = 1Credit", "3Coins = 1Credit", "4Coins = 1Credit",
	"5Coins = 1Credit", "6Coins = 1Credit",
	"1Coin  = 2Credits", "1Coin  = 3Credits", "1Coin  = 4Credits"
};
static const unsigned char off9[9] = {0, 2, 5, 15, 15, 17, 19, 21, 21};
static const short ypos[12] = {40, 53, 66, -100, 79, 92, 105, 118, 131, 144, 157, 170};

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
			if (f_bc02("Default?", buf, 1))
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
			else if (f_bc02("Save?", buf, 0) == 0) {
				f_2b4e8();
				f_b892(buf, 11);
				f_e3e0(200, "Saved", 0);
				k = g_60792ec;
				g_606006c[k].w2c |= 0x8000;
				f_2b4e8();
				for (i = 0, j = 0; i < 10; i++)
					j += (short)(k = buf[i]), cfg[i] = k;
				cfg[10] = j;
				f_2f36e();
				f_20206();
				f_b892(buf, 11);
				f_e3e0(200, "Saved", 0);
				k = g_60792ec;
				g_606006c[k].w2c |= 0x8000;
				for (n = 20; n > 0; n--) {
					f_2b4e8();
					f_9ede();
					f_b892(buf, 11);
					f_e3e0(200, "Saved", 0);
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

void f_b892(short *buf, short cur)
{
	short tmp[12];
	short j, idx;
	int f;

	f_e3e0(10, "- Game  Setting -", 0);
	f_e412(50, ypos[0], "Screen", cur == 0 ? 0 : 1);
	f_e412(50, ypos[1], "VS Round", cur == 1 ? 0 : 1);
	f_e412(50, ypos[2], "VS Level", cur == 2 ? 0 : 1);
	f_e412(50, ypos[4], "Demo Sound", cur == 4 ? 0 : 1);
	f_e412(50, ypos[5], "Coin Slot", cur == 5 ? 0 : 1);
	f_e412(50, ypos[6], "Coin Mode", cur == 6 ? 0 : 1);
	if (buf[6] == 0) {
		if (buf[5] == 0) {
			f_e412(70, ypos[7], "Coin-1", cur == 7 ? 0 : 1);
			f_e412(70, ypos[8], "Coin-2", cur == 8 ? 0 : 1);
		} else
			f_e412(70, ypos[7], "Coin", cur == 7 ? 0 : 1);
	}
	f_e412(50, ypos[9], "Factory Setting", cur == 9 ? 0 : 1);
	f_e412(50, ypos[10], "Exit", cur == 10 ? 0 : 1);
	if (cur <= 10) {
		g_606006c[tmp[cur]].b1c = 0;
		switch (cur) {
		case 9:
			f_e3e0(200, "Default = Button1", 0);
			break;
		case 10:
			f_e3e0(200, "Exit = Button1", 0);
			break;
		default:
			f_e3e0(200, "Select = Left or Right", 0);
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
		f_e412(200, 200, "Yes", sel == 0 ? 0 : 1);
		f_e412(240, 200, "No", sel == 1 ? 0 : 1);
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

void f_bd8a(void)
{
	long i;
	long a, b;
	unsigned long *q;

	a = 0x1c00;
	b = 0x1c00;
	q = (unsigned long *)0x2400437c;
	for (i = 0; i < 112; i++) {
		((volatile unsigned long *)0x24004000)[i] = a | 0x7f;
		a += 0x100;
		*q = b | 0x7f;
		q--;
		b += 0x100;
	}
}

extern unsigned short d_3a8e8[];
extern void f_2f2cc(void);
extern void f_2f3dc(void);
extern void f_2e5aa(short);
extern void f_2ed3e(void);
extern void f_2e6fc(short);
extern void f_2ed06(void);
void f_e63a(int, short, int, short, long, unsigned char, char);

unsigned short f_be04(short v)
{
	short i;

	for (i = 0; i < 6; i++)
		if (d_3a8e8[i] == (unsigned short)v)
			return 1;
	return 0;
}

void f_be28(void)
{
	short effect = 0;
	short sel = 0;
	short music = 0;
	int run = 1;
	short cnt1 = 0;
	short prev2 = 0;
	short cnt2 = 0;
	short prev1 = 0;
	short rep1, rep2;
	int y;
	int k;

	while (g_6064750)
		f_2b4e8();
	f_29cc6(0, 4, 0, -0x7ffa);
	f_2f2cc();
	f_2f3dc();
	g_606006c[g_60792ec].w4 = 50;
	g_606006c[g_60792ec].w8 = 60;
	do {
		f_2b4e8();
		f_9ede();
		if (prev1 != g_6064754) {
			prev1 = g_6064754;
			cnt1 = 0;
			rep1 = prev1;
		} else {
			cnt1++;
			cnt1 &= 0x7fff;
			if (cnt1 > 30)
				rep1 = prev1;
			else
				rep1 = 0;
		}
		if (prev2 != g_6064755) {
			prev2 = g_6064755;
			cnt2 = 0;
			rep2 = prev2;
		} else {
			cnt2++;
			cnt2 &= 0x7fff;
			if (cnt2 > 30)
				rep2 = prev2;
			else
				rep2 = 0;
		}
		if ((g_6079304 == 0x40) | (g_6079305 == 0x40)) {
			if (++sel > 2)
				sel = 0;
		}
		if ((g_6079304 == 0x80) | (g_6079305 == 0x80)) {
			if (--sel < 0)
				sel = 2;
		}
		switch (sel) {
		case 0:
			if ((rep1 == 0x20) | (rep2 == 0x20)) {
				if (++music > 7)
					music = 0;
			}
			if ((rep1 == 0x10) | (rep2 == 0x10)) {
				if (--music < 0)
					music = 7;
			}
			if ((g_6079304 == 8) | (g_6079305 == 8))
				f_2e5aa(music);
			if ((g_6079304 == 4) | (g_6079305 == 4))
				f_2ed3e();
			break;
		case 1:
			if ((rep1 == 0x20) | (rep2 == 0x20)) {
				if (++effect > 40)
					effect = 0;
			}
			if ((rep1 == 0x10) | (rep2 == 0x10)) {
				if (--effect < 0)
					effect = 40;
			}
			if ((g_6079304 == 8) | (g_6079305 == 8)) {
				if (f_be04(effect) == 0)
					f_2e6fc(effect);
			}
			if ((g_6079304 == 4) | (g_6079305 == 4))
				f_2ed06();
			break;
		case 2:
			if ((g_6079304 == 8) | (g_6079305 == 8))
				run = 0;
			break;
		}
		f_e3e0(10, "- Sound Test -", 0);
		f_e412(80, 50, "Music", sel == 0 ? 0 : 1);
		f_e412(80, 64, "Effect", sel == 1 ? 0 : 1);
		f_e412(80, 78, "Exit", sel == 2 ? 0 : 1);
		g_606006c[g_60792ec].w4 = y = sel * 14 + 50;
		f_e520(60, y, g_6032960, 0);
		f_e63a(music, 50, 200, 2, 0, 1, sel == 0 ? 0 : 1);
		f_e63a(effect, 64, 200, 3, 0, 1, sel == 1 ? 0 : 1);
		if (sel == 2)
			f_e3e0(200, "Exit = Button1", 0);
		else {
			f_e3e0(208, "Play = Button1", 0);
			f_e3e0(194, "Stop = Button2", 0);
			f_e3e0(180, "Select = Left or Right", 0);
		}
	} while (run);
	f_2f2cc();
	f_2f3dc();
	f_2b4e8();
}

extern void f_2ee38(void);
extern void f_2ee74(void);
extern char f_2ee0c(char);
extern void f_2edcc(char, char);

void f_c21a(void)
{
	int run = 1;
	short sel = 0;
	short music = 0;
	short effect = 0;
	short prev1 = 0;
	short cnt1 = 0;
	short prev2 = 0;
	short cnt2 = 0;
	short voice = 0;
	short rep1, rep2;
	short i;

	while (g_6064750)
		f_2b4e8();
	f_29cc6(0, 4, 0, -0x7ffa);
	g_606006c[g_60792ec].w4 = 50;
	g_606006c[g_60792ec].w8 = 60;
	f_2f2cc();
	f_2f3dc();
	f_2ee38();
	do {
		f_2b4e8();
		f_9ede();
		if (prev1 != g_6064754) {
			prev1 = g_6064754;
			cnt1 = 0;
			rep1 = prev1;
		} else {
			cnt1++;
			cnt1 &= 0x7fff;
			if (cnt1 > 30)
				rep1 = prev1;
			else
				rep1 = 0;
		}
		if (prev2 != g_6064755) {
			prev2 = g_6064755;
			cnt2 = 0;
			rep2 = prev2;
		} else {
			cnt2++;
			cnt2 &= 0x7fff;
			if (cnt2 > 30)
				rep2 = prev2;
			else
				rep2 = 0;
		}
		if ((g_6079304 == 0x40) | (g_6079305 == 0x40)) {
			if (sel == 2) {
				if (voice < 8)
					voice += 8;
				else if (++sel > 5)
					sel = 0;
			} else if (++sel > 5)
				sel = 0;
		}
		if ((g_6079304 == 0x80) | (g_6079305 == 0x80)) {
			if (sel == 2) {
				if (voice > 7)
					voice -= 8;
				else if (--sel < 0)
					sel = 5;
			} else if (--sel < 0)
				sel = 5;
		}
		switch (sel) {
		case 0:
			if ((rep1 == 0x20) | (rep2 == 0x20)) {
				if (++music > 7)
					music = 0;
			}
			if ((rep1 == 0x10) | (rep2 == 0x10)) {
				if (--music < 0)
					music = 7;
			}
			if ((g_6079304 == 8) | (g_6079305 == 8))
				f_2e5aa(music);
			if ((g_6079304 == 4) | (g_6079305 == 4))
				f_2ed3e();
			g_606006c[g_60792ec].w4 = 50;
			f_e520(60, 50, g_6032960, 0);
			break;
		case 1:
			if ((rep1 == 0x20) | (rep2 == 0x20)) {
				if (++effect > 40)
					effect = 0;
			}
			if ((rep1 == 0x10) | (rep2 == 0x10)) {
				if (--effect < 0)
					effect = 40;
			}
			if ((g_6079304 == 8) | (g_6079305 == 8)) {
				if (f_be04(effect) == 0)
					f_2e6fc(effect);
			}
			if ((g_6079304 == 4) | (g_6079305 == 4))
				f_2ed06();
			g_606006c[g_60792ec].w4 = 64;
			f_e520(60, 64, g_6032960, 0);
			break;
		case 2:
			if ((g_6079304 == 0x20) | (g_6079305 == 0x20)) {
				if (++voice > 15)
					voice = 0;
			}
			if ((g_6079304 == 0x10) | (g_6079305 == 0x10)) {
				if (--voice < 0)
					voice = 15;
			}
			if ((g_6079304 == 8) | (g_6079305 == 8))
				f_2edcc(voice, (f_2ee0c(voice) + 1) & 1);
			g_606006c[g_60792ec].w4 = 78;
			f_e520(60, 78, g_6032960, 0);
			break;
		case 3:
			if ((g_6079304 == 8) | (g_6079305 == 8))
				f_2ee38();
			g_606006c[g_60792ec].w4 = 120;
			f_e520(60, 120, g_6032960, 0);
			break;
		case 4:
			if ((g_6079304 == 8) | (g_6079305 == 8))
				f_2ee74();
			g_606006c[g_60792ec].w4 = 134;
			f_e520(60, 134, g_6032960, 0);
			break;
		case 5:
			if ((g_6079304 == 8) | (g_6079305 == 8))
				run = 0;
			g_606006c[g_60792ec].w4 = 148;
			f_e520(60, 148, g_6032960, 0);
			break;
		}
		f_e3e0(10, "- Sound Test -", 0);
		f_e412(80, 50, "Music", sel == 0 ? 0 : 1);
		f_e412(80, 64, "Effect", sel == 1 ? 0 : 1);
		f_e412(80, 78, "Track", sel == 2 ? 0 : 1);
		f_e412(80, 120, "All On", sel == 3 ? 0 : 1);
		f_e412(80, 134, "All Off", sel == 4 ? 0 : 1);
		f_e412(80, 148, "Exit", sel == 5 ? 0 : 1);
		for (i = 0; i < 16; i++) {
			if (i < 8)
				f_e520(i * 17 + 114, 92, f_2ee0c(i) == 0 ? "Y" : "N", 0);
			else
				f_e520((i - 8) * 17 + 114, 106, f_2ee0c(i) == 0 ? "Y" : "N", 0);
		}
		if (sel == 2) {
			if (voice < 8)
				f_e520(voice * 17 + 114, 92, "W", 0);
			else
				f_e520((voice - 8) * 17 + 114, 106, "W", 0);
		}
		f_e63a(cnt1, 50, 200, 2, 0, 1, sel == 0 ? 0 : 1);
		f_e63a(cnt2, 64, 200, 3, 0, 1, sel == 1 ? 0 : 1);
		f_e63a(voice, 78, 200, 2, 0, 1, sel == 2 ? 0 : 1);
		switch (sel) {
		case 2:
			f_e3e0(204, "Change = Button1", 0);
			f_e3e0(190, "Select = Left or Right", 0);
			break;
		case 3:
			f_e3e0(200, "All On = Button1", 0);
			break;
		case 4:
			f_e3e0(200, "All Off = Button1", 0);
			break;
		case 5:
			f_e3e0(200, "Exit = Button1", 0);
			break;
		default:
			f_e3e0(208, "Play = Button1", 0);
			f_e3e0(194, "Stop = Button2", 0);
			f_e3e0(180, "Select = Left or Right", 0);
			break;
		}
	} while (run);
	f_2f2cc();
	f_2f3dc();
	f_2b4e8();
}


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

void f_cac8(void)
{
	short s0;
	short t4;
	char cnt;
	struct preview_sprite **tbl;
	short show;
	signed char h20, h44;
	unsigned char h36, h40;
	short w24, w28;
	long run;
	struct s2t s2;
	struct s2t *p;
	short idx;
	long id;
	long hold;

	tbl = *(struct preview_sprite ***)0x20040024;
	run = 1;
	s0 = 0;
	cnt = 0;
	w24 = 0;
	w28 = 0;
	show = 1;
	idx = 0;
	while (g_6064750)
		f_2b4e8();
	f_29cc6(0, 4, 0, -0x7ffa);
	f_acdc(&s0);
	{
		short v = g_60792ec;
		g_606006c[v].w2c |= 0x8000;
		f_25658(v);
	}
	for (;;) {
	
	p = &s2;
	p->spr = tbl[0];
	p->b = 1;
	p->c = 0;
	p->d = 1;
	S.b10 |= 1;
	S.b11 = 0;
	id = f_25f6e();
	f_2616a(id, p);
	f_26f64(id, 1);
	f_2774a(id, 1);
	
	while (run) {
		f_2b4e8();
		f_9ede();
		if (cnt < 18)
			cnt++;
		if (cnt > 16) {
			unsigned char a = g_6064754;
			unsigned char b = g_6064755;
			if (h40 == (a & 15)) {
				if (--h20 < 0)
					h20 = 1;
				hold = (hold + 1) & 0xfff;
			} else {
				h40 = a & 15;
				h20 = 16;
				hold = 0;
			}
			if (h36 == (b & 15)) {
				if (--h44 < 0)
					h44 = 1;
				hold = (hold + 1) & 0xfff;
			} else {
				h36 = b & 15;
				h44 = 16;
				hold = 0;
			}
		}
		if (g_6079304 & 8) {
			idx++;
			if (idx >= 10)
				idx = 0;
			S.b10 |= 1;
			S.b11 = idx;
			w24 = 0;
			w28 = 0;
			f_25fde(id);
			id = f_25f6e();
			p->spr = tbl[idx];
			f_2616a(id, p);
			f_26f64(id, 1);
			f_2774a(id, 1);
		}
		if (g_6079304 & 4) {
			idx--;
			if (idx < 0)
				idx = 9;
			S.b10 |= 1;
			S.b11 = idx;
			w24 = 0;
			w28 = 0;
			f_25fde(id);
			id = f_25f6e();
			p->spr = tbl[idx];
			f_2616a(id, p);
			f_26f64(id, 1);
			f_2774a(id, 1);
		}
		if (g_6079305 & 8) {
			s0++;
			f_acdc(&s0);
		}
		if (g_6079305 & 4) {
			s0--;
			f_acdc(&s0);
		}
		if (g_6079305 & 2)
			show ^= 1;
		if (show) {
			f_e3e0(10, g_6032da4, 0);
			f_e412(10, 20, g_6032db0, 0);
			f_e412(10, 34, g_6032a18, 0);
			f_e63a(idx, 20, 90, 1, 1, 1, 0);
			f_e63a(s0, 34, 90, 1, 0, 1, 0);
			f_e412(250, 20, g_6032a24, 0);
			f_e412(250, 34, g_6032a28, 0);
			f_e63a(t4 = -w24, 20, 275, 3, 1, 1, 0);
			f_e63a(t4 = -w28, 34, 275, 3, 1, 1, 0);
			f_e412(220, 48, g_6032db8, 0);
			f_e63a(t4 = p->spr->w4, 48, 280, 3, 1, 1, 0);
			f_e412(220, 62, g_6032dc0, 0);
			f_e63a(t4 = p->spr->w6, 62, 280, 3, 1, 1, 0);
			f_e3e0(214, g_6032a2c, 0);
		}
		if ((g_6064754 & 1) == 1)
			run = 0;
		f_2639e(id, w24, w28);
	}
	break;
	}
	*(short *)0x6061944 = 125;
	f_25658(0);
	f_2b4e8();
}

void f_cf68(void)
{
	unsigned short m, h, s;
	long run;
	unsigned long r, q;

	run = 1;
	while (g_6064750)
		f_2b4e8();
	f_29cc6(0, 4, 0, -0x7ffa);
	do {
		f_2b4e8();
		f_9ede();
		f_e3e0(20, g_6032dc8, 0);
		h = g_6065650.a48 / 216000;
		r = g_6065650.a48 - h * 216000;
		m = r / 3600;
		r = r - m * 3600;
		s = r / 60;
		f_e412(60, 60, g_6032dd8, 0);
		f_e412(60, 74, g_6032de4, 0);
		f_e412(60, 88, g_6032df0, 0);
		f_e412(60, 102, g_6032dfc, 0);
		f_e412(60, 130, g_6032e0c, 0);
		f_e412(200, 130, g_6032e1c, 0);
		f_e412(224, 130, g_6032e1c, 0);
		f_e63a(g_6065640, 60, 200, 6, 0, 2, 0);
		f_e63a(g_6065650.w56, 74, 200, 6, 0, 2, 0);
		f_e63a(g_6065650.w58, 88, 200, 6, 0, 2, 0);
		f_e63a(g_6065650.w60, 102, 200, 6, 0, 2, 0);
		f_e63a(h, 130, 160, 5, 0, 2, 0);
		f_e63a(m, 130, 208, 2, 1, 2, 0);
		f_e63a(s, 130, 232, 2, 1, 2, 0);
		q = g_6065650.a52 / g_6065650.w56;
		h = q / 216000;
		r = q - h * 216000;
		m = r / 3600;
		r = r - m * 3600;
		s = r / 60;
		f_e412(60, 144, g_6032e20, 0);
		f_e412(200, 144, g_6032e1c, 0);
		f_e412(224, 144, g_6032e1c, 0);
		f_e63a(h, 144, 160, 5, 0, 2, 0);
		f_e63a(m, 144, 208, 2, 1, 2, 0);
		f_e63a(s, 144, 232, 2, 1, 2, 0);
		f_e3e0(214, g_6032e30, 0);
		if ((g_6079304 & 8) || (g_6079305 & 8))
			run = 0;
	} while (run);
	f_2b4e8();
}

/* rom: 0xd28c len: 0x108 func: f_d28c flags: -macsave=1 -optimize=1 -speed */
void f_d28c(unsigned long t, char *s)
{
	unsigned short sec, min, hour;

	s[5] = ':';
	s[2] = ':';
	if (t >= 5184000UL)
		t %= 5184000UL;
	t /= 60;
	sec = t % 60;
	t /= 60;
	min = t % 60;
	hour = t / 60;
	s[0] = hour / 10 + '0';
	s[1] = hour % 10 + '0';
	s[3] = min / 10 + '0';
	s[4] = min % 10 + '0';
	s[6] = sec / 10 + '0';
	s[7] = sec % 10 + '0';
	s[8] = 0;
}

void f_d33a(unsigned short g, char *s)
{
	if ((g >> 8) >= 16) {
		s[0] = 'A';
		s[1] = 'L';
		s[2] = 'L';
	} else {
		if ((g >> 8) >= 8) {
			s[0] = '2';
			s[2] = (g >> 8) + 41;
		} else {
			s[0] = '1';
			s[2] = (g >> 8) + 49;
		}
		s[1] = '-';
	}
	s[3] = 0;
}

void f_d37c(void)
{
}

void f_d394(void)
{
	short tab[5] = {0, 0, 0, 0, 0};
	long run;
	short w4, w8, mode;
	short i, x, row, col, found;
	long neg;

	run = 1;
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

short f_e39c(char *s)
{
	int w = 0;

	while (*s)
		w += g_6033014[*s++];
	return w;
}

void f_e3b8(void)
{
	f_2af5e(81, 1, (long *)d_67910);
	f_2af5e(82, 1, (long *)d_67910);
}

void f_e3e0(short y, char *s, char flag)
{
	f_e412((320 - f_e39c(s)) / 2, y, s, flag);
}

void f_e412(short x, short y, char *s, char flag)
{
	short cx = x;

	while (*s) {
		if (*s == '\n') {
			cx = x;
			y += 12;
			s++;
		} else if (*s == ' ') {
			cx += 5;
			s++;
		} else {
			f_1865e(d_3ab1c[*s], y, cx, 81, 125, 63, 63, flag);
			cx += g_6033014[*s];
			s++;
		}
	}
}

void f_e490(short x, short y, char *s, short w, char flag)
{
	short cx = x;

	while (*s) {
		if (*s == '\n') {
			cx = x;
			y += 12;
			s++;
		} else if (*s == ' ') {
			cx += 5;
			s++;
		} else {
			f_1865e(d_3ab1c[*s], y, cx, w, 124, 63, 63, flag);
			cx += g_6033014[*s];
			s++;
		}
	}
}

/* rom: 0xe520 len: 0x230 func: f_e520 flags: -macsave=1 -optimize=1 -speed */
void f_1865e(long, short, short, short, short, short, short, short);
extern short g_6033014[];
extern long d_3ad1c[];
extern char d_a7c04[], d_a7c10[], d_a7bf8[], d_a7bec[], d_a7afc[], d_a7b08[], d_a7b14[], d_a7b2c[], d_a7da8[];

void f_e520(short x, short y, char *s, int h)
{
	while (*s != 0) {
		switch (*s) {
		case 'Y':
			f_1865e((long)d_a7c04, y, x, 82, 125, 63, 63, h);
			break;
		case 'N':
			f_1865e((long)d_a7c10, y, x, 82, 125, 63, 63, h);
			break;
		case 'O':
			f_1865e((long)d_a7bf8, y, x, 82, 125, 63, 63, h);
			break;
		case 'X':
			f_1865e((long)d_a7bec, y, x, 82, 125, 63, 63, h);
			break;
		case '1':
			f_1865e((long)d_a7afc, y, x, 82, 125, 63, 63, h);
			break;
		case '2':
			f_1865e((long)d_a7b08, y, x, 82, 125, 63, 63, h);
			break;
		case 'W':
			f_1865e((long)d_a7b14, y - 2, x, 82, 125, 63, 63, h);
			break;
		case '>':
			f_1865e((long)d_a7b2c, y, x, 82, 125, 63, 63, h);
			break;
		default:
			f_1865e((long)d_a7da8, y, x, 82, 125, 63, 63, h);
			break;
		}
		x += g_6033014[*s];
		s++;
	}
}

void f_e63a(int v, short y, int x, short k, long flag, unsigned char mode, char h)
{
	int div = 1;
	int n = k;
	int d;
	long g;


	for (k--; k; k--)
		div *= 10;
	while (n != 0) {
		d = v / div;
		v -= d * div;
		if (d > 9)
			d %= 10;
		if (d == 0) {
			if (flag != 0 || n == 1)
				g = d_3ad1c[0];
			else
				g = 0;
		} else {
			g = d_3ad1c[d];
			flag = 1;
		}
		if (g != 0) {
			f_1865e(g, y, x, 81, 125, 63, 63, h);
			x += 8;
		} else if (mode == 2) {
			x += 8;
		}
		n--;
		div /= 10;
	}
}
