/* rom: 0x26264 len: 0xb6 func: f_26264 flags: -macsave=1 -optimize=1 -speed */
/* Non-matching: functionally equivalent C for [0x26264, 0x2631a) (f_26264),
 * linked only by the shiftable build (splits_nonmatching.txt). Not matching coverage.
 * From src/board_255dc.c. */
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
extern int g_dummy;


void f_255dc(short i);

void f_25658(void);

void f_257f8(void);

void f_25806(void);

void f_25938(short i, long *a, long *b);

void f_259bc(void);

/* CONTEXT ONLY until f_25f6e. Not linked, not counted as matched C.
 * f_259ea has temp-register swaps in two of its field blocks and f_26264
 * needs an unused word load; the functions after them supply the pool. */
void f_259ea(void);

short f_25f6e(void);

void f_25fde(short i);

short f_26098(void);

void f_26156(short i);

short f_2616a(short i, struct animent *ae);

void f_2622c(short i, struct animent *ae, long dx, long dy);

short f_26264(short i, struct animent *ae)
{
	short id;
	struct animent *e;
	short x;
	struct field *f;
	short *p;
	char *b;
	int k;
	if (g_60ad6a0[i].a24[g_60ad6a0[i].w6] < 0)
		id = g_60ad6a0[i].a24[g_60ad6a0[i].w6] = f_26098();
	else
		id = g_60ad6a0[i].a24[g_60ad6a0[i].w6];
	f = &g_60ad6a0[i];
	p = f->a24;
	x = *p;
	if (0) f->a24[3] = x;
	g_60ad228[id].ae = ae;
	g_60ad228[id].l20 = 0;
	for (e = ae; e->a; e++)
		g_60ad228[id].l20 += e->a->w6 * e->n;
	g_60ad228[id].l76 = 0;
	g_60ad228[id].l80 = 0;
	return id;
}

void f_2631a(short i, struct animent *ae, long dx, long dy);
