/* rom: 0x60e8 len: 0xe8 func: f_60e8 */
struct P {
	char pad0[0x308];
	long l308;                 /* 0x308 */
	unsigned short w30c;       /* 0x30c */
	char pad1[0x322 - 0x30e];
	unsigned short w322;       /* 0x322 */
	char pad2[0x346 - 0x324];
	unsigned char b346;        /* 0x346 */
	char pad3[0x370 - 0x347];
	long l370;                 /* 0x370 */
	long l374;                 /* 0x374 */
	char pad4[0x37e - 0x378];
	unsigned char b37e;        /* 0x37e */
};
extern const unsigned char tbl_3a7ca[];
extern void f_6007820(struct P *p);

void f_608c(struct P *p)
{
}

void f_60e8(struct P *p)
{
	unsigned short f = p->w30c;

	if ((f & 1) && !(f & 8) && !(p->l308 & 0x20)) {
		if (p->w322 >= (p->b346 + 1) * 100) {
			long mask = 1 << tbl_3a7ca[p->b346];
			if (p->l374 & mask) {
				p->l374 &= ~mask;
				f_608c(p);
				p->b346++;
			}
			return;
		}
	}
	if ((p->w30c & 0x208) && p->b37e >= 20 && p->l370)
		f_6007820(p);
}
