/* rom: 0x278c0 len: 0x290 func: f_278c0 flags: -macsave=1 -optimize=1 -speed */
/* NOT FINAL: f_278c0 clears four 224-entry arrays of the current board.
 * Structure matches (register-normalized ~93%): the loop steps i by 8 with
 * eight stores per array written out, and SHC unrolls it by 4. `register` on
 * the pointers and on i is what moves them to r4-r7 and puts the zero in r13;
 * pa and pc still come out in r5/r6 where the ROM has r6/r5, and the first
 * global store is scheduled earlier than in the ROM. */
struct game {
	char pad[0x52];
	short player;              /* 0x52 */
};
struct board {
	char pad0[0x50];
	long a[224];               /* 0x050 */
	long x[224];               /* 0x3d0 */
	long b[224];               /* 0x750 */
	short c[224];              /* 0xad0 */
	short d[224];              /* 0xc90 */
};
extern struct game *g_606005c;
extern struct board g_60ad6a0[];
extern short g_60b13a6, g_60b13a8, g_60b13aa, g_60b13ac, g_60b13ae, g_60b13b0, g_60b13b2;

#define BOARD ((struct board *)((char *)g_60ad6a0 + (short)(g_606005c->player * 0xe50)))

void f_278c0(void)
{
	struct board *p;
	register long *pa, *pb;
	register short *pc, *pd;
	register unsigned int i;

	g_60b13ae = 0;
	p = BOARD;
	pa = p->a;
	pb = p->b;
	pc = p->c;
	pd = p->d;
	g_60b13b0 = 0;
	g_60b13b2 = 0;
	g_60b13a6 = 0;
	g_60b13a8 = 0;
	g_60b13aa = 0;
	g_60b13ac = 0;
	for (i = 0; i < 224; i += 8) {
		*pa++ = 0;
		*pa++ = 0;
		*pa++ = 0;
		*pa++ = 0;
		*pa++ = 0;
		*pa++ = 0;
		*pa++ = 0;
		*pa++ = 0;
		*pb++ = 0;
		*pb++ = 0;
		*pb++ = 0;
		*pb++ = 0;
		*pb++ = 0;
		*pb++ = 0;
		*pb++ = 0;
		*pb++ = 0;
		*pc++ = 0;
		*pc++ = 0;
		*pc++ = 0;
		*pc++ = 0;
		*pc++ = 0;
		*pc++ = 0;
		*pc++ = 0;
		*pc++ = 0;
		*pd++ = 0;
		*pd++ = 0;
		*pd++ = 0;
		*pd++ = 0;
		*pd++ = 0;
		*pd++ = 0;
		*pd++ = 0;
		*pd++ = 0;
	}
}
