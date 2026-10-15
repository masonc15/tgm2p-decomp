/* rom: 0xbe04 len: 0x24 func: f_be04 flags: -macsave=1 -optimize=1 -speed */
/* Only f_be04 is retained; the following real menu routines provide
 * original shared-pool compiler context and are not exported. */
/* Menu state and actual text/table addresses reconstructed from ROM. */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef unsigned int u32;
typedef int s32;
extern unsigned short d_3a8e8[6];
extern long g_6064750;
extern unsigned char g_6064754, g_6064755, g_6079304, g_6079305;
extern short g_60792ec;
extern short g_6060070[], g_6060074[];
extern char g_6032960[],g_6032b64[],g_6032b68[],g_6032cb4[],g_6032cd0[],g_6032ce0[];
extern char g_6032d00[],g_6032d10[],g_6032d18[],g_6032d20[],g_6032d30[],g_6032d40[];
extern char g_6032d48[],g_6032d50[],g_6032d58[],g_6032d5c[],g_6032d70[],g_6032d84[];
int f_be04(short);
void f_be28(void);
void f_c21a(void);
int f_2b4e8(void);
void f_9ede(void);
void f_29cc6(short,short,long,long);
void f_2f2cc(void);
void f_2f3dc(void);
void f_2ee38(void);
void f_2edcc(char,char);
char f_2ee0c(char);
void f_2ee74(void);
void f_2e5aa(char);
void f_2e6fc(short);
void f_2ed3e(void);
void f_2ed06(void);
void f_e3e0(short,char *,char);
void f_e412(int,int,char *,char);
void f_e520(short,short,char *,int);
void f_e63a(int,short,int,short,long,unsigned char,char);

#define CURSOR(off) (*(short *)((char *)g_6060070 + (g_60792ec << 6) + (off)))

/* Verified retained function [0xbe04,0xbe28): membership in six words. */
int f_be04(short n)
{
    short i = 0;
    register unsigned short *p = d_3a8e8;

    do {
        if (*p == (unsigned short)n)
            return 1;
        i++;
        p++;
    } while (i < 6);
    return 0;
}

/* CONTEXT ONLY: sound/options menu, including the real shared literal. */
void f_be28(void) {
    s16 sp0;
    s16 sp4;
    s16 sp8;
    s32 spC;
    s16 sp10;
    s16 sp14;
    s16 temp_r0;
    s16 temp_r0_2;
    s16 temp_r0_3;
    s16 temp_r5;
    s16 var_r10;
    s16 var_r11;
    s16 var_r12;
    s16 var_r4;
    u16 var_r8;
    s8 *var_r5;
    s8 var_r2;
    s8 var_r3;
    s8 var_r7;
    s8 var_r7_2;
    s8 var_r7_3;

    register short repeat_mask = 0x7fff;
    register long *busy = &g_6064750;
    var_r12 = 0;
    var_r11 = 0;
    var_r10 = 0;
    spC = 1;
    sp4 = 0;
    sp8 = 0;
    sp0 = 0;
    var_r8 = 0;
    if (*busy != 0) {
        do {
            f_2b4e8();
        } while (*busy != 0);
    }
    f_29cc6(0, 4, 0, -0x7FFA);
    f_2f2cc();
    f_2f3dc();
    CURSOR(0) = 0x32;
    CURSOR(4) = 0x3C;
loop_3:
    f_2b4e8();
    f_9ede();
    if (var_r8 != (u8) (s8) g_6064754) {
        var_r8 = (s16) (u8) (s8) g_6064754;
        sp4 = 0;
        goto block_6;
    }
    temp_r0 = sp4 + 1;
    sp4 = temp_r0;
    temp_r0_2 = temp_r0 & repeat_mask;
    sp4 = temp_r0_2;
    if (temp_r0_2 > 0x1E) {
block_6:
        sp10 = var_r8;
    } else {
        sp10 = 0;
    }
    if (sp8 != (u8) (s8) g_6064755) {
        sp8 = (s16) (u8) (s8) g_6064755;
        sp0 = 0;
        sp14 = (s16) (u8) (s8) g_6064755;
    } else {
        sp0 += 1;
        temp_r0_3 = sp0 & repeat_mask;
        sp0 = temp_r0_3;
        if (temp_r0_3 > 0x1E) {
            sp14 = sp8;
        } else {
            sp14 = 0;
        }
    }
    if ((((u8) (s8) g_6079304 == 0x40) | ((u8) (s8) g_6079305 == 0x40)) != 0) {
        var_r11 += 1;
        if (var_r11 > 2) {
            var_r11 = 0;
        }
    }
    if ((((u8) (s8) g_6079304 == 0x80) | ((u8) (s8) g_6079305 == 0x80)) != 0) {
        var_r11 -= 1;
        if (var_r11 < 0) {
            var_r11 = 2;
        }
    }
    switch (var_r11) {                              /* irregular */
    case 2:
        if ((((u8) (s8) g_6079304 == 8) | ((u8) (s8) g_6079305 == 8)) != 0) {
            spC = 0;
        }
        break;
    case 0:
        if (((sp10 == 0x20) | (sp14 == 0x20)) != 0) {
            var_r10 += 1;
            if (var_r10 > 7) {
                var_r10 = 0;
            }
        }
        if (((sp10 == 0x10) | (sp14 == 0x10)) != 0) {
            var_r10 -= 1;
            if (var_r10 < 0) {
                var_r10 = 7;
            }
        }
        if ((((u8) (s8) g_6079304 == 8) | ((u8) (s8) g_6079305 == 8)) != 0) {
            f_2e5aa((s8) var_r10);
        }
        if ((((u8) (s8) g_6079304 == 4) | ((u8) (s8) g_6079305 == 4)) != 0) {
            f_2ed3e();
        }
        break;
    case 1:
        if (((sp10 == 0x20) | (sp14 == 0x20)) != 0) {
            var_r12 += 1;
            if (var_r12 > 0x28) {
                var_r12 = 0;
            }
        }
        if (((sp10 == 0x10) | (sp14 == 0x10)) != 0) {
            var_r12 -= 1;
            if (var_r12 < 0) {
                var_r12 = 0x28;
            }
        }
        if (((((u8) (s8) g_6079304 == 8) | ((u8) (s8) g_6079305 == 8)) != 0) && (f_be04((u16) var_r12) == 0)) {
            f_2e6fc(var_r12);
        }
        if ((((u8) (s8) g_6079304 == 4) | ((u8) (s8) g_6079305 == 4)) != 0) {
            f_2ed06();
        }
        break;
    }
    f_e3e0(0xA, g_6032d00, 0);
    if (var_r11 == 0) {
        var_r7 = 0;
    } else {
        var_r7 = 1;
    }
    f_e412(0x50, 0x32, g_6032d10, var_r7);
    if (var_r11 == 1) {
        var_r7_2 = 0;
    } else {
        var_r7_2 = 1;
    }
    f_e412(0x50, 0x40, g_6032d18, var_r7_2);
    if (var_r11 == 2) {
        var_r7_3 = 0;
    } else {
        var_r7_3 = 1;
    }
    f_e412(0x50, 0x4E, g_6032cb4, var_r7_3);
    temp_r5 = (var_r11 * 0xE) + 0x32;
    CURSOR(0) = temp_r5;
    f_e520(0x3C, temp_r5, g_6032960, 0);
    if (var_r11 == 0) {
        var_r2 = 0;
    } else {
        var_r2 = 1;
    }
    f_e63a((s32) var_r10, 0x32, 0xC8, 2, 0, 1U, var_r2);
    if (var_r11 == 1) {
        var_r3 = 0;
    } else {
        var_r3 = 1;
    }
    f_e63a((s32) var_r12, 0x40, 0xC8, 3, 0, 1U, var_r3);
    if (var_r11 == 2) {
        var_r5 = g_6032cd0;
        var_r4 = 0xC8;
    } else {
        f_e3e0(0xD0, g_6032d20, 0);
        f_e3e0(0xC2, g_6032d30, 0);
        var_r5 = g_6032ce0;
        var_r4 = 0xB4;
    }
    f_e3e0(var_r4, var_r5, 0);
    if (spC != 0) {
        goto loop_3;
    }
    f_2f2cc();
    f_2f3dc();
    f_2b4e8();
}

/* CONTEXT ONLY: extended options menu. */
void f_c21a(void) {
    s16 sp0;
    s32 sp4;
    s16 sp8;
    s16 spC;
    s16 sp10;
    s16 sp14;
    s16 sp18;
    s16 sp1C;
    s32 sp28;
    s16 temp_r0;
    s16 temp_r0_10;
    s16 temp_r0_11;
    s16 temp_r0_2;
    s16 temp_r0_3;
    s16 temp_r0_4;
    s16 temp_r0_5;
    s16 temp_r0_6;
    s16 temp_r0_7;
    s16 temp_r0_8;
    s16 temp_r0_9;
    s16 temp_r14;
    s16 temp_r14_2;
    s16 temp_r14_3;
    s16 var_r14;
    s16 var_r14_2;
    s16 var_r4_4;
    s16 var_r5;
    s16 var_r5_2;
    s16 var_r8;
    s32 var_r4;
    s32 var_r4_2;
    s32 var_r4_3;
    s8 *var_r5_3;
    s8 *var_r6;
    s8 *var_r6_2;
    s8 *var_r6_3;
    s8 *var_r6_4;
    s8 *var_r6_5;
    s8 *var_r6_6;
    s8 *var_r6_7;
    s8 *var_r6_8;
    s8 var_r2;
    s8 var_r3;
    s8 var_r3_2;
    s8 var_r7;
    s8 var_r7_2;
    s8 var_r7_3;
    s8 var_r7_4;
    s8 var_r7_5;
    s8 var_r7_6;
    short repeat0;
    short held0;

    sp28 = 1;
    sp8 = 0;
    sp14 = 0;
    sp10 = 0;
    held0 = 0;
    repeat0 = 0;
    sp18 = 0;
    sp1C = 0;
    var_r8 = 0;
    if (g_6064750 != 0) {
        do {
            f_2b4e8();
        } while (g_6064750 != 0);
    }
    f_29cc6(0, 4, 0, -0x7FFA);
    CURSOR(0) = 0x32;
    CURSOR(4) = 0x3C;
    f_2f2cc();
    f_2f3dc();
    f_2ee38();
loop_3:
    f_2b4e8();
    f_9ede();
    if (held0 != (u8) (s8) g_6064754) {
        held0 = (s16) (u8) (s8) g_6064754;
        repeat0 = 0;
        goto block_6;
    }
    repeat0 = (s16) (repeat0 + 1);
    temp_r0 = repeat0 & 0x7FFF;
    repeat0 = temp_r0;
    if (temp_r0 > 0x1E) {
block_6:
        var_r14 = held0;
    } else {
        var_r14 = 0;
    }
    if (sp18 != (u8) (s8) g_6064755) {
        sp18 = (s16) (u8) (s8) g_6064755;
        sp1C = 0;
        goto block_11;
    }
    temp_r0_2 = sp1C + 1;
    sp1C = temp_r0_2;
    temp_r0_3 = temp_r0_2 & 0x7FFF;
    sp1C = temp_r0_3;
    if (temp_r0_3 > 0x1E) {
block_11:
        sp0 = sp18;
    } else {
        sp0 = 0;
    }
    if ((((u8) (s8) g_6079304 == 0x40) | ((u8) (s8) g_6079305 == 0x40)) != 0) {
        if (sp8 == 2) {
            if (var_r8 < 8) {
                var_r8 += 8;
            } else {
                temp_r0_4 = sp8 + 1;
                sp8 = temp_r0_4;
                if (temp_r0_4 > 5) {
                    sp8 = 0;
                }
            }
        } else {
            temp_r0_5 = sp8 + 1;
            sp8 = temp_r0_5;
            if (temp_r0_5 > 5) {
                sp8 = 0;
            }
        }
    }
    if ((((u8) (s8) g_6079304 == 0x80) | ((u8) (s8) g_6079305 == 0x80)) != 0) {
        if (sp8 == 2) {
            if (var_r8 > 7) {
                var_r8 -= 8;
            } else {
                temp_r0_6 = sp8 - 1;
                sp8 = temp_r0_6;
                if (temp_r0_6 < 0) {
                    sp8 = 5;
                }
            }
        } else {
            temp_r0_7 = sp8 - 1;
            sp8 = temp_r0_7;
            if (temp_r0_7 < 0) {
                sp8 = 5;
            }
        }
    }
    sp4 = (s32) sp8;
    switch (sp8) {                                  /* switch 1; irregular */
    case 2:                                         /* switch 1 */
        if ((((u8) (s8) g_6079304 == 0x20) | ((u8) (s8) g_6079305 == 0x20)) != 0) {
            var_r8 += 1;
            if (var_r8 > 0xF) {
                var_r8 = 0;
            }
        }
        if ((((u8) (s8) g_6079304 == 0x10) | ((u8) (s8) g_6079305 == 0x10)) != 0) {
            var_r8 -= 1;
            if (var_r8 < 0) {
                var_r8 = 0xF;
            }
        }
        if ((((u8) (s8) g_6079304 == 8) | ((u8) (s8) g_6079305 == 8)) != 0) {
            f_2edcc((s8) var_r8, 1 & (f_2ee0c((s8) var_r8) + 1));
        }
        CURSOR(0) = 0x4E;
        var_r5 = 0x4E;
block_81:
        f_e520(0x3C, var_r5, g_6032960, 0);
        break;
    case 3:                                         /* switch 1 */
        if ((((u8) (s8) g_6079304 == 8) | ((u8) (s8) g_6079305 == 8)) != 0) {
            f_2ee38();
        }
        CURSOR(0) = 0x78;
        var_r5 = 0x78;
        goto block_81;
    case 4:                                         /* switch 1 */
        if ((((u8) (s8) g_6079304 == 8) | ((u8) (s8) g_6079305 == 8)) != 0) {
            f_2ee74();
        }
        CURSOR(0) = 0x86;
        var_r5 = 0x86;
        goto block_81;
    case 5:                                         /* switch 1 */
        if ((((u8) (s8) g_6079304 == 8) | ((u8) (s8) g_6079305 == 8)) != 0) {
            sp28 = 0;
        }
        var_r5 = 0x94;
        CURSOR(0) = 0x94;
        goto block_81;
    case 0:                                         /* switch 1 */
        if (((var_r14 == 0x20) | (sp0 == 0x20)) != 0) {
            temp_r0_8 = sp14 + 1;
            sp14 = temp_r0_8;
            if (temp_r0_8 > 7) {
                sp14 = 0;
            }
        }
        if (((var_r14 == 0x10) | (sp0 == 0x10)) != 0) {
            temp_r0_9 = sp14 - 1;
            sp14 = temp_r0_9;
            if (temp_r0_9 < 0) {
                sp14 = 7;
            }
        }
        if ((((u8) (s8) g_6079304 == 8) | ((u8) (s8) g_6079305 == 8)) != 0) {
            f_2e5aa((s8) sp14);
        }
        if ((((u8) (s8) g_6079304 == 4) | ((u8) (s8) g_6079305 == 4)) != 0) {
            f_2ed3e();
        }
        CURSOR(0) = 0x32;
        var_r5 = 0x32;
        goto block_81;
    case 1:                                         /* switch 1 */
        if (((var_r14 == 0x20) | (sp0 == 0x20)) != 0) {
            temp_r0_10 = sp10 + 1;
            sp10 = temp_r0_10;
            if (temp_r0_10 > 0x28) {
                sp10 = 0;
            }
        }
        if (((var_r14 == 0x10) | (sp0 == 0x10)) != 0) {
            temp_r0_11 = sp10 - 1;
            sp10 = temp_r0_11;
            if (temp_r0_11 < 0) {
                sp10 = 0x28;
            }
        }
        if (((((u8) (s8) g_6079304 == 8) | ((u8) (s8) g_6079305 == 8)) != 0) && (f_be04((u16) sp10) == 0)) {
            f_2e6fc(sp10);
        }
        if ((((u8) (s8) g_6079304 == 4) | ((u8) (s8) g_6079305 == 4)) != 0) {
            f_2ed06();
        }
        CURSOR(0) = 0x40;
        var_r5 = 0x40;
        goto block_81;
    }
    f_e3e0(0xA, g_6032d00, 0);
    if (sp4 == 0) {
        var_r7 = 0;
    } else {
        var_r7 = 1;
    }
    f_e412(0x50, 0x32, g_6032d10, var_r7);
    if (sp4 == 1) {
        var_r7_2 = 0;
    } else {
        var_r7_2 = 1;
    }
    f_e412(0x50, 0x40, g_6032d18, var_r7_2);
    if (sp4 == 2) {
        var_r7_3 = 0;
    } else {
        var_r7_3 = 1;
    }
    f_e412(0x50, 0x4E, g_6032d40, var_r7_3);
    if (sp4 == 3) {
        var_r7_4 = 0;
    } else {
        var_r7_4 = 1;
    }
    f_e412(0x50, 0x78, g_6032d48, var_r7_4);
    if (sp4 == 4) {
        var_r7_5 = 0;
    } else {
        var_r7_5 = 1;
    }
    f_e412(0x50, 0x86, g_6032d50, var_r7_5);
    if (sp4 == 5) {
        var_r7_6 = 0;
    } else {
        var_r7_6 = 1;
    }
    f_e412(0x50, 0x94, g_6032cb4, var_r7_6);
    var_r14_2 = 0;
loop_101:
    if (var_r14_2 < 8) {
        if (f_2ee0c((s8) var_r14_2) == 0) {
            var_r6 = g_6032b64;
            var_r4 = var_r14_2 * 0x11;
        } else {
            var_r6 = g_6032b68;
            var_r4 = var_r14_2 * 0x11;
        }
        f_e520(var_r4 + 0x72, 0x5C, var_r6, 0);
    } else {
        if (f_2ee0c((s8) var_r14_2) == 0) {
            var_r6_2 = g_6032b64;
            var_r4_2 = (var_r14_2 - 8) * 0x11;
        } else {
            var_r6_2 = g_6032b68;
            var_r4_2 = (var_r14_2 - 8) * 0x11;
        }
        f_e520(var_r4_2 + 0x72, 0x6A, var_r6_2, 0);
    }
    temp_r14 = var_r14_2 + 1;
    spC = (temp_r14 * 0x11) + 0x72;
    sp0 = ((temp_r14 - 8) * 0x11) + 0x72;
    if (temp_r14 < 8) {
        if (f_2ee0c((s8) temp_r14) == 0) {
            var_r6_3 = g_6032b64;
        } else {
            var_r6_3 = g_6032b68;
        }
        f_e520(spC, 0x5C, var_r6_3, 0);
    } else {
        if (f_2ee0c((s8) temp_r14) == 0) {
            var_r6_4 = g_6032b64;
        } else {
            var_r6_4 = g_6032b68;
        }
        f_e520(sp0, 0x6A, var_r6_4, 0);
    }
    temp_r14_2 = temp_r14 + 1;
    sp0 = (temp_r14_2 * 0x11) + 0x72;
    spC = ((temp_r14_2 - 8) * 0x11) + 0x72;
    if (temp_r14_2 < 8) {
        if (f_2ee0c((s8) temp_r14_2) == 0) {
            var_r6_5 = g_6032b64;
        } else {
            var_r6_5 = g_6032b68;
        }
        f_e520(sp0, 0x5C, var_r6_5, 0);
    } else {
        if (f_2ee0c((s8) temp_r14_2) == 0) {
            var_r6_6 = g_6032b64;
        } else {
            var_r6_6 = g_6032b68;
        }
        f_e520(spC, 0x6A, var_r6_6, 0);
    }
    temp_r14_3 = temp_r14_2 + 1;
    if (temp_r14_3 < 8) {
        sp0 = (temp_r14_3 * 0x11) + 0x72;
        if (f_2ee0c((s8) temp_r14_3) == 0) {
            var_r6_7 = g_6032b64;
        } else {
            var_r6_7 = g_6032b68;
        }
        f_e520(sp0, 0x5C, var_r6_7, 0);
    } else {
        sp0 = ((temp_r14_3 - 8) * 0x11) + 0x72;
        if (f_2ee0c((s8) temp_r14_3) == 0) {
            var_r6_8 = g_6032b64;
        } else {
            var_r6_8 = g_6032b68;
        }
        f_e520(sp0, 0x6A, var_r6_8, 0);
    }
    var_r14_2 = temp_r14_3 + 1;
    if (var_r14_2 < 0x10) {
        goto loop_101;
    }
    if (sp4 == 2) {
        if (var_r8 < 8) {
            var_r4_3 = var_r8 * 0x11;
            var_r5_2 = 0x5C;
        } else {
            var_r5_2 = 0x6A;
            var_r4_3 = (var_r8 - 8) * 0x11;
        }
        f_e520(var_r4_3 + 0x72, var_r5_2, g_6032d58, 0);
    }
    if (sp4 == 0) {
        var_r3 = 0;
    } else {
        var_r3 = 1;
    }
    f_e63a((s32) sp14, 0x32, 0xC8, 2, 0, 1U, var_r3);
    if (sp4 == 1) {
        var_r2 = 0;
    } else {
        var_r2 = 1;
    }
    f_e63a((s32) sp10, 0x40, 0xC8, 3, 0, 1U, var_r2);
    if (sp4 == 2) {
        var_r3_2 = 0;
    } else {
        var_r3_2 = 1;
    }
    f_e63a((s32) var_r8, 0x4E, 0xC8, 2, 0, 1U, var_r3_2);
    switch (sp4) {                                  /* switch 2; irregular */
    case 2:                                         /* switch 2 */
        f_e3e0(0xCC, g_6032d5c, 0);
        var_r5_3 = g_6032ce0;
        var_r4_4 = 0xBE;
        break;
    case 3:                                         /* switch 2 */
        var_r5_3 = g_6032d70;
        var_r4_4 = 0xC8;
        break;
    case 4:                                         /* switch 2 */
        var_r5_3 = g_6032d84;
        var_r4_4 = 0xC8;
        break;
    case 5:                                         /* switch 2 */
        var_r5_3 = g_6032cd0;
        var_r4_4 = 0xC8;
        break;
    default:                                        /* switch 2 */
        f_e3e0(0xD0, g_6032d20, 0);
        f_e3e0(0xC2, g_6032d30, 0);
        var_r5_3 = g_6032ce0;
        var_r4_4 = 0xB4;
        break;
    }
    f_e3e0(var_r4_4, var_r5_3, 0);
    if (sp28 != 0) {
        goto loop_3;
    }
    f_2f2cc();
    f_2f3dc();
    f_2b4e8();
}
