/* rom: 0x2ac4c len: 0x374 func: f_2ac4c flags: -macsave=1 -optimize=1 -speed */
void f_2ac4c(long a, long b, long c)
{
	unsigned long *s1 = (unsigned long *)b;
	unsigned long *s2 = (unsigned long *)c;
	char t = a >> 24;
	int u;
	unsigned char pal = a;
	int n = (unsigned char)(a >> 16);
	short i;
	register short j;
	register unsigned long out;
	unsigned long c1, c2;
	unsigned char x1, x2;
	unsigned long buf[16];

	u = 64 - t;
	for (i = 0; i < n; i++) {
		for (j = 0; j < 16; j++) {
			c1 = *(unsigned long *)((int)s1 + j * 4);
			x1 = 0xfc & (c1 >> 24);
			c2 = *(unsigned long *)((int)s2 + j * 4);
			x2 = 0xfc & (c2 >> 24);
			out = 0;
			out |= ((u * x1 + x2 * t) << 18) & 0xfc000000;
			x1 = 0xfc & (c1 >> 16); x2 = 0xfc & (c2 >> 16);
			out |= ((u * x1 + x2 * t) << 10) & 0xfc0000;
			x1 = 0xfc & (c1 >> 8); x2 = 0xfc & (c2 >> 8);
			out |= ((u * x1 + x2 * t) << 2) & 0xfc00;
			*(unsigned long *)((int)buf + j * 4) = out;
		}
		for (j = 0; j < 32; j++)
			*(short *)(0x24040000 + i * 64 + pal * 64 + j * 2) = *(short *)((int)buf + j * 2);
		s1 += 16;
		s2 += 16;
	}
}

struct task {
	void (*func)();
	long a;
	long b;
	long c;
};
extern struct task g_6064350[];
extern unsigned short g_606434c;
extern void f_2afc0(long a, long b, long c);

void f_2af5e(unsigned char pal, unsigned char n, long *src)
{
	g_6064350[g_606434c].func = f_2afc0;
	g_6064350[g_606434c].a = pal;
	g_6064350[g_606434c].b = n;
	g_6064350[g_606434c++].c = (long)src;
}
