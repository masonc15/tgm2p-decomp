/* rom: 0xee9c len: 0x68 func: f_ee9c context-rom: 0xec0c flags: -macsave=1 -optimize=1 -speed */
/* Only EE9C-EF04 is retained: five complete functions, 104 bytes. The
 * preceding 656 bytes provide compiler context and remain assembly.
 * done increments at most once per row across three rows (range 0..3).
 * The sprite word accesses use the same signed-short field, stride 64,
 * offset 26, as struct spr.x1a. No context bytes receive C credit. */
struct vm {
	char pad0[0x80];
	long regs[4];              /* 0x80 */
};
struct spr {
	char pad0[0x14];
	unsigned char b14;
	unsigned char b15;
	char pad1[0x1a - 0x16];
	short x1a;
	char pad2[0x40 - 0x1c];
};
struct s30 { char c[30]; };
struct s2 { char a, b; };
extern struct vm *g_606005c;
extern struct spr g_606006c[];
extern struct s30 g_607930c[];
extern struct s2 g_6079366[];
extern short g_607936e;
extern short g_603585a[];
extern struct s2 g_603343c;

extern short f_2bf58(char *, short, short);
extern long _divls(long, long);

void f_ec0c(unsigned long a)
{
	short spr;
	long col;
	char *line;
	long row;
	unsigned char done;
	char *str;
	struct s2 bufs;
	char *buf;
	long i;
	long x;
	short y;

	done = 0;
	str = (char *)g_606005c->regs[a];
	bufs = g_603343c;
	buf = (char *)&bufs;
	i = row = col = 0;
	x = 24;
	do {
		line = (char *)&g_607930c[0] + (char)(row * 30);
		y = 62 - row * 29;
		for (;; i++) {
			buf[0] = str[i];
			if (buf[0] == 0) {
				done++;
				break;
			}
			if (i <= g_607936e - 4) {
				line[col] = buf[0];
				x += g_603585a[buf[0]];
				col++;
				line[col] = 0;
				if (i == g_607936e - 4)
					line[col] = 0;
			} else if (i == g_607936e - 3) {
				g_6079366[0].a = buf[0];
				spr = f_2bf58(&g_6079366[0].a, x, y);
				x += g_603585a[buf[0]];
				*(short *)((char *)g_606006c + (long)spr * 64 + 26) = 0xfe;
				g_606006c[spr].b14 = 63;
				g_606006c[spr].b15 = 59;
			} else if (i == g_607936e - 2) {
				g_6079366[1].a = buf[0];
				spr = f_2bf58(&g_6079366[1].a, x, y);
				x += g_603585a[buf[0]];
				g_606006c[spr].x1a = 0xfe;
				g_606006c[spr].b14 = 63;
				g_606006c[spr].b15 = 49;
			} else if (i == g_607936e - 1) {
				g_6079366[2].a = buf[0];
				spr = f_2bf58(&g_6079366[2].a, x, y);
				x += g_603585a[buf[0]];
				g_606006c[spr].x1a = 0xfe;
				g_606006c[spr].b14 = 63;
				g_606006c[spr].b15 = 35;
			} else if (i == g_607936e) {
				g_6079366[3].a = buf[0];
				spr = f_2bf58(&g_6079366[3].a, x, y);
				x += g_603585a[buf[0]];
				*(short *)((char *)g_606006c + (long)spr * 64 + 26) = 0xfe;
				g_606006c[spr].b14 = 63;
				g_606006c[spr].b15 = 20;
			} else
				break;
		}
		i++;
		row++;
		col = 0;
		x = 24;
	} while (row < 3);
	if (done == 3)
		g_606005c->regs[1] = 1;
	{ char *rows_base = (char *)g_607930c;
	g_606006c[f_2bf58(rows_base, 24, 62)].x1a = 0xfe;
	g_606006c[f_2bf58(rows_base + 30, 24, 34)].x1a = 0xfe;
	g_606006c[f_2bf58(rows_base + 60, 24, 4)].x1a = 0xfe;
	}
}

void f_ee9c(unsigned long a)
{
	g_607936e = g_606005c->regs[a];
}

void f_eeb0(void)
{
}

void f_eeb4(void)
{
}

void f_eeb8(void)
{
}

void f_eebc(unsigned long a)
{
	long m = (unsigned char)(a >> 8);
	short d = a & 0xff;
	a &= 0xffff0000;
	g_606005c->regs[d] = (long)a / m;
}

