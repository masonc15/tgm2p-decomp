/* rom: 0x21454 len: 0x27c func: f_21454 flags: -macsave=1 -optimize=1 -speed */
/* Medal display, one strip of six icons per player (AC ST SK RE RO CO).
 * f_21454 resets the strip and places the icons at (x, y) plus the offsets
 * in d_3b574. f_214ec runs each frame: an icon waits in state 0 until the
 * player's grade for it (f->medal[i]) is nonzero, or is put in state 1 by
 * the award code in medal.c (its "changed[0] = 1" is this state byte); state
 * 1 plays a sound and starts the flash, state 2 draws the zooming icon for 64
 * frames and then latches the new grade, state 3 draws it still. f_2164e
 * loads the three icon graphics. Matches from a fresh file.
 * SHC notes: the loop only matches with m++ in the for header (with m++ at
 * the end of the body SHC strength-reduces d_3b55c[i] and f->medal[i]); the
 * switch needs its last case to fall off the end (or a "default: break;");
 * and the zoom argument has to be assigned inside the call, which SHC
 * evaluates right to left, so arg 7 is computed first and arg 6 reads it. */
struct medal {
	unsigned char state;       /* 0x0 */
	unsigned char pad1;
	unsigned char idx;         /* 0x2 */
	signed char alpha;         /* 0x3 */
	unsigned char grade;       /* 0x4 */
	char pad5;
	short timer;               /* 0x6 */
	short y;                   /* 0x8 */
	short x;                   /* 0xa */
};
struct field {
	char pad0[0x30e];
	unsigned char id;          /* 0x30e */
	char pad1[0x33a - 0x30f];
	unsigned char medal[6];    /* 0x33a */
};
struct pos {
	short x;
	short y;
};
struct icon {
	long a;
	long b;
	signed char c;
	char pad9;
	signed char d;
	char padb;
};
struct frame;
extern struct medal g_607cd48[];
extern struct pos d_3b574[];
extern struct frame *d_3b55c[];
extern struct icon d_3b58c[];
extern void f_2e6fc(int);
extern void f_2a576(int, long, long, int, int, int, int);
extern void f_1159c(struct frame *, int, int, int, int);
extern void f_11680(struct frame *, short, short, short, short, short, short, short);

#define MEDALS(f) ((struct medal *)((char *)g_607cd48 + (unsigned char)((f)->id * 72)))

void f_21454(struct field *f, int y, int x)
{
	struct medal *m;
	short i;

	m = MEDALS(f);
	for (i = 0; i < 6; i++) {
		*(long *)m = 0;
		m->idx = i;
		m->alpha = 0;
		m->grade = 0;
		m->x = d_3b574[i].x + x;
		m->y = d_3b574[i].y + y;
		m++;
	}
}

void f_214ec(struct field *f)
{
	struct medal *m;
	struct icon *e;
	short i;
	signed char a;
	short sh;
	short z;

	m = MEDALS(f);
	for (i = 0; i < 6; i++, m++) {
		e = (struct icon *)((char *)d_3b58c + (signed char)(m->grade * 12));
		switch (m->state) {
		case 0:
			if (f->medal[i] == 0)
				break;
			m->state++;
		case 1:
			m->state++;
			m->timer = 64;
			m->alpha = 64;
			f_2e6fc(36);
			f_2a576((unsigned char)e->d, e->b, e->a, 1, 64, 1, 63);
		case 2:
			sh = (a = m->alpha) >> 3;
			f_11680(d_3b55c[i], m->y - sh, m->x - sh, e->d, 40, z, z = a + 63, 0);
			if (m->alpha) {
				m->alpha -= 3;
				if (m->alpha < 0)
					m->alpha = 0;
			}
			if (--m->timer == 0) {
				m->state++;
				m->grade = f->medal[i];
			}
			break;
		case 3:
			f_1159c(d_3b55c[i], m->y, m->x, e->c, 40);
		}
	}
}

void f_2164e(void)
{
	f_2a576(0xd3, 0x63dd0, 0x68350, 2, 66, 1, 32);
	f_2a576(0xd5, 0x68310, 0x683d0, 2, 66, 1, 32);
	f_2a576(0xd4, 0x68290, 0x68390, 2, 66, 1, 32);
}
