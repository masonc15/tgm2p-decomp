/* rom: 0x2635c len: 0x220 func: f_2635c flags: -macsave=1 -optimize=1 -speed */
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
extern void (*f_f928(short))();
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


void f_2635c(short i)
{
	short id;

	if (g_60ad6a0[i].a24[g_60ad6a0[i].w6] >= 0) {
		id = g_60ad6a0[i].a24[g_60ad6a0[i].w6];
		g_60ad228[id].l96 = 0;
	}
}

void f_2639e(short i, short x, short y)
{
	struct rec104 *l;
	short k;
	int dx, dy;

	if (g_60ad6a0[i].a24[g_60ad6a0[i].w6] < 0)
		return;
	l = &g_60ad228[g_60ad6a0[i].a24[g_60ad6a0[i].w6]];
	k = l->w84;
	dx = x - l->w48[k];
	l->w56[k] += dx;
	dy = y - l->w52[k];
	l->w60[k] += dy;
	l->w68 = 0;
	l->w74 = 0;
	if (l->w64[k] >= 0) {
		if (dx > 0) {
			l->w64[k] = -1;
			l->w68 = 1;
		}
	} else if (dx < 0) {
		l->w64[k] = 1;
		l->w68 = 1;
	}
	if (l->w70[k] >= 0) {
		if (dy > 0) {
			l->w70[k] = -1;
			l->w74 = 1;
		}
	} else if (dy < 0) {
		l->w70[k] = 1;
		l->w74 = 1;
	}
	if (l->w88 == 1) {
		l->w90[0] = dy <= 0 ? 1 : 0;
		l->w90[1] = dy <= 0 ? 1 : 0;
		l->w88 = 0;
	}
	if (g_60ad6a0[i].w72 == 1) {
		g_60ad6a0[i].l64 = 0;
		g_60ad6a0[i].w72 = 0;
	}
	l->l24[k] += x - l->w48[k];
	l->l32[k] += y - l->w52[k];
	l->l40[k] += y - l->w52[k];
	l->w48[k] = x;
	l->w52[k] = y;
	g_60ad6a0[i].l64 += y - g_60ad6a0[i].l68;
	g_60ad6a0[i].l68 = y;
}
