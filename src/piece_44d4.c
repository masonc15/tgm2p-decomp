/* rom: 0x44d4 len: 0xc0c func: f_44d4 flags: -macsave=1 -optimize=1 -speed */
/* Stamp the current piece into the playfield, including doubled pieces,
 * per-cell flags and timers. Coordinates are the high words of fixed-point
 * positions; the low words are not read here. */
struct cell { unsigned short flags; unsigned char b2, b3; short timer; };
union bits { unsigned short word; struct { unsigned char hi, lo; } byte; };
struct pos { short x; char pad[2]; short y; };
struct field {
    struct cell *cells;
    char pad4[0xde-4];
    unsigned char height, width;
    char pade0[0x308-0xe0];
    unsigned long flags308;
    unsigned short mode;
    char pad30e[0x338-0x30e];
    unsigned char b338;
    char pad339[0x35c-0x339];
    unsigned short state;
    union bits piece;
    char pad360[2];
    unsigned char rotation;
    char pad363;
    struct pos position;
    char pad36a[0x37f-0x36a];
    unsigned char b37f;
};
extern unsigned char d_363cc[16][4][4][4];
extern char g_607cf0c;
extern void f_31dc(struct field *, unsigned char);
extern void f_2e6fc(short);
#define CELL f->cells[((y-i)*f->width+x)+j]
void f_44d4(struct field *f, unsigned char how)
{
    short x, y, i, j, timer;
    unsigned char rot, kind, rule;
    struct pos *position;
    if (f->state & 0x8000) {
        f_31dc(f, 0);
        return;
    }
    /* Keep the shared coordinate base and the byte-sized rule local: both
     * affect SHC's register allocation and stack-slot order. */
    position = &f->position;
    x = position->x;
    y = position->y;
    rot = f->rotation;
    kind = f->piece.byte.lo & 15;
    rule = f->b338 & 0x75;
    if (rule == 0x75) timer = 3;
    else timer = 300;
    if (g_607cf0c && (f->flags308 & 0x200000)) timer = 0;
    if (f->piece.word & 0x200) {
        /* This order preserves the loop-counter initialization register. */
        y += 1;
        x -= 2;
        for (i = 0; i < 8; i++) {
            if (y-i < f->height) {
                for (j = 0; j < 8; j++) {
                    if (d_363cc[kind][i/2][rot][j/2]) {
                        if (how != 2) CELL.flags = f->piece.word | 0xa0;
                        else CELL.flags = f->piece.word;
                        if (f->piece.word & 0x2000) {
                            CELL.flags |= 0x2000;
                            CELL.b3 = f->b37f;
                        }
                        if ((f->mode & 0x10) && how != 2 && timer) {
                            CELL.flags |= 0x1000;
                            CELL.timer = timer;
                        }
                        if (how == 1) CELL.flags |= 0x8000;
                    }
                }
            }
        }
    } else {
        for (i = 0; i < 4; i++) {
            if (y-i < f->height) {
                for (j = 0; j < 4; j++) {
                    if (d_363cc[kind][i][rot][j]) {
                        if (how != 2) CELL.flags = f->piece.word | 0xa0;
                        else CELL.flags = f->piece.word;
                        if (f->piece.word & 0x2000) {
                            CELL.flags |= 0x2000;
                            CELL.b3 = f->b37f;
                        }
                        if ((f->mode & 0x10) && how != 2 && timer) {
                            CELL.flags |= 0x1000;
                            CELL.timer = timer;
                        }
                        if (how == 1) CELL.flags |= 0x8000;
                    }
                }
            }
        }
    }
    f->b37f = 0;
    if (how == 0) f_2e6fc(12);
}
