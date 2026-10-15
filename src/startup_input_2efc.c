/* rom: 0x2e4c len: 0xf0 func: f_2e4c context-rom: 0x2e4c literal-tail-calls: 1 flags: -macsave=1 -optimize=1 -speed */
/* f_2e4c and f_2efc compile as one exact native context with their shared pool. */
struct cell { unsigned short value; char pad[4]; };
struct field {
    struct cell *cells;
    char pad0[0xde - 4];
    unsigned char height, width;
    char pad1[0x30c - 0xe0];
    unsigned short mode;
    unsigned char id;
};
extern unsigned long g_6060008;
extern unsigned char d_363c6[];
extern unsigned char g_606475a[];
extern void f_2e6fc(short);

void f_2e4c(struct field *f)
{
    short count = 0;
    short x, y;
    short h = f->height, w = f->width;
    for (y = h - 6; y < h - 1; y++) {
        for (x = 1; x < w - 1; x++) {
            if (f->cells[y * f->width + x].value)
                count++;
        }
    }
    if (count > 10) {
        if (g_6060008 % 20 == 0) {
            count /= 10L;
            if (4 < count)
                count = 4;
            f_2e6fc(d_363c6[count]);
        }
    }
}

void f_2efc(struct field *f)
{
    unsigned char id = f->id;
    id = g_606475a[id];
    if (id == 7)
        f->mode |= 0x200;
}
