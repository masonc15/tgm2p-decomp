/* rom: 0x2f2cc len: 0x1c func: f_2f2cc flags: -macsave=1 -optimize=0 */
extern unsigned char g_60b180e[10];
extern short g_60b181e, g_60b181c;
extern unsigned char g_60b1758[];
extern short g_60b17d0[];
extern struct { unsigned char a, b; } g_60b1728[];
extern short g_60b1820[];

extern long g_60b1874, g_60b1870;

void f_2ed3e(void)
{
}

void f_2f25c(void)
{
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
