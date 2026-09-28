/* rom: 0xfbdc len: 0xb0 func: f_fbdc flags: -macsave=1 -optimize=1 -speed */
struct fld {
	char pad0[0x322];
	unsigned short x322;       /* 0x322 */
	char pad1[0x358 - 0x324];
	unsigned long x358;        /* 0x358 */
};
extern void f_11254(long, long, long, char, long, long, long, long);
extern void f_1159c(long, long, long, char, long);
extern long d_3ade4[];

void f_fbdc(struct fld *f, unsigned short b, long c, long d, char e)
{
	unsigned long n;

	f_11254(f->x322, c, d, e, 40, 3, 0, 2);
	f_11254(b, c + 15, d, e, 40, 3, 0, 2);
	n = f->x358 >> 15;
	if (n > 20)
		n = 20;
	f_1159c(d_3ade4[n], c + 11, d, e, 40);
}
