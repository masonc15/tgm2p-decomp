/* rom: 0x27310 len: 0x2a func: f_27310 context-rom: 0x27310 flags: -macsave=1 -optimize=1 -speed */
/* Retain only callback27310–2733a. The genuine queue producer, tile writer
 * and callback dispatch supply compiler context and remain uncounted. */
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




struct board_tile {
    short x, y;
    unsigned char height, pad5, width, pad7, palette, attr;
    unsigned short tile;
};
void f_274c0(short, long, short, struct board_tile *, long, long);
void f_27310(long, unsigned long, struct board_tile *, long, long);

void f_27310(long i, unsigned long packed, struct board_tile *p, long a, long b)
{
    f_274c0((short)i, (short)(packed >> 16), (short)packed, p, a, b);
}

void f_2733a(short i, long p, long a, long b)
{
    struct field *f = REC(i);
    short k;
    long cur;
    short zero;
    if (f->a24[f->w6] < 0)
        return;
    cur = f->w6;
    k = ((struct rec104 *)((char *)g_60ad228 + (short)(f->a24[(short)cur] * 104)))->w84;
    f_274c0(i, cur, k, (struct board_tile *)p, a, b);
    zero = (k == 0);
    g_60b0fe0[g_60b13a4][g_60b13a0[g_60b13a4]].fn = f_27310;
    g_60b0fe0[g_60b13a4][g_60b13a0[g_60b13a4]].a4 = i;
    g_60b0fe0[g_60b13a4][g_60b13a0[g_60b13a4]].a8 = ((unsigned long)f->w6 << 16) | zero;
    g_60b0fe0[g_60b13a4][g_60b13a0[g_60b13a4]].a12 = p;
    g_60b0fe0[g_60b13a4][g_60b13a0[g_60b13a4]].a16 = a;
    g_60b0fe0[g_60b13a4][g_60b13a0[g_60b13a4]].a20 = b;
    g_60b13a0[g_60b13a4]++;
}

void f_274c0(short i, long pos, short k, struct board_tile *p, long a, long b)
{
    short cur = (short)pos;
    struct field *f = REC(i);
    struct rec104 *rec;
    long *dest;
    short count, xx, yy, x, y;
    int n;
    long clip;
    unsigned long tile;
    if (f->a24[cur] < 0)
        return;
    rec = (struct rec104 *)((char *)g_60ad228 + (short)(f->a24[cur] * 104));
    count = (unsigned short)p->y >> 10;
    a -= rec->l76;
    b -= rec->l80;
    a /= 16;
    b /= 16;
    dest = rec->l4[k];
    for (n = 0; n < count; n++, p++) {
        if (p->x & 0x200)
            x = p->x | 0xfc00;
        else
            x = p->x & 0x3ff;
        if (p->y & 0x200)
            y = p->y | 0xfc00;
        else
            y = p->y & 0x3ff;
        x /= 16;
        y /= 16;
        tile = ((unsigned long)(p->attr & 15) << 16) | p->tile;
        for (yy = 0; yy <= (p->height & 15); yy++) {
            short px = (short)a + yy + x;
            for (xx = 0; xx <= (p->width & 15); xx++) {
                short py = (short)b + xx + y;
                clip = rec->l24[k] >> 4;
                if (px >= clip && px < clip + 16)
                    dest[((px & 15) << 5) + (py & 31)] = ((unsigned long)p->palette << 24) | tile;
                tile++;
            }
        }
    }
}

void f_2769e(short i)
{
    struct field *f = REC(i);
    if (f->cb48) {
        f->cb48(f);
        f->cb48 = 0;
        return;
    }
    if (f->cb52)
        f->cb52(f);
}
