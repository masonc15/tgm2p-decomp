/* rom: 0x213da len: 0x7a func: f_213da context-rom: 0x21170 flags: -macsave=1 -optimize=1 -speed */
/* C identifier-list EEPROM definitions receive promoted integer arguments.
 * Callers use no-prototype declarations; the implementation stores byte addresses.
 * Both sides are verified against the original machine code (checkpoint122). */
/* Retain the complete EEPROM status read/compare and write pair213da-21454.
 * Genuine preceding checksum/reset/counter/read/write context supplies shared
 * native literal pools. Only the final122 bytes are exported and counted.
 */
struct save {
    char pad[44];
    long l2c, l30, l34;
    short w38, w3a, w3c, w3e, w40;
};
extern struct save g_6065650;
extern long g_6065640, g_60657a0, g_6060008;
extern unsigned short g_6065642, g_60657a2;
extern short g_606574c;
extern long f_2beca(unsigned long);
extern void f_3068c();
extern void f_30844();
#define E g_6065650
#pragma inline(diff)
static short diff(void *b, void *p) { return (long)p - (long)b; }
#define OFS(p) diff(&E, p)

unsigned short f_21170(void) {
    long sum;
    E.l2c = g_6065642;
    sum = E.l2c;
    sum += E.l30;
    sum += E.l34;
    sum += E.w38;
    sum += E.w3a;
    sum += E.w3c;
    sum += E.w3e;
    sum++;
    return sum;
}
void f_211a2(void) {
    g_6065640 = 0;
    E.l2c = g_6065640;
    E.l30 = E.l34 = 0;
    E.w38 = E.w3a = E.w3c = 0;
    g_60657a0 += f_2beca(0x4a8) + g_6060008;
    E.w3e = g_60657a0;
}
void f_211e8(long v) {
    if ((unsigned short)E.w38 < 0xefff) {
        E.w38++;
        E.l34 += v;
    }
}
void f_21206(void) {
    unsigned short sum;
    E.l2c = g_6065640;
    { short offset = OFS(&E.l2c); f_30844(offset, (unsigned char *)&E.l2c, 4); }
    { short offset = OFS(&E.l30); f_30844(offset, (unsigned char *)&E.l30, 4); }
    { short offset = OFS(&E.l34); f_30844(offset, (unsigned char *)&E.l34, 4); }
    { short offset = OFS(&E.w38); f_30844(offset, (unsigned char *)&E.w38, 2); }
    { short offset = OFS(&E.w3a); f_30844(offset, (unsigned char *)&E.w3a, 2); }
    { short offset = OFS(&E.w3c); f_30844(offset, (unsigned char *)&E.w3c, 2); }
    E.w3e = g_60657a2;
    { short offset = OFS(&E.w3e); f_30844(offset, (unsigned char *)&E.w3e, 2); }
    sum = f_21170();
    E.w40 = sum;
    { short offset = OFS(&E.w40); f_30844(offset, (unsigned char *)&sum, 2); }
}
int f_212f8(void) {
    unsigned short sum;
    { short offset = OFS(&E.l2c); f_3068c(offset, (unsigned char *)&E.l2c, 4); }
    g_6065640 = E.l2c;
    { short offset = OFS(&E.l30); f_3068c(offset, (unsigned char *)&E.l30, 4); }
    { short offset = OFS(&E.l34); f_3068c(offset, (unsigned char *)&E.l34, 4); }
    { short offset = OFS(&E.w38); f_3068c(offset, (unsigned char *)&E.w38, 2); }
    { short offset = OFS(&E.w3a); f_3068c(offset, (unsigned char *)&E.w3a, 2); }
    { short offset = OFS(&E.w3c); f_3068c(offset, (unsigned char *)&E.w3c, 2); }
    { short offset = OFS(&E.w3e); f_3068c(offset, (unsigned char *)&E.w3e, 2); }
    g_60657a0 = (unsigned short)E.w3e;
    { short offset = OFS(&E.w40); f_3068c(offset, (unsigned char *)&sum, 2); }
    E.w40 = sum;
    if (sum != f_21170()) {
        f_211a2();
        f_21206();
        return 0;
    }
    return 1;
}
int f_213da(short v) {
    short x;
    short offset=OFS(&g_606574c);
    f_3068c(offset, (unsigned char *)&x, 2);
    g_606574c = x;
    return v == x ? 1 : 0;
}
void f_2140e(short v) {
    f_30844(OFS(&g_606574c), (unsigned char *)&v, 2);
    g_606574c = v;
}
