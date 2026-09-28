/* rom: 0x19014 len: 0x128 func: f_19014 flags: -macsave=1 -optimize=1 -speed */
struct field {
	char pad0[0xe8];
	short e8;                  /* 0x0e8 */
	char pad1[0x30c - 0xea];
	unsigned short mode;       /* 0x30c */
	char pad2[0x3b4 - 0x30e];
};
struct player {
	char b0;
	char b1;
	short w[3];
	char c[4];
	char d[0x800];
	struct field *field;       /* 0x80c */
	struct player *prev;       /* 0x810 */
	struct player *next;       /* 0x814 */
};
extern struct field g_607c5e0[2];
extern struct player g_6079544[6];
extern short g_607c5d4;
extern struct player *g_607c5d8, *g_607c5dc;
extern void (*d_3b460[])(struct field *, short);

void f_19014(void)
{
	struct field *f;           /* never set: SHC reads f->mode as @(0x30c,r15) */
	struct field *g;
	short i, v;

	if (f->mode & 8) {
		for (i = 0; i < 2; i++) {
			g = &g_607c5e0[i];
			v = g->e8;
			if (v >= 1 && v <= 19)
				d_3b460[v - 1](g, v);
		}
	}
}

void f_19074(void)
{
	short i, j;

	g_607c5d4 = 6;
	g_607c5d8 = 0;
	g_607c5dc = 0;
	for (i = 0; i < 6; i++) {
		g_6079544[i].b0 = 0;
		g_6079544[i].b1 = 0;
		for (j = 0; j < 3; j++)
			g_6079544[i].w[j] = 0;
		for (j = 0; j < 4; j++)
			g_6079544[i].c[j] = 0;
		for (j = 0; j < 0x800; j++)
			g_6079544[i].d[j] = 0;
	}
}
