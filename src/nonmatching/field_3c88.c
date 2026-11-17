/* rom: 0x3c88 len: 0x1a6 func: f_3c88 flags: -macsave=1 -optimize=1 -speed */
/* Non-matching: functionally equivalent C for [0x3c88, 0x3e2e) (f_3c88),
 * linked only by the shiftable build (splits_nonmatching.txt). Not matching coverage.
 * From src/f_2f68.c. */
struct player;
struct slot {
	char pad0[12];
	unsigned long l0c;         /* 0x0c; unsigned lets the store reuse the zero in r13 */
	char pad1[60 - 16];
};
struct player {
	char pad0[0x2f4];
	struct player *other;      /* 0x2f4 */
	char pad1[0x2fe - 0x2f8];
	char b2fe;                 /* 0x2fe */
	char pad2;
	short w300;                /* 0x300 */
	short w302;                /* 0x302 */
	short w304;                /* 0x304 */
	unsigned short w306;       /* 0x306 */
	unsigned long l308;        /* 0x308 */
	unsigned short w30c;       /* 0x30c */
	unsigned char idx;         /* 0x30e */
	char pad3[0x314 - 0x30f];
	short x;                   /* 0x314 */
	char pad4[0x322 - 0x316];
	unsigned short level;      /* 0x322 */
	char pad5[0x33a - 0x324];
	char b33a[6];              /* 0x33a */
	char pad6[0x347 - 0x340];
	char b347;                 /* 0x347 */
	char b348;                 /* 0x348 */
	char b349;                 /* 0x349 */
	char pad7[0x350 - 0x34a];
	long l350;                 /* 0x350 */
	long l354;                 /* 0x354 */
	char pad8[0x35c - 0x358];
	unsigned short w35c;       /* 0x35c */
	char pad9[0x38a - 0x35e];
	unsigned short w38a;                /* 0x38a */
	char pad10[0x39a - 0x38c];
	char b39a;                 /* 0x39a */
	char pad11;
	short w39c;                /* 0x39c */
};
extern unsigned long g_6064880;
extern unsigned char g_6079374[];
extern unsigned char g_606475a[];
extern unsigned char g_6079284[];
extern unsigned char g_6079286[];
extern unsigned char g_6079288[];
extern short g_607928a[];
extern struct slot g_60794c0[2];
extern short d_39812[];
extern short d_39bfc[];
extern short d_3903e[];
extern short d_3a3d0[];
extern short d_39fe6[];
extern short d_39428[];
extern unsigned char f_81cc(struct player *);
extern void f_20c14(struct player *);
extern void f_12e2(struct player *, int);
extern void f_1175c(struct player *);
extern void f_21454(struct player *, int, int);
extern void f_15f10(struct player *, int);
extern void f_13848(struct player *);
extern void f_8314(struct player *);

void f_2f68(struct player *p, int s);

void f_2f7a(struct player *p);

void f_2fa2(struct player *p);

void f_2fc0(struct player *p);

void f_30d0(struct player *p);

void f_3108(struct player *p);

void f_31dc(struct player *p, unsigned char flag);

void f_32fc(struct player *p);

void f_331a(struct player *p);

void f_3510(struct player *p);

void f_3574(struct player *p);

void f_35a0(struct player *p);

/* Collision test for the active piece: the field is a grid of 6-byte cells
 * with walls stored as cells, and the piece shape is a 4x4 grid per rotation
 * in d_363cc, indexed by the piece type in the low nibble of flags. Big mode
 * (flag 0x200) doubles every shape cell. Overlaps are counted in g_6077608,
 * split by whether the cell has bit 15 set, and any overlap is a collision. */
struct cell {
	unsigned short v;
	char pad[4];
};
struct field {
	struct cell *cells;
	char pad0[0xde - 4];
	unsigned char h;           /* 0xde, rows including the walls */
	unsigned char w;           /* 0xdf, columns including the walls */
	char pad1[0x30c - 0xe0];
	unsigned short mode;
	unsigned char idx;
	char pad2[0x322 - 0x30f];
	unsigned short level;
	char pad3[0x349 - 0x324];
	unsigned char das;
	char pad4[0x35e - 0x34a];
	unsigned short flags;      /* 0x35e; low nibble is the piece type */
	char pad5[0x362 - 0x360];
	unsigned char rot;
	char pad6;
	union {
		struct { short x; char pad[2]; long y; } p;
		short pos[4];
	} u;
};
struct hits {
	char pad[0x370];
	unsigned short a;          /* overlaps with plain cells */
	unsigned short b;          /* overlaps with cells that have bit 15 set */
};
extern struct hits g_6077608;
extern unsigned char d_363cc[16][4][4][4];   /* [type][row][rot][col] */

unsigned char f_35f0(struct field *f, short x, short y, unsigned char rot);

/* Rotation test with wall kicks. Returns zero if the piece fits, possibly
 * after kicking one column (two in big mode). The I piece and collisions
 * in the centre column suppress kicks. */
unsigned char f_3a02(struct field *f, short x, short y, unsigned char rot);

/* CONTEXT ONLY BELOW THIS LINE. Not linked, not counted as matched C.
 * This nonmatching DAS draft makes SHC emit f_3a02's shared pool in its
 * original position. Keep its source ordinary C; never patch instructions. */
extern unsigned char g_606475e[];
void f_3c88(struct field *f)
{
  unsigned char idx;
  char rot;
  short x;
  short y;
  unsigned char delay;
  unsigned char dir;
  idx = f->idx;
  rot = f->rot;
  x = f->u.pos[0];
  y = f->u.pos[2];
  if (f->mode & 0x80)
    delay = 14;
  else if (f->mode & 0x1000)
  {
    if (f->level >= 400)
      delay = 6;
    else if (f->level >= 300)
      delay = 8;
    else if (f->level >= 200)
      delay = 9;
    else
      delay = 10;
  }
  else if (f->level >= 900)
    delay = 6;
  else if (f->level >= 500)
    delay = 8;
  else
    delay = 14;
  dir = 0;
  if (g_606475a[idx] & 0x10)
  {
    if (g_606475e[idx] & 0x10)
    {
      f->das = 0;
      dir = 0x10;
    }
    else if (f->das < delay)
      f->das++;
    else
      dir = 0x10;
  }
  if (g_606475a[idx] & 0x20)
  {
    if (g_606475e[idx] & 0x20)
    {
      f->das = 0;
      dir = 0x20;
    }
    else if (f->das < delay)
      f->das++;
    else
      dir = 0x20;
  }
  if (dir & 0x10)
  {
    if (f->mode & 0x40)
    {
      if (!f_35f0(f, x - 2, y, rot))
        x -= 2;
    }
    else if (!f_35f0(f, x - 1, y, rot))
      x -= 1;
  }
  if (dir & 0x20)
  {
    if (f->mode & 0x40)
    {
      if (!f_35f0(f, x + 2, y, rot))
        x += 2;
    }
    else if (!f_35f0(f, x + 1, y, rot))
      x += 1;
  }
  f->u.p.x = x;
}
