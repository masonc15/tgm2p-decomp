/* rom: 0xf958 len: 0x284 func: f_f958 flags: -macsave=1 -optimize=1 -speed */
struct fld {
	char pad0[0x30c];
	unsigned short x30c;       /* 0x30c */
	unsigned char b30e;        /* 0x30e */
	char pad1[0x318 - 0x30f];
	short x318;                /* 0x318 */
	short x31a;                /* 0x31a */
	short x31c;                /* 0x31c */
	short x31e;                /* 0x31e */
	short x320;                /* 0x320 */
	unsigned short x322;       /* 0x322 */
	char pad2[0x339 - 0x324];
	unsigned char b339;        /* 0x339 */
	char pad3[0x37e - 0x33a];
	unsigned char b37e;        /* 0x37e */
};
extern long g_6060008;
extern short g_6060038;
extern short d_36314[];
extern char d_a6cd4[], d_a6ce0[], d_a6cec[];
extern void f_11680(char *, short, short, short, short, char, char, short);
extern void f_1159c(char *, int, short, int, int);

void f_f958(struct fld *p, short b)
{
	char e;
	short dy, dx, s;

	if (p->x30c & 0x200) {
		p->x318 += p->x31a;
		if (p->x318 < 0) {
			p->x31c = 0;
			p->x31a = 0;
			p->x318 = 0;
		} else
			p->x31a -= p->x31c;
		if (!(p->b339 & 16)) {
			p->b339 |= 16;
			p->x318 = 0;
			p->x31a = 0x800;
			p->x31c = 0x100;
		} else if (!(g_6060008 & 31) && p->b37e > 17) {
			short v = p->b37e - 17;

			p->x318 = 0;
			p->x31a = v << 9;
			p->x31c = v << 5;
		}
		dy = -((e = *(char *)&p->x318) * 18) / 64;
		dx = -(e * 6) / 64;
		f_11680(d_a6cd4, dx + 26, b + dy, 0, 61, s, s = e + 63, 0);
		return;
	}
	f_1159c(d_a6cd4, 26, b, 0, 61);
}

void f_fa78(struct fld *p)
{
	short v;

	if (!p->b30e)
		v = 108;
	else
		v = 0x10c;
	f_1159c(d_a6ce0, 0x82, v, 0, 40);
}

void f_fac8(struct fld *p, short b, short c)
{
	char e;
	short dy, dx, s;

	if ((p->x30c & 8) && p->x322 >= d_36314[g_6060038] - 20) {
		p->x31e += p->x320;
		if (p->x31e < 0) {
			p->x31e = 0;
			p->x320 = 0;
		} else
			p->x320 -= 64;
		if (!(g_6060008 & 31)) {
			p->x31e = 0;
			p->x320 = 0x400;
		}
		dy = -((e = *(char *)&p->x31e) * 32) / 128;
		dx = -(e * 8) / 128;
		f_11680(d_a6cec, b + dx, c + dy, 0, 110, s, s = e + 63, 0);
		return;
	}
	f_1159c(d_a6cec, b, c, 0, 40);
}
