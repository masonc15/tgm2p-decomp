/* rom: 0x9c0c len: 0x61a func: f_9c0c flags: -macsave=1 -optimize=1 -speed */
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
extern long g_60792f0;
extern short g_60792ec;
extern short g_6060022;
extern char **g_607926c;
extern char g_60b1819;
extern char g_6061945;
struct snd { unsigned char b0; char pad[13]; short we; };
extern struct snd g_6066198;
extern unsigned char g_6064754, g_6064755;
/* g_6079304/5: input bytes this frame; g_6079306/7: per-bit latches */
extern unsigned char g_6079304, g_6079305, g_6079306, g_6079307;

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
extern void f_29cc6(short, short, long, long);
extern void f_bd8a(void);
extern void f_2b4e8(void);
extern void f_2f2cc(void);
extern void f_2f3dc(void);
extern void f_2ecea(void);
extern void f_2edc2(void);
extern void f_2eeaa(int);
extern void f_2774a(short, int);
extern void f_e3b8(void);

/* 0x2405ffxx: hardware registers at 0x0405ffxx, through the cache-through mirror */
#define REG(a) (*(unsigned char *)(a))

void f_9c0c(void)
{
	f_2b1cc();
	g_60792f0 = rom_hdr_ptr->l30;
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

/* f_9ede expands both helpers inline; they are also emitted on their own. */
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
