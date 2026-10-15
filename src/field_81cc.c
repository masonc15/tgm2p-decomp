/* rom: 0x8314 len: 0x204 func: f_8314 context-rom: 0x81cc literal-tail-calls: 1 flags: -macsave=1 -optimize=1 -speed */
/* Verified retained range 0x8314-0x8518: f_8314/f_837a/f_8430.
 * Genuine preceding f_81cc context is unlinked and uncounted. */
struct cell { unsigned short flags; unsigned char b2, b3; short b; };
struct field {
    struct cell *cells;
    char pad4[0xde - 4];
    unsigned char height, width;
    char pade0[0x300 - 0xe0];
    short w300, w302;
    char pad304[4];
    unsigned long flags308;
    unsigned short mode;
    char pad30e[0x322 - 0x30e];
    unsigned short level;
    char pad324[0x339 - 0x324];
    unsigned char b339;
};
unsigned char f_81cc(struct field *f)
{
    short y, x, k;
    unsigned char n;
    for (y = 1, n = 0; y < f->height - 1; y++) {
        for (x = 1, k = 0; x < f->width - 1; x++)
            if (f->cells[y * f->width + x].flags != 0) {} else k++;
        if (k == 1) {
            if (y < 10) {
                if (f->cells[y * f->width + y].flags == 0) {
                    if (((struct cell *)((long)&f->cells->flags + ((((y+1)*f->width)+y)*6)))->flags != 0) n++;
                } else y = f->height;
            } else {
                k = 20 - y;
                if (f->cells[y * f->width + k].flags == 0) {
                    if (((struct cell *)((long)&f->cells->flags + ((((y+1)*f->width)+k)*6)))->flags != 0) n++;
                } else y = f->height;
            }
        } else y = f->height;
    }
    if (n >= 19) n = 20;
    return n;
}

/* Verified range begins here. */
void f_8314(struct field *f)
{
    short y, x;
    struct cell *p;
    for (y = 1; y < f->height - 1; y++) {
        p = &f->cells[y * f->width + 1];
        for (x = 1; x < f->width - 1; x++, p++) p->flags &= 0xaf7f;
    }
}
extern long d_37a68[], d_3660c[], d_375b0[], d_37f1c[];
/* Gravity is a full 32-bit 16.16 value; callers use its high word. */
long f_837a(struct field *f)
{
    long v;
    unsigned long n;
    if (f->mode & 4) v = d_37a68[f->level];
    else if (f->mode & 10) v = d_3660c[f->level];
    else v = d_375b0[f->level];
    if (f->mode & 0x80) v = d_37f1c[f->level];
    else if (f->mode & 0x1000) {
        n = f->level + 700;
        if (n >= 1000) n = 999;
        v = d_3660c[n];
    }
    if (f->mode & 0x2000) v = d_3660c[999];
    if (f->b339 & 8) v = 0x4000;
    return v;
}
extern void f_17f10(struct field *, short);
extern void f_21d94(struct field *);
extern void f_31dc(struct field *, short);
/* Retained f_8430 has one proven ordered long r1 tail. */
void f_8430(struct field *f)
{
    short x;
    f->b339 &= 0xfb;
    if (++f->w300 % 6 == 0) {
        f_17f10(f, f->w302);
        for (x = 1; x < f->width - 1; x++) f->cells[f->w302 * f->width + x].flags = 0;
        f->w302++;
        if (f->w302 == f->height - 1) {
            f->flags308 |= 0x20;
            f_21d94(f);
            f->mode |= 16;
            f_31dc(f, 1);
        }
    }
}
