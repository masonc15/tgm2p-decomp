/* rom: 0x7f38 len: 0x68 func: f_7f38 flags: -macsave=1 -optimize=1 -speed */
struct sub {
	char pad[0x379];
	unsigned char b379;
};
struct obj {
	char pad0[0x2f8];
	struct sub *p2f8;          /* 0x2f8 */
	char pad1[0x308 - 0x2fc];
	unsigned long l308;        /* 0x308 */
	char pad2[0x388 - 0x30c];
	char b388;                 /* 0x388 */
};
extern void f_32fc(struct obj *);
extern void f_2f68(struct obj *, int);
extern void f_35a0(struct obj *);

void f_7f38(struct obj *o)
{
	struct obj *p;

	if (!(o->l308 & 0x40000000)) {
		if (o->b388 != 0 && o->p2f8->b379 != 18 && o->p2f8->b379 != 19) {
			f_32fc(o);
		} else {
			p = o;
			f_2f68(p, 1);
		}
	}
	f_35a0(o);
}
