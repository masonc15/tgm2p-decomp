/* rom: 0xfc8c len: 0x14c func: f_fc8c flags: -macsave=1 -optimize=1 -speed */
extern void f_1159c(long, long, long, long, long);
extern long d_3ad44[];
extern char d_a71b4[];

/* Draws a frame count as MM:SS:CC (CC in hundredths) with digit glyphs. */
void f_fc8c(unsigned long t, long x, long y)
{
	unsigned char m, s, c;

	m = t / 3600;
	t -= m * 3600;
	s = t / 60;
	t -= s * 60;
	c = t * 100 / 60;
	f_1159c(d_3ad44[m / 10], 0xd7, x, y, 40);
	f_1159c(d_3ad44[m % 10], 0xd7, x + 16, y, 40);
	f_1159c((long)d_a71b4, 0xd7, x + 28, y, 40);
	f_1159c(d_3ad44[s / 10], 0xd7, x + 40, y, 40);
	f_1159c(d_3ad44[s % 10], 0xd7, x + 56, y, 40);
	/* written differently from the first colon so SHC doesn't CSE the address */
	f_1159c((long)&d_a71b4[0], 0xd7, x + 68, y, 40);
	f_1159c(d_3ad44[c / 10], 0xd7, x + 80, y, 40);
	f_1159c(d_3ad44[c % 10], 0xd7, x + 96, y, 40);
}
