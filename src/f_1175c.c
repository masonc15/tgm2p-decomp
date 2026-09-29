/* rom: 0x1175c len: 0x240 func: f_1175c flags: -macsave=1 -optimize=1 -speed */
/* Per-player 76-byte records at g_6079378, indexed by the field's player
 * byte (0x30e). f_117a8 takes a line count c (0 resets, 4 has extra
 * handling) and adds table-driven points: d_3ae78[lv][(c - 1) & 3] times
 * d_3af77[b343][c], rounded up to tens, times w322 / 250 + 1; 100 points
 * bump lv. f_11926 decays the points with the rate in d_3ae78[lv][4].
 * This looks like the TGM grade-point system (w322 the level, b343 a
 * combo count), but the names are unconfirmed. Bytes 0-1 are also read and
 * written as one short. */
struct sect {
	char lv;                /* 0x00 */
	unsigned char pts;      /* 0x01 */
	short best;             /* 0x02 */
	short w4[10];           /* 0x04 */
	short w24;              /* 0x18 */
	short pad26;
	long l28[10];           /* 0x1c */
	long l68;               /* 0x44 */
	char b72;               /* 0x48 */
	char pad73[3];
};
struct fld {
	char pad0[0x30c];
	unsigned short w30c;       /* 0x30c */
	unsigned char b30e;        /* 0x30e */
	char pad1[0x322 - 0x30f];
	unsigned short w322;       /* 0x322 */
	char pad2[0x328 - 0x324];
	unsigned short w328;       /* 0x328 */
	short w32a;                /* 0x32a */
	char pad3[0x339 - 0x32c];
	unsigned char b339;        /* 0x339 */
	char pad4[0x343 - 0x33a];
	char b343;                 /* 0x343 */
};
extern struct sect g_6079378[];
extern long g_6064880;
extern unsigned char d_3ae78[][5];
extern unsigned char d_3af77[][4];
extern void f_217a2(struct fld *);
extern void f_21862(struct fld *);

#define SECT(p) ((struct sect *)((char *)g_6079378 + (unsigned char)((p)->b30e * 76)))

void f_1175c(struct fld *p)
{
	struct sect *s = SECT(p);
	int i;

	*(short *)&s->lv = 0;
	s->best = 0;
	s->b72 = 0;
	for (i = 0; i < 10; i++) {
		s->w4[i] = 0;
		s->l28[i] = 0;
	}
	s->l68 = 0;
	s->w24 = 0;
}

void f_117a8(struct fld *p, char c)
{
	struct sect *s = SECT(p);
	int lv;
	short k;
	unsigned long r, v;

	if (c == 0) {
		p->b343 = 0;
		p->w32a = 0;
		return;
	}
	if (c == 4) {
		if ((g_6064880 & 1) && (p->w30c & 0x1002))
			f_217a2(p);
		k = p->b339 & 3;
		if (k < 3)
			k++;
		p->b339 |= k;
	}
	lv = s->lv;
	if (lv > 51)
		lv = 51;
	r = d_3ae78[lv][(c - 1) & 3];
	r *= d_3af77[p->b343][c];
	if (r % 10)
		r += 10;
	r /= 10;
	r *= p->w322 / 250 + 1;
	v = s->pts;
	v += r;
	if (v >= 100) {
		s->lv++;
		s->pts = 0;
		s->b72 = 0;
	} else {
		s->pts = v;
	}
	if (s->best < *(short *)&s->lv)
		s->best = *(short *)&s->lv;
	if (p->w32a < p->w328 && c > 1) {
		p->b343++;
		if ((g_6064880 & 1) && (p->w30c & 0x1002))
			f_21862(p);
	}
	p->w32a = p->w328;
}

void f_11926(struct fld *p)
{
	struct sect *s = SECT(p);
	int lv;

	if (p->w328 <= 1) {
		lv = s->lv;
		if (lv > 51)
			lv = 51;
		if (s->pts) {
			s->b72++;
			if (s->b72 >= d_3ae78[lv][4]) {
				s->b72 = 0;
				s->pts--;
			}
		}
	}
}
