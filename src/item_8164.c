/* rom: 0x8164 len: 0x68 func: f_8164 context-rom: 0x7fa0 flags: -macsave=1 -optimize=1 -speed */
/* Complete genuine predecessor context is compiled below; only f_8164 and
 * its naturally emitted literal pool are retained by the default verifier. */
struct field {
    char pad0[0x308];
    unsigned long flags308;
    unsigned short mode;
    unsigned char side;
    char pad30f[0x35e - 0x30f];
    unsigned short w35e;
    char pad360[0x370 - 0x360];
    unsigned long mask370;
};
struct scratch { char pad[0x374]; unsigned char weights[19]; };
extern struct scratch g_6077608;
extern unsigned char d_38fcc[];
extern long g_606487c, g_6079538;
extern unsigned long g_6060008;
extern unsigned char g_6079374[];
extern unsigned long f_2beca(unsigned long);
extern void f_608c(struct field *, unsigned short);
extern void f_21d94(struct field *);
extern void f_18240(struct field *);
void f_7fa0(struct field *f)
{
    unsigned short i, pick, acc;
    register unsigned short j;
    short sum, retry;
    unsigned char *p;
    for (i = 0; i < 19; i++) g_6077608.weights[i] = d_38fcc[i];
    for (i = 0; i < 19; i++)
        if (!((1UL << i) & f->mask370)) g_6077608.weights[i] = 0;
    sum = 0;
    for (i = 0; i < 19; i++) sum += g_6077608.weights[i];
    if (sum > 0) {
        retry = 1;
        while (retry) {
            if (g_606487c && g_6079538 == 5) {
                if (f->side == 0) pick = 0;
                else pick = 50;
            } else pick = (f_2beca(4750) + g_6060008) % sum + 1;
            acc = 0;
            for (j = 0; j < 19; j++) {
                acc += g_6077608.weights[j];
                if (acc >= pick) break;
            }
            if (j == 19) break;
            if ((1UL << j) & f->mask370) {
                f_608c(f, j);
                retry = 0;
            }
        }
    }
}
void f_8156(struct field *f)
{
    f->flags308 |= 0x20;
    f_21d94(f);
}
void f_8164(struct field *f)
{
    f->flags308 &= 0xfffdbfff;
    f_18240(f);
    if (f->w35e & 0x2000) g_6079374[f->side] = 0;
}
