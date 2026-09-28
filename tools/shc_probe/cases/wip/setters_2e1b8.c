/* rom: 0x2e1b8 len: 0x58 func: f_2e1b8 flags: -macsave=1 -optimize=0 */
extern long g_60b1870, g_60b1858, g_60b187c;

void f_2e1b8(long v)
{
	g_60b1870 = v;
}

void f_2e1c8(long v)
{
	g_60b1858 = v * 48 / 60;
}

void f_2e1ee(long v)
{
	g_60b187c = v;
}
