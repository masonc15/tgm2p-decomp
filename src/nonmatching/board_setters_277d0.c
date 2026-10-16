/* rom: 0x277d0 len: 0xf0 func: f_277d0 flags: -macsave=1 -optimize=1 -speed */
/* Non-matching: functionally equivalent C for [0x277d0, 0x278c0) (f_277d0, f_27862),
 * linked only by the shiftable build (splits_nonmatching.txt). Not matching coverage.
 * From src/board_setters_2774a.c. */
struct field {
	short w0;
	short w2;
	short w4;
	short w6;
	short w8;
	short w10;
	short w12;
	short w14;
	short w16;
	char pad18[2];
	long l20;
	short a24[11];
	char pad46[2];
	void (*cb48)();
	void (*cb52)();
	short a56[4];
	long l64;
	long l68;
	short w72;
	char pad74[2];
	long l76;
	long x80[224];
	long x976[224];
	char pad1872[0xe50 - 1872];
};
struct anim {
	long flags;
	short w4;
	short w6;
};
struct animent {
	struct anim *a;
	long n;
};
struct rec104 {
	short w0;
	char pad2[2];
	long *l4[2];
	short w12[2];
	struct animent *ae;
	long l20;
	long l24[2];
	long l32[2];
	long l40[2];
	short w48[2];
	short w52[2];
	short w56[2];
	short w60[2];
	short w64[2];
	short w68;
	short w70[2];
	short w74;
	long l76;
	long l80;
	short w84;
	short w86;
	short w88;
	short w90[2];
	char pad94[2];
	long l96;
	long l100;
};
struct cell {
	void (*fn)();
	long a4, a8, a12, a16, a20;
};
struct qent {
	long fn;
	long pad[3];
};
extern struct field g_60ad6a0[4];
extern long f_f928(int);
extern char *g_606005c;
extern short g_60b13b0, g_60b13b2, g_60b13ae;
struct tsk {
	char pad0[8];
	short w8;
	char pad10[6];
	short w16;
};
#define REC(i) ((struct field *)((char *)g_60ad6a0 + (short)((i) * (short)sizeof(struct field))))
extern struct rec104 g_60ad228[11];
extern struct cell g_60b0fe0[2][20];
extern short g_60b13a4;
extern short g_60b13a0[2];
extern unsigned short g_6060000;
extern unsigned short g_606434c;
extern struct qent g_6064350[];
void f_259ea(void);
void f_25806(void);
short f_26098(void);

void f_26e18(short, struct animent *, struct rec104 *, short, short, short, unsigned long, long);
void f_26cb0(short, struct rec104 *, short, short, short);
struct animent *f_2722c(short, short, short *, short *);
void f_26ff0(short, short, short);
void f_26f64(short, int, long *);
void f_274c0(short, short, short, long, long, long);
void f_27310();


void f_276e8(short i, register int a, short b);

void f_2774a(short i, short v);

void f_2775c(short i, short v);

void f_2776e(short i, short v);

void f_27780(short i, short v);

void f_27792(short i, short k, short v);

void f_277ac(short i, short v);

void f_277be(short i, short v);

void f_277d0(struct tsk *t)
{
	int id;
	t->w16 = (t->w16 + 1) & 1;
	if (t->w16) { if (t->w8 == 0) { id = f_26098(); t->w8 = id; return; } g_60ad228[t->w8].w0 = 0; id = f_26098(); t->w8 = id; return; }
	if (t->w8) { g_60ad228[t->w8].w0 = 0; t->w8 = 0; }
}

void f_27862(int idx)
{
	short s;
	long *p;

	s = *(short *)(g_606005c + 82);
	g_60b13b0 = (unsigned long)*(long *)(g_606005c + 128 + idx * 4) >> 16;
	p = (long *)(g_606005c + 128 + idx * 4);
	g_60b13b2 = *p;
	g_60b13ae = 1;
	f_26f64(s, 0, p);
	g_60b13ae = 0;
}
