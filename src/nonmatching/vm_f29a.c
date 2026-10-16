/* rom: 0xf29a len: 0x22c func: f_f29a flags: -macsave=1 -optimize=1 -speed */
/* Non-matching: functionally equivalent C for [0xf29a, 0xf4c6) (f_f29a, f_f3a2),
 * linked only by the shiftable build (splits_nonmatching.txt). Not matching coverage.
 * From src/vm_empty_f292.c. */
/* The two argument tails preserve the original register ABI. f_2c6fc
 * consumes low16 of the long coordinate/mode arguments; its short result
 * is ignored by this void VM handler. */
struct obj {
	char pad0[0x1a];
	short x1a;                 /* 0x1a */
	char pad1[0x31 - 0x1c];
	unsigned char b31;         /* 0x31 */
	char pad2[0x34 - 0x32];
	short x34;                 /* 0x34 */
};
struct vm {
	char pad0[0x20];
	struct obj *obj;           /* 0x20 */
	char pad1[0x80 - 0x24];
	long regs[1];              /* 0x80 */
};
struct spr {
	char pad0[0x18];
	short x18;                 /* 0x18 */
	char pad1[0x1c - 0x1a];
	unsigned char b1c;         /* 0x1c */
	char pad2[0x22 - 0x1d];
	short x22;                 /* 0x22 */
	char pad3[0x40 - 0x24];
};
extern struct vm *g_606005c;
extern struct spr g_606006c[];
extern long g_6060004;
extern void f_2c3ca(long);
extern short f_2ce54(long, long, long, long);
extern short f_2c042(long, long, long);
extern void f_2443c(void (*)(void), long, long, long, long, long, long, long);
extern void f_2418e(struct vm *, void (*)(void), long, long, long);
extern void f_29c50(long);
extern short f_2cf62(long, long, long);
extern void f_2c6fc(long *, long, long, long);

void f_f200(unsigned long a);

void f_f292(void);

void f_f296(void);

void f_f29a(long a)
{
	register short n = a;
	register short t = (a & 0xff0000) >> 16;
	struct obj *o = g_606005c->obj;
	short cur = o->b31;
	short dir = 0;
	short d;
	long p0, p1, p3;
	void (*p2)(void);

	if (n > 0 && cur != t) {
		o->x1a |= 2;
		d = (t == 0 ? t | 0x100 : (unsigned char)t) - cur;
		if (d < 0) {
			dir = 1;
			d = d < 0 ? -d : d;
		} else if (d > 0x80) {
			if ((d < 0 ? -d : d) < 0x100) {
				dir = 1;
				d = 0xff - d;
			}
		}
		d = (unsigned char)d;
		p1 = ((unsigned char)d << 8) | (n << 16) | cur;
		p0 = ((d << 8) / n) << 16;
		p3 = (o->x34 << 16) | dir;
		f_2443c(p2 = f_f296, 16, 0, 0, 0, 0, o->x34, 0);
		f_2418e(g_606005c, p2, p1, p0, p3);
	}
}

void f_f3a2(unsigned long a)
{
 long v = g_606005c->regs[a];
 register short n = v;
 short t = (v & 0xffff0000) >> 16;
 register struct obj *o = g_606005c->obj;
 short dir = 0;
 short cur;
 short d;
 long p0, p1, p3;
 void (*p2)(void);
 o->x1a |= 2;
 if (t == 0) d = t | 0x100;
 else d = (unsigned char)t;
 cur = g_606005c->obj->b31;
 if (n <= 0 || cur == d) {
  o->x1a &= 0xfffd;
  return;
 }
 d -= cur;
 if (d < 0) {
  dir = 1;
  d = d < 0 ? -d : d;
 } else if (d > 0x80) {
  if ((d < 0 ? -d : d) < 0x100) {
   dir = 1;
   d = (unsigned char)(0xff - d);
  }
 }
 d = (unsigned char)d;
 p1 = ((unsigned char)d << 8) | (n << 16) | cur;
 p0 = ((d << 8) / n) << 16;
 p3 = (o->x34 << 16) | dir;
 f_2443c(p2 = f_f296, 16, 0, 0, 0, 0, o->x34, 0);
 f_2418e(g_606005c, p2, p1, p0, p3);
}

void f_f4c6(unsigned long a);

void f_f4dc(void);

void f_f4e0(void);

void f_f4e4(void);

void f_f4e8(void);

void f_f4ec(void);

void f_f4f0(void);

void f_f4f4(void);

void f_f4f8(void);

void f_f4fc(void);

void f_f500(void);

void f_f504(void);

void f_f508(void);

void f_f50c(void);

void f_f510(void);

void f_f514(void);

void f_f518(unsigned long a);

void f_f574(unsigned long a);
