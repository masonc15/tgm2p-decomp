/* rom: 0x26f64 len: 0x8c func: f_26f64 context-rom: 0x26f64 flags: -macsave=1 -optimize=1 -speed */
/* Retain both redraw wrappers and their shared literal pool [26f64,26ff0).
 * Following bodies preserve genuine local-call and literal context.
 * The first wrapper uses a signed 16-bit byte offset into the bank column.
 * The lock wrapper converts three full-width incoming arguments to shorts.
 * Evidence: docs/research/2026-10-13-redraw-current-match.md.
 */
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
/* Bank column: rec104 base 0x060ad228 + w84 offset 0x54. */
extern char g_60ad27c[];
extern struct cell g_60b0fe0[2][20];
extern short g_60b13a4;
extern short g_60b13a0[2];
extern unsigned short g_6060000;
extern unsigned short g_606434c;
extern struct qent g_6064350[];
void f_259ea(void);
void f_25806(void);
short f_26098(void);

void f_26e18(short, struct animent *, struct rec104 *, short, short, short, short, short);
void f_26cb0(short, struct rec104 *, short, short, short);
struct animent *f_2722c(short, short, short *, short *);
void f_26ff0(short, short, short);
void f_26f64(short);
void f_26faa(long, long, long, long);

void f_26f64(short i)
{
    register short cur = g_60ad6a0[i].w6;
    short id = g_60ad6a0[i].a24[cur];
    if (id >= 0) {
        register long offset = (short)(id * (short)sizeof(struct rec104));
        f_26ff0(i, cur, *(short *)&g_60ad27c[offset]);
    }
}

void f_26faa(long i, long cur, long k, long flag)
{
    if (flag) {
        g_60b13ae = 1;
        f_26ff0(i, cur, k);
        g_60b13ae = 0;
    } else f_26ff0(i, cur, k);
}

void f_26ff0(short i, short cur, short k)
{
    struct field *f = REC(i);
    struct rec104 *l;
    struct animent *ae;
    short row, remain;
    char *data;
    long xx, yy, sx, sy, px, py, ix, dx;
    unsigned long *dest;
    remain = 0;
    if (f->a24[cur] == -1) return;
    l = (struct rec104 *)((char *)g_60ad228 + (short)(f->a24[cur] * 104));
    l->w86 = 1;
    sx = (-l->l24[k] - 16) >> 4;
    sy = (-l->l32[k] - 16) >> 4;
    px = (short)(((-l->l24[k] - 16) >> 4) + (-l->l76 >> 4));
    py = ((-l->l40[k] - 16) >> 4) + (-l->l80 >> 4);
    row = 0;
    for (yy = 0; yy < 32; yy++, py++) {
        dest = l->l4[k] + ((sy + yy) & 31);
        ae = f_2722c(i, py, &row, &remain);
        if (ae->a->flags & 0x100000) {
            data = (char *)(ae->a + 1) + row * ae->a->w4 * 4;
            dx = px;
            xx = 0; ix = sx;
            do {
                long cx = dx % ae->a->w4;
                if (cx < 0) cx += ae->a->w4;
                *(unsigned long *)((long)dest + ((ix & 15) << 7)) = ((unsigned long *)data)[cx];
                xx++; ix++; dx++;
            } while (xx < 16);
        } else {
            data = (char *)(ae->a + 1) + row * ae->a->w4 * 2;
            dx = px;
            xx = 0; ix = sx;
            do {
                long cx = dx % ae->a->w4;
                if (cx < 0) cx += ae->a->w4;
                *(unsigned long *)((long)dest + ((ix & 15) << 7)) = ((unsigned short *)data)[cx] | ae->a->flags;
                xx++; ix++; dx++;
            } while (xx < 16);
        }
    }
    l->w56[k] %= 16;
    l->w60[k] %= 16;
}

struct animent *f_2722c(short i, short y, short *row, short *remain)
{
    struct field *f = REC(i);
    struct rec104 *l;
    struct animent *ae, *first;
    unsigned short id;
    id = f->a24[(unsigned short)f->w6];
    if ((long)id < 0) return 0;
    l = (struct rec104 *)((char *)g_60ad228 + (short)(id * 104));
    ae = l->ae;
    first = ae;
    if (ae->a) {
        *row = y;
        while (*row < 0) {
            if (l->l96) {
                *row += l->l100;
                ae = (struct animent *)l->l96;
                first = ae;
            } else *row += l->l20;
        }
next_entry:
        if (ae->a->w6 * ae->n > *row) {
            *remain = ae->a->w6 - *row - 1;
            *row %= ae->a->w6;
        } else {
            *row -= ae->a->w6 * (short)ae->n;
            ae++;
            if (!ae->a) {
                if (l->l96) ae = (struct animent *)l->l96;
                else ae = first;
            }
            goto next_entry;
        }
    }
    return ae;
}
