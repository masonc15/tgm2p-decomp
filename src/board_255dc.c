/* rom: 0x255dc len: 0x35c func: f_255dc flags: -macsave=1 -optimize=1 -speed */
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


void f_255dc(short i)
{
	int j;
	int k;
	g_60ad6a0[i].w2 = 0;
	g_60ad6a0[i].w4 = 1;
	g_60ad6a0[i].w6 = 0;
	g_60ad6a0[i].w10 = 3;
	g_60ad6a0[i].w12 = 0;
	g_60ad6a0[i].w14 = 0;
	g_60ad6a0[i].w16 = 0;
	g_60ad6a0[i].l20 = 0;
	g_60ad6a0[i].cb48 = 0;
	g_60ad6a0[i].cb52 = 0;
	g_60ad6a0[i].w8 = 0;
	g_60ad6a0[i].l64 = 0;
	g_60ad6a0[i].l68 = 0;
	g_60ad6a0[i].l76 = 0;
	for (j = 0; j < 11; j++)
		g_60ad6a0[i].a24[j] = -1;
	for (k = 0; k < 4; k++)
		g_60ad6a0[i].a56[k] = 0;
}

void f_25658(void)
{
	int i, j;

	*(char *)0x2405fff8 = 10;
	*(char *)0x2405fff9 = 12;
	*(char *)0x2405fffa = 14;
	*(char *)0x2405fffb = 16;
	*(char *)0x2405fffe = 0;
	*(char *)0x2405ffff = 0;
	*(char *)0x2405ffeb |= 0x40;
	g_60b13a4 = 0;
	for (i = 0; i < 4; i++) {
		g_60ad6a0[i].w0 = 0;
		f_255dc(i);
	}
	for (i = 0; i < 11; i++) {
		g_60ad228[i].w0 = 0;
		for (j = 0; j < 2; j++) {
			g_60ad228[i].l4[j] = (long *)(0x24005000 + i * 4096 + j * 2048);
			g_60ad228[i].w12[j] = j + i * 2 + 10;
		}
		g_60ad228[i].ae = 0;
		g_60ad228[i].l20 = 0;
		g_60ad228[i].l96 = 0;
		g_60ad228[i].l100 = 0;
	}
	for (i = 0; i < 2; i++) {
		for (j = 0; j < 20; j++) {
			g_60b0fe0[i][j].fn = 0;
			g_60b0fe0[i][j].a4 = 0;
			g_60b0fe0[i][j].a8 = 0;
			g_60b0fe0[i][j].a12 = 0;
			g_60b0fe0[i][j].a16 = 0;
			g_60b0fe0[i][j].a20 = 0;
		}
	}
}

void f_257f8(void)
{
	if (g_6060000 > 0)
		return;
	f_25806();
}

void f_25806(void)
{
	int i;
	short b;

	b = (g_60b13a4 == 0) ? 1 : 0;
	for (i = 0; i < 20; i++) {
		if (g_60b0fe0[b][i].fn) {
			g_60b0fe0[b][i].fn(g_60b0fe0[b][i].a4, g_60b0fe0[b][i].a8, g_60b0fe0[b][i].a12, g_60b0fe0[b][i].a16, g_60b0fe0[b][i].a20);
			g_60b0fe0[b][i].fn = 0;
		}
	}
	g_60b13a0[b] = 0;
}

