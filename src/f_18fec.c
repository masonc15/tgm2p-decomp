/* rom: 0x18fec len: 0x28 func: f_18fec flags: -macsave=1 -optimize=1 -speed */
extern void f_2af5e(int, int, void *);
extern char d_67950[], d_67d50[];

void f_18fec(void)
{
	f_2af5e(0xb4, 16, d_67950);
	f_2af5e(0xc4, 1, d_67d50);
}
