/* rom: 0x27e70 len: 0x4c4 func: f_27e70 flags: -macsave=1 -optimize=1 -speed */
/* NOT FINAL (unit 0x27e70-0x28334; same board struct as wip/board_27b50.c).
 * f_27e70 and f_27e80 are 100%.  f_27e90 (~22%) and f_2808a (~5%) are
 * first drafts: both call the 16.16 fixmul (dmuls.l/xtrct), so this only
 * compiles through agent_b/shcc_asm.sh with ROF2ELF=agent_b/ia/rof2elf_expr.py
 * (sym+3 pool words), e.g. agent_b/fsa.py funcscore.
 * Known ROM facts for f_27e90: the 224-step loop is unrolled by 2; i (sp8)
 * and ang (sp4, an 8-bit angle read back with extu.b) live on the stack; the
 * fixmul results c (sp16) and s (sp20) are written and read (high halves,
 * mov.w) through pointers kept in r9/r8, while k*4 is spilled to sp0.  This
 * draft keeps k*4 in r8 and spills the s pointer instead.  f_2808a mirrors
 * rows 111..0 and 112..223 from the middle out and ends by updating the
 * values at d_3b9f0/d_3b9f4/d_3b9f8. */
struct board {
	char pad0[6];
	short sel;                 /* 0x06 */
	char pad8[2];
	short w0a;                 /* 0x0a */
	unsigned short w0c;        /* 0x0c */
	unsigned short w0e;        /* 0x0e */
	char pad10[0x18 - 0x10];
	short set[16];             /* 0x18 */
	short w38;                 /* 0x38 */
	char pad3a[0x4c - 0x3a];
	long w4c;                  /* 0x4c */
	long a[224];               /* 0x050 */
	unsigned long x[224];      /* 0x3d0 */
};
struct pal {
	char pad0[0xc];
	unsigned short w0c[3];     /* 0x0c */
	char pad12[0x18 - 0x12];
	long l18[2];               /* 0x18 */
	long l20[13];              /* 0x20 */
	short n;                   /* 0x54 */
};
extern struct pal g_60ad228[];
extern short g_60b13a6, g_60b13aa;
extern long d_3b9e4, d_3b9e8, d_3b9ec, d_3b9f0, d_3b9f4, d_3b9f8;
extern unsigned char d_3b8e4[];
extern long f_2be48(unsigned char);
extern long f_2be88(unsigned char);

#pragma inline_asm(fixmul)
static long fixmul(long a, long b)
{
	DMULS.L R4,R5
	STS MACH,R4
	STS MACL,R0
	XTRCT R4,R0
}

void f_27e70(int on)
{
	g_60b13a6 = on ? 1 : 0;
}

void f_27e80(int on)
{
	g_60b13aa = on ? 1 : 0;
}

void f_27e90(struct board *bd)
{
	struct pal *q;
	short k;
	unsigned long attr;
	long c, s;
	long *pc, *ps;
	unsigned char ang;
	int i;

	q = (struct pal *)((char *)g_60ad228 + (short)(bd->set[bd->sel] * 104));
	k = q->n;
	attr = (bd->w0a << 24) | (bd->w0c << 15) | (bd->w0e << 8) | q->w0c[k];
	pc = &c;
	ps = &s;
	ang = d_3b9e4;
	for (i = 0; i < 224; i++) {
		*pc = fixmul(f_2be88(ang), 0xf0000);
		*ps = fixmul(f_2be48(ang), 0x1f0000);
		bd->x[i] = attr;
		bd->a[i] = ((q->l18[k] - *(short *)pc) & 0x3ff) << 16 |
		           ((q->l20[k] - *(short *)ps - 64) & 0x3ff);
		ang += 3;
	}
	d_3b9e4 += 3;
	d_3b9e8 += d_3b9ec;
	if (d_3b9e8 > 63)
		d_3b9ec = -1;
	else if (d_3b9e8 < 1)
		d_3b9ec = 1;
	d_3b9e4 = *((unsigned char *)&d_3b9e4 + 3);
}

void f_2808a(struct board *bd)
{
	struct pal *q;
	short k, w;
	unsigned long attr;
	long c, s;
	int i, j, m, h, ang, sh;

	w = bd->w38;
	q = (struct pal *)((char *)g_60ad228 + (short)(bd->set[bd->sel] * 104));
	k = q->n;
	attr = (bd->w0a << 24) | (bd->w0c << 15) | (bd->w0e << 8) | q->w0c[k];
	ang = (unsigned char)d_3b9f0;
	j = 112;
	m = 111;
	for (i = 0; i < 112; i++) {
		c = fixmul(f_2be48((i >> 2) + 64), 0x400000);
		h = 64 - *(short *)&c;
		if (h < 0)
			h = 0;
		bd->x[m] = bd->x[j] = (h << 16) | attr;
		c = fixmul(f_2be48(ang + i), 0x100000);
		s = fixmul(f_2be48(ang - i), 0x100000);
		bd->a[j] = (q->l20[k] - d_3b8e4[h] - *(short *)&c) & 0x3ff;
		bd->a[m] = (q->l20[k] - d_3b8e4[h] - *(short *)&s) & 0x3ff;
		sh = bd->w4c + i;
		if (sh > 0xdf)
			sh -= 224;
		bd->a[j] |= ((q->l18[k] - sh + j) & 0x3ff) << 16;
		bd->a[m] |= ((q->l18[k] - sh + m) & 0x3ff) << 16;
		j++;
		m--;
	}
	bd->w4c -= w;
	if (bd->w4c >= 224)
		bd->w4c -= 224;
	if (bd->w4c < 0)
		bd->w4c += 224;
	d_3b9f0 += d_3b9f8;
	if (d_3b9f0 > 0xff) {
		if (d_3b9f8 == 4)
			d_3b9f4 = -1;
		else if (d_3b9f8 == 1)
			d_3b9f4 = 1;
		d_3b9f8 += d_3b9f4;
		d_3b9f0 = *((unsigned char *)&d_3b9f0 + 3);
	}
}
