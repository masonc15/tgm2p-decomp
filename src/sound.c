/* rom: 0x2e1b8 len: 0x1c20 func: f_2e1b8 flags: -macsave=1 -optimize=0 -speed */
/* Sound driver state at 0x60b1860. These fields have to be struct members:
 * SHC -optimize=0 keeps a plain global's address in a register across
 * statements, but reloads it for every member access, which the ROM shows
 * (e.g. the two reads of the volume in f_2ea8a). */
struct snd_state {
	unsigned char tick;	/* 60 */
	char pad61;
	short w62;		/* 62 */
	char pad64;
	unsigned char note;	/* 65 */
	char vol;		/* 66 */
	char b67;		/* 67 */
	char b68;		/* 68 */
	char ch;		/* 69: current FM channel */
};
extern struct snd_state g_60b1860;
#define g_60b1862 (g_60b1860.w62)
#define g_60b1865 (g_60b1860.note)
#define g_60b1866 (g_60b1860.vol)
#define g_60b1867 (g_60b1860.b67)
#define g_60b1868 (g_60b1860.b68)
#define g_60b1869 (g_60b1860.ch)
extern short g_60b186a;
extern short g_60b186c;
extern char g_60b186e;
extern char g_60b186f;
extern long g_60b1870, g_60b187c, g_60b1874, g_60b1878;
extern long g_60b1840[];
extern short g_60b181c, g_60b181e;
extern long g_606002c, g_6060028;
extern char g_6064767;
extern char g_60b1877;
extern char g_60b1819;
extern short g_60b17d0[];
extern char g_60b1800[];
extern char g_60b1758[];
void f_2ecea(void);
void f_2e87c(char i, short v);
extern short g_60b17a0[];
extern short g_60b1820[];
extern char g_60b181d;
struct drum { short w0; char b2; char b3; };
extern char g_60b1818;
extern char g_60b1770[];
extern char g_60b1788[];
extern unsigned short g_6035990[16][12];
#define IO_PORT (*(volatile unsigned char *)0x23000004)
#define FM0_ADDR (*(volatile unsigned char *)0x23100000)
#define FM0_DATA (*(volatile unsigned char *)0x23100001)
#define FM1_ADDR (*(volatile unsigned char *)0x23100002)
#define FM1_DATA (*(volatile unsigned char *)0x23100003)
#define OPL_STAT (*(volatile unsigned char *)0x23100000)
#define OPL_REG (*(volatile unsigned char *)0x23100004)
#define OPL_DATA (*(volatile unsigned char *)0x23100005)
void f_2f44a(void);
void f_2f3ec(void);
extern char g_60b180e[10];
struct pair { unsigned char a, b; };
extern struct pair g_60b1728[];
void f_2f418(void);
void f_2f3dc(void);
void f_2ee38(void);
void f_2ea8a(void);
void f_2e6fc(short v);
struct sfx { short w0; char b2; char b3; unsigned char flags; char b5; };
/* sound data pointer, read from the ROM header through the cache-through mirror */
#define snd_base (*(char **)0x20040034)
struct chan { char prog; char b; char c; };
extern struct chan g_60b16f8[];
void f_2fc00(void);
void f_2fcfe(void);
void f_2f852(char v);
void f_2fa30(char v);
char f_2f93c(void);
void f_2f4c0(void);
void f_2f7a6(char v);
extern char g_60b187f;
struct s1638 { char wait; unsigned char *ptr; unsigned char *loop; };
extern struct s1638 g_60b1638[16];

void f_2ed06(void);
void f_2ed3e(void);
void f_2f304(void);
void f_2f25c(void);
void f_2f2cc(void);
void f_2f71c(void);
void f_2e8e6(void);
void f_2e362(struct s1638 *p);
void f_2e5aa(char n);

void f_2e1b8(long v) { g_60b1870 = v; }
void f_2e1c8(long v) { g_60b1840[6] = v * 48 / 60; }
void f_2e1ee(long v) { g_60b187c = v; }

void f_2e210(long v)
{
	if (v)
		g_60b181c = 14;
	else
		g_60b181c = 14;
	f_2ed06();
	f_2f304();
	f_2ed3e();
	f_2f25c();
	g_60b1874 = 0;
	g_60b1870 = 0;
}

void f_2e254(void)
{
	char i;

	f_2f71c();
	f_2e8e6();
	g_60b1840[4] += g_606002c * g_60b1840[6];
	g_606002c = 0;
	g_60b1860.tick = g_60b1840[4] / 60;
	g_60b1840[4] %= 60;
	g_6060028 = 0;
	for (i = 0; i < 16; i++) {
		if (g_60b1638[i].ptr)
			f_2e362(&g_60b1638[i]);
	}
	if (g_60b1878 && g_60b1870 && g_60b1874) {
		g_6064767 = -1;
		f_2e5aa(g_60b1877 - 1);
		g_60b1874 = 0;
	}
	g_60b1878 = 0;
	if (g_60b181e)
		g_60b181e--;
}

void f_2e362(struct s1638 *p)
{
	unsigned char b;
	unsigned char cmd;
	char ch;
	char t;
	struct chan *tbl;

	tbl = g_60b16f8;
	t = g_60b1860.tick;
	while (t) {
		if (p->wait > t) {
			p->wait -= t;
			return;
		} else
			t -= p->wait;
		while ((b = *p->ptr) & 0x80) {
			p->ptr++;
			ch = b & 15;
			cmd = b & 0xf0;
			if (cmd == 0x80) {
				g_60b1865 = *p->ptr++;
				g_60b1866 = *p->ptr++;
				tbl[ch].b = g_60b1866;
				if (tbl[ch].c == 0) {
					g_60b186a = ch;
					if (ch == 9)
						f_2fc00();
					else
						f_2f852(tbl[ch].prog);
				}
			} else if (cmd == 0x90) {
				g_60b1865 = *p->ptr++;
				if (ch == 9)
					f_2fcfe();
				else
					f_2fa30(tbl[ch].prog);
			} else if (cmd == 0xc0) {
				tbl[ch].prog = *p->ptr++;
			} else if (b == 0xd3) {
				p->ptr = 0;
				return;
			} else if (cmd == 0xa0) {
				p->loop = p->ptr;
			} else if (cmd == 0xb0) {
				p->ptr = p->loop;
				if (ch == 15)
					g_60b1878 = 1;
			} else if (b == 0xd0) {
				g_60b1840[6] = *p->ptr++;
				g_60b1840[4] = 0;
				g_6060028 = 0;
				g_606002c = 0;
			}
		}
		p->wait = b;
		p->ptr++;
	}
}

void f_2e5aa(char n)
{
	int i;
	unsigned char *p;
	long unused;
	struct chan *tbl;
	struct s1638 *seq;

	if (g_60b1819)
		return;
	if (g_60b1870) {
		if (g_6064767 != -1) {
			g_60b1874 = n + 1;
			return;
		}
	}
	tbl = g_60b16f8;
	seq = g_60b1638;
	g_60b1840[4] = 0;
	g_60b1840[6] = 60;
	g_6064767 = n;
	f_2f2cc();
	for (i = 0; i < 16; i++) {
		tbl[i].prog = 0;
		tbl[i].b = 0;
		p = ((unsigned char **)(snd_base + 0xf00))[n * 16 + i];
		seq[i].wait = 0;
		seq[i].ptr = p;
		seq[i].loop = seq[i].ptr;
		p++;
	}
	g_6060028 = 0;
	g_606002c = 0;
}

void f_2e6fc(short v)
{
	int i;
	int j;

	if (v) {
		if (g_60b1819)
			return;
	} else {
		f_2ecea();
		g_60b181e = 60;
	}
	g_60b186c = v & 0x8000;
	v &= 0x7fff;
	for (i = g_60b181c; i < 24; i++) {
		if (g_60b17d0[i] == v) {
			f_2e87c(i, v);
			return;
		}
	}
	for (j = 0; j < 24; j++) {
		for (i = g_60b181c; i < 24; i++) {
			if (g_60b1800[i] == (((struct sfx *)(snd_base + 0x300))[v].flags & 7) && g_60b1758[i] <= 0) {
				f_2e87c(i, v);
				return;
			}
		}
		for (i = g_60b181c; i < 24; i++) {
			if (g_60b1800[i] == (((struct sfx *)(snd_base + 0x300))[v].flags & 7) && g_60b1758[i])
				g_60b1758[i]--;
		}
	}
}

void f_2e87c(char i, short v)
{
	if (!g_60b17a0[i]) {
		g_60b1869 = i;
		f_2f44a();
	}
	if (g_60b186c) {
		g_60b1758[i] = 0;
		g_60b17d0[i] = -1;
		g_60b17a0[i] = 0;
		return;
	}
	g_60b17a0[i] = v + 1;
}

void f_2e8e6(void)
{
	short s;
	int i;

	for (i = g_60b181c; i < 24; i++) {
		if (g_60b17a0[i]) {
			s = g_60b17a0[i] - 1;
			g_60b17a0[i] = 0;
			g_60b17d0[i] = s;
			if (!g_60b1800[i] && g_60b181e && s) {
				g_60b1758[i] = 0;
				break;
			}
			g_60b1758[i] = 24 - (char)g_60b181c;
			g_60b1866 = ((struct sfx *)(snd_base + 0x300))[s].b2;
			g_60b1865 = ((struct sfx *)(snd_base + 0x300))[s].b5;
			g_60b1862 = ((struct sfx *)(snd_base + 0x300))[s].w0;
			g_60b1867 = ((struct sfx *)(snd_base + 0x300))[s].b3;
			g_60b1868 = 32;
			g_60b1869 = i;
			g_60b186a = 16;
			f_2ea8a();
			g_60b1820[i] = 0x1a4;
		} else if (g_60b1820[i]) {
			g_60b1820[i]--;
			if (g_60b1820[i] < 4) {
				g_60b1869 = i;
				f_2f44a();
			}
		}
	}
}

void f_2ea8a(void)
{
	register char reg;
	register int data;
	register char ch;
	char oct;
	char note;
	char tl;

	ch = g_60b1869;
	reg = ch + 0x68;
	while (OPL_STAT & 1)
		;
	OPL_REG = reg;
	data = 0;
	data |= 0x38;
	while (OPL_STAT & 1)
		;
	OPL_DATA = data;
	oct = g_60b1865 / 12 - 5;
	note = g_60b1865 % 12;
	reg = ch + 0x38;
	while (OPL_STAT & 1)
		;
	OPL_REG = reg;
	data = g_6035990[g_60b186a][note] >> 7;
	data &= 7;
	data |= oct << 4;
	while (OPL_STAT & 1)
		;
	OPL_DATA = data;
	reg = ch + 0x20;
	while (OPL_STAT & 1)
		;
	OPL_REG = reg;
	data = g_60b1862;
	data >>= 4;
	data >>= 4;
	data &= 1;
	data |= g_6035990[g_60b186a][note] << 1;
	while (OPL_STAT & 1)
		;
	OPL_DATA = data;
	tl = (unsigned char)g_60b1866 / 4;
	tl += (unsigned char)g_60b1866 / 2;
	tl |= 1;
	reg = ch + 0x50;
	while (OPL_STAT & 1)
		;
	OPL_REG = reg;
	if (g_60b1818)
		g_60b1866 = tl;
	data = ~((unsigned char)g_60b1866 << 1);
	while (OPL_STAT & 1)
		;
	OPL_DATA = data;
	reg = ch + 8;
	while (OPL_STAT & 1)
		;
	OPL_REG = reg;
	g_60b1770[ch] = 0;
	if (ch < g_60b181c)
		g_60b1788[ch] = g_60b1868 | 0x87;
	else
		g_60b1788[ch] = g_60b1868 | 0x89;
	data = g_60b1862;
	while (OPL_STAT & 1)
		;
	OPL_DATA = data;
}

void f_2ece0(void)
{
	g_60b1818 = 1;
}

void f_2ecea(void)
{
	g_60b1818 = 0;
}

void f_2ecf4(void)
{
	f_2ecea();
	f_2e6fc(0);
}

void f_2ed06(void)
{
	for (g_60b1869 = (char)g_60b181c; (unsigned char)g_60b1869 < 22; g_60b1869++)
		f_2f44a();
}

void f_2ed3e(void)
{
	int i;

	for (g_60b1869 = 0; (unsigned char)g_60b1869 < g_60b181c; g_60b1869++)
		f_2f44a();
	for (i = 0; i < 16; i++)
		g_60b1638[i].ptr = 0;
	g_6064767 = -1;
	g_60b1874 = 0;
}

void f_2edb8(void)
{
	g_60b1819 = 1;
}

void f_2edc2(void)
{
	g_60b1819 = 0;
}

void f_2edcc(char ch, char v)
{
	struct chan *tbl;

	tbl = g_60b16f8;
	ch &= 15;
	v &= 1;
	tbl[ch].c = v;
}

char f_2ee0c(char ch)
{
	struct chan *tbl;

	tbl = g_60b16f8;
	ch &= 15;
	return tbl[ch].c;
}

void f_2ee38(void)
{
	char i;
	struct chan *tbl;

	tbl = g_60b16f8;
	for (i = 0; i < 16; i++)
		tbl[i].c = 0;
}

void f_2ee74(void)
{
	char i;
	struct chan *tbl;

	tbl = g_60b16f8;
	for (i = 0; i < 16; i++)
		tbl[i].c = 1;
}

void f_2eeaa(char v)
{
	g_60b186e = (7 - (v & 7)) << 3;
	f_2f3ec();
	OPL_REG = 0xf9;
	f_2f3ec();
	OPL_DATA = g_60b186e | g_60b186f;
}

void f_2ef00(char v)
{
	g_60b186f = 7 - (v & 7);
	f_2f3ec();
	OPL_REG = 0xf9;
	f_2f3ec();
	OPL_DATA = g_60b186e | g_60b186f;
}

void f_2ef3e(void)
{
	char ch;
	int i;
	short b;
	short a;

	b = 0;
	for (i = 0; i < 1000; i++)
		b++;
	IO_PORT = 2;
	g_60b181c = 14;
	g_60b186a = 0;
	for (i = 0; i < b; i++)
		a--;
	b = a;
	f_2f3ec(); FM1_ADDR = 5;
	f_2f3ec(); FM1_DATA = 3;
	f_2f3ec(); OPL_REG = 0;
	f_2f3ec(); OPL_DATA = 0;
	f_2f3ec(); OPL_REG = 1;
	f_2f3ec(); OPL_DATA = 0;
	f_2f3ec(); FM0_ADDR = 0;
	f_2f3ec(); FM0_DATA = 0;
	f_2f3ec(); FM0_ADDR = 1;
	f_2f3ec(); FM0_DATA = 0;
	f_2f3ec(); FM1_ADDR = 0;
	f_2f3ec(); FM1_DATA = 0;
	f_2f3ec(); FM1_ADDR = 1;
	f_2f3ec(); FM1_DATA = 0;
	f_2f3ec(); FM0_ADDR = 4;
	f_2f3ec(); FM0_DATA = 0xe0;
	f_2f3ec(); FM0_ADDR = 2;
	f_2f3ec(); FM0_DATA = 0xff;
	f_2f3ec(); FM0_ADDR = 3;
	f_2f3ec(); FM0_DATA = 0xff;
	f_2f3ec(); FM0_ADDR = 5;
	f_2f3ec(); FM0_DATA = 0;
	f_2f3ec(); FM0_ADDR = 8;
	f_2f3ec(); FM0_DATA = 0;
	f_2f3ec(); FM1_ADDR = 2;
	f_2f3ec(); FM1_DATA = 0;
	f_2f3ec(); FM1_ADDR = 3;
	f_2f3ec(); FM1_DATA = 0;
	f_2f3ec(); FM1_ADDR = 8;
	f_2f3ec(); FM1_DATA = 0;
	f_2f3ec(); FM1_ADDR = 0xbd;
	f_2f3ec(); FM1_DATA = 0;
	for (ch = 0; ch < 22; ch++) {
		f_2f3ec(); FM0_ADDR = ch + 64;
		f_2f3ec(); FM0_DATA = 63;
		f_2f3ec(); FM1_ADDR = ch + 64;
		f_2f3ec(); FM1_DATA = 63;
	}
	f_2f3ec(); OPL_REG = 2;
	f_2f3ec(); OPL_DATA = 0;
	f_2f3ec(); OPL_REG = 7;
	f_2f3ec(); OPL_DATA = 0;
	for (ch = 0; ch < 24; ch++) {
		g_60b1869 = ch;
		f_2f44a();
		f_2f3ec(); OPL_REG = ch + 56;
		f_2f3ec(); OPL_DATA = 0;
		f_2f3ec(); OPL_REG = ch + 32;
		f_2f3ec(); OPL_DATA = 0;
		f_2f3ec(); OPL_REG = ch + 8;
		f_2f3ec(); OPL_DATA = 0;
		f_2f418();
	}
	f_2f3ec(); OPL_REG = 0xf8;
	f_2f3ec(); OPL_DATA = 63;
	f_2f3ec(); OPL_REG = 0xf9;
	f_2f3ec(); OPL_DATA = 0;
	f_2f2cc();
	f_2f3dc();
	f_2ee38();
	g_60b1819 = 0;
	g_60b1818 = 0;
	g_60b186e = 0;
	g_60b186f = 0;
	g_60b187c = 0;
}

void f_2f25c(void)
{
	int i;

	for (i = g_60b181c; i < 24; i++) {
		g_60b1788[i] = 0;
		g_60b17a0[i] = 0;
		g_60b1758[i] = 0;
		g_60b1770[i] = 1;
	}
	g_6060028 = 0;
}

void f_2f2cc(void)
{
	f_2ed3e();
	f_2f25c();
	g_60b1874 = 0;
	g_60b1870 = 0;
}

void f_2f304(void)
{
	int i;

	g_60b180e[9] = 0;
	g_60b180e[8] = 0;
	g_60b180e[7] = 2;
	g_60b180e[6] = 2;
	g_60b180e[5] = 2;
	g_60b180e[4] = 3;
	g_60b180e[3] = 3;
	g_60b180e[2] = 3;
	g_60b180e[1] = 5;
	g_60b180e[0] = 5;
	g_60b181e = 0;
	for (i = g_60b181c; i < 24; i++) {
		g_60b1758[i] = 0;
		g_60b17d0[i] = -1;
		g_60b1728[i].b = 0xff;
		g_60b1728[i].a = 0xff;
		g_60b1820[i] = 0;
	}
}

void f_2f3dc(void)
{
	f_2ed06();
	f_2f304();
}

void f_2f3ec(void)
{
	int i;

	for (i = 0; i < 64; i++)
		;
	while (OPL_STAT & 1)
		;
}

void f_2f418(void)
{
	int i;

	for (i = 0; i < 64; i++)
		;
	while (OPL_STAT & 2)
		;
}

void f_2f44a(void)
{
	register char reg;
	register char data;

	reg = g_60b1869 + 80;
	while (OPL_STAT & 1)
		;
	OPL_REG = reg;
	data = 0xff;
	while (OPL_STAT & 1)
		;
	OPL_DATA = data;
	reg = g_60b1869 + 104;
	while (OPL_STAT & 1)
		;
	OPL_REG = reg;
	data = 120;
	while (OPL_STAT & 1)
		;
	OPL_DATA = data;
}

void f_2f4c0(void)
{
	register char reg;
	register int data;
	register char ch;
	char oct;
	char note;
	char tl;

	ch = g_60b1869;
	reg = ch + 0x50;
	while (OPL_STAT & 1)
		;
	OPL_REG = reg;
	data = 0;
	data |= -1;
	while (OPL_STAT & 1)
		;
	OPL_DATA = data;
	reg = ch + 0x68;
	while (OPL_STAT & 1)
		;
	OPL_REG = reg;
	data = 0;
	data |= 0x30;
	while (OPL_STAT & 1)
		;
	OPL_DATA = data;
	oct = g_60b1865 / 12 - 5;
	note = g_60b1865 % 12;
	reg = ch + 0x38;
	while (OPL_STAT & 1)
		;
	OPL_REG = reg;
	data = g_6035990[g_60b186a][note] >> 7;
	data &= 7;
	data |= oct << 4;
	while (OPL_STAT & 1)
		;
	OPL_DATA = data;
	reg = ch + 0x20;
	while (OPL_STAT & 1)
		;
	OPL_REG = reg;
	data = g_60b1862;
	data >>= 4;
	data >>= 4;
	data &= 1;
	data |= g_6035990[g_60b186a][note] << 1;
	while (OPL_STAT & 1)
		;
	OPL_DATA = data;
	tl = (unsigned char)g_60b1866 / 4;
	tl += (unsigned char)g_60b1866 / 2;
	reg = ch + 0x50;
	while (OPL_STAT & 1)
		;
	OPL_REG = reg;
	if (g_60b1818)
		g_60b1866 = tl;
	data = ~((unsigned char)g_60b1866 << 1);
	while (OPL_STAT & 1)
		;
	OPL_DATA = data;
	reg = ch + 8;
	while (OPL_STAT & 1)
		;
	OPL_REG = reg;
	g_60b1770[ch] = 0;
	if (ch < g_60b181c)
		g_60b1788[ch] = g_60b1868 | 0x87;
	else
		g_60b1788[ch] = g_60b1868 | 0x89;
	data = g_60b1862;
	while (OPL_STAT & 1)
		;
	OPL_DATA = data;
}

void f_2f71c(void)
{
	register char reg;
	int i;

	f_2f418();
	reg = 104;
	for (i = 0; i < 24; i++) {
		if (g_60b1788[i]) {
			while (OPL_STAT & 1)
				;
			OPL_REG = reg;
			while (OPL_STAT & 1)
				;
			OPL_DATA = g_60b1788[i];
			g_60b1788[i] = 0;
		}
		reg++;
	}
}

void f_2f7a6(char v)
{
	register char reg;
	register char ch;

	ch = g_60b1869;
	reg = ch + 104;
	g_60b1728[ch].b = 0xff;
	g_60b1728[ch].a = 0xff;
	while (OPL_STAT & 1)
		;
	OPL_REG = reg;
	g_60b1758[ch] = 0;
	g_60b1770[ch] = 1;
	if (ch < g_60b181c)
		v |= 7;
	else
		v |= 9;
	while (OPL_STAT & 1)
		;
	OPL_DATA = v;
}

void f_2f852(char v)
{
	char ch;

	if (*(short *)((int)snd_base + v * 2) == -1)
		return;
	ch = f_2f93c();
	g_60b1728[ch].b = g_60b1865 + g_60b187f;
	g_60b1758[ch] = (char)g_60b181c + 1;
	g_60b1728[ch].a = v;
	g_60b1862 = *(short *)((int)snd_base + v * 2);
	g_60b1868 = 0;
	g_60b1869 = ch;
	f_2f4c0();
	for (ch = 0; ch < g_60b181c - 1; ch++) {
		if (g_60b1758[ch]) {
			if (--g_60b1758[ch] == 0)
				g_60b1770[ch] = 1;
		}
	}
}

char f_2f93c(void)
{
	char i;
	char best;

	best = 0;
	for (i = 1; i < g_60b181c - 1; i++) {
		if (g_60b1770[i]) {
			if (g_60b1770[best] < g_60b1770[i])
				best = i;
		}
	}
	if (g_60b1770[best] == 0) {
		for (;;) {
			for (i = 0; g_60b1758[i] && i < g_60b181c - 1; i++)
				;
			if (i >= g_60b181c - 1) {
				for (i = 0; i < g_60b181c - 1; i++) {
					if (g_60b1758[i]) {
						g_60b1758[i]--;
						if (g_60b1758[i] == 0)
							g_60b1770[i] = 1;
					}
				}
			} else
				return i;
		}
	}
	return best;
}

void f_2fa30(char v)
{
	char ch;

	if (*(short *)((int)snd_base + v * 2) == -1)
		return;
	for (ch = 0; ch < g_60b181c - 1; ch++) {
		if (g_60b1728[ch].b == g_60b1865 && g_60b1728[ch].a == v) {
			g_60b1869 = ch;
			f_2f7a6(0);
			for (ch = 0; ch < g_60b181c - 1; ch++) {
				if (g_60b1758[ch])
					g_60b1758[ch]++;
				if (g_60b1770[ch])
					g_60b1770[ch]++;
			}
			return;
		}
	}
}

char f_2fb06(void)
{
	char i;
	char best;

	best = (char)g_60b181c - 1;
	for (i = (char)g_60b181c; i < g_60b181c; i++) {
		if (g_60b1770[i]) {
			if (g_60b1770[best] < g_60b1770[i])
				best = i;
		}
	}
	if (!g_60b1770[best]) {
		for (;;) {
			for (i = (char)g_60b181c - 1; g_60b1758[i] && i < g_60b181c; i++)
				;
			if (i >= g_60b181c) {
				for (i = (char)g_60b181c - 1; i < g_60b181c; i++) {
					if (g_60b1758[i]) {
						g_60b1758[i]--;
						if (g_60b1758[i] == 0)
							g_60b1770[i] = 1;
					}
				}
			} else
				return i;
		}
	} else
		return best;
}

void f_2fc00(void)
{
	long unused[4];
	char i;

	if (((struct drum *)(snd_base + 0x100))[g_60b1865].w0 == -1)
		return;
	i = f_2fb06();
	g_60b1758[i] = 3;
	g_60b1728[i].b = g_60b1865;
	g_60b1728[i].a = 0xff;
	g_60b1862 = ((struct drum *)(snd_base + 0x100))[g_60b1865].w0;
	g_60b1865 = ((struct drum *)(snd_base + 0x100))[g_60b1865].b2;
	g_60b1868 = 32;
	g_60b1869 = i;
	f_2f4c0();
	for (i = (char)g_60b181c - 1; i < g_60b181c; i++) {
		if (g_60b1758[i]) {
			if (--g_60b1758[i] == 0)
				g_60b1770[i] = 1;
		}
	}
}

void f_2fcfe(void)
{
	char i;

	if (((struct drum *)(snd_base + 0x100))[g_60b1865].w0 == -1)
		return;
	for (i = (char)g_60b181c - 1; i < g_60b181c; i++) {
		if (g_60b1728[i].b == g_60b1865) {
			g_60b1869 = i;
			f_2f7a6(0);
			for (i = (char)g_60b181c - 1; i < g_60b181c; i++) {
				if (g_60b1758[i])
					g_60b1758[i]++;
				if (g_60b1770[i])
					g_60b1770[i]++;
			}
			return;
		}
	}
}
