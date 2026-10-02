/* rom: 0x3e2e len: 0x216 func: f_3e2e context-rom: 0x2f68 flags: -macsave=1 -optimize=1 -speed */
/* c-range: retain only f_3e2e and f_3f92, ROM 0x3e2e-0x4044.
 * Everything before f_3e2e is unlinked compiler context. In particular,
 * the signed-index DAS draft is NOT matched or counted as C. Earlier
 * functions reproduce SHC's same-file calls and literal-pool placement;
 * their definitions here are never exported into the final ROM. */
struct player;
struct slot
{
  char pad0[12];
  unsigned long l0c;
  char pad1[60 - 16];
};
struct player
{
  char pad0[0x2f4];
  struct player *other;
  char pad1[0x2fe - 0x2f8];
  char b2fe;
  char pad2;
  short w300;
  short w302;
  short w304;
  unsigned short w306;
  unsigned long l308;
  unsigned short w30c;
  unsigned char idx;
  char pad3[0x314 - 0x30f];
  short x;
  char pad4[0x322 - 0x316];
  unsigned short level;
  char pad5[0x33a - 0x324];
  char b33a[6];
  char pad6[0x347 - 0x340];
  char b347;
  char b348;
  char b349;
  char pad7[0x350 - 0x34a];
  long l350;
  long l354;
  char pad8[0x35c - 0x358];
  unsigned short w35c;
  char pad9[0x38a - 0x35e];
  unsigned short w38a;
  char pad10[0x39a - 0x38c];
  char b39a;
  char pad11;
  short w39c;
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
struct cell
{
  unsigned short v;
  char pad[4];
};
struct field
{
  struct cell *cells;
  char pad0[0xde - 4];
  unsigned char h;
  unsigned char w;
  char pad1[0x2f4 - 0xe0];
  struct field *other;
  char pad2[0x308 - 0x2f8];
  unsigned long l308;
  unsigned short mode;
  unsigned char idx;
  char pad3[0x322 - 0x30f];
  unsigned short level;
  char pad4[0x334 - 0x324];
  long l334;
  unsigned char b338;
  unsigned char b339;
  char pad5[0x347 - 0x33a];
  signed char lock;
  signed char lock_delay;
  unsigned char das;
  char pad6[0x358 - 0x34a];
  unsigned long grav;
  unsigned short w35c;
  unsigned short flags;
  char pad7[0x362 - 0x360];
  unsigned char rot;
  char pad8;
  union {
    struct {
      short x;
      char pad[2];
      union { long l; struct { short row; unsigned short frac; } s; } y;
    } p;
    short pos[4];
  } u;
  char pad10[0x37c - 0x36c];
  unsigned short w37c;
  char pad11[0x38c - 0x37e];
  short w38c;
  char pad12[0x396 - 0x38e];
  unsigned short turns;
};
struct hits
{
  char pad[0x370];
  unsigned short a;
  unsigned short b;
};
extern struct hits g_6077608;
extern unsigned char d_363cc[16][4][4][4];
void f_2f68(struct player *p, int s)
{
 p->w35c &= 0xff00;
 p->w35c |= s;
}

void f_2f7a(struct player *p)
{
 p->l308 |= 0x40000;
 f_2f68(p, 10);
 p->w300 = 80;
}

void f_2fa2(struct player *p)
{
 f_2f68(p, 12);
 p->w35c &= 0x7fff;
}

void f_2fc0(struct player *p)
{
 int i;
 f_2f68(p, 2);
 p->l308 |= 0x20000;
 if (g_6064880 & 4)
  p->l308 &= ~0x4000;
 else
  p->l308 |= 0x4000;
 g_6079286[p->idx] = 0;
 g_6079284[p->idx] = 0;
 p->w39c = 0;
 if (p->w30c & 0x80) {
 } else if (p->w30c & 0x1000) {
  if (p->level > 500)
   i = 500;
  else
   i = p->level;
  p->b348 = d_39bfc[i];
 } else {
  if (p->level > 999)
   p->level = 999;
  if (p->level >= 500)
   p->b348 = d_39812[p->level - 500];
 }
 if (p->w30c & 0x2000)
  p->b348 = d_39812[499];
 p->b347 = p->b348;
}

void f_30d0(struct player *p)
{
 f_2f68(p, 3);
 p->b347 = 0;
 g_6079288[p->idx] = 0;
 p->l308 &= ~0x4000;
 p->l308 |= 0x80;
}

void f_3108(struct player *p)
{
 int i;
 f_2f68(p, 4);
 if (p->w30c & 0x1000) {
  if (p->level > 500)
    i = 500;
   else
    i = p->level;
   p->w300 = d_3a3d0[i];
 } else if (p->w30c & 0x80) {
  p->w300 = 40;
 } else {
  if (p->level > 999)
   p->level = 999;
  if (p->level >= 500)
   p->w300 = d_3903e[p->level - 500];
  else
   p->w300 = 40;
 }
 if (p->w30c & 0x2000)
  p->w300 = d_3903e[499];
 if (g_6064880 & 4)
  p->w300 = 1;
}

void f_31dc(struct player *p, unsigned char flag)
{
 int i;
 p->l308 &= ~0x80;
 f_2f68(p, 5);
 if (flag) {
  if (p->w30c & 0x1000) {
   if (p->level > 500)
    i = 500;
   else
    i = p->level;
   p->w300 = d_3a3d0[i];
  } else if (p->w30c & 0x80) {
   p->w300 = 25;
  } else {
   if (p->level > 999)
    p->level = 999;
   if (p->level >= 500)
    p->w300 = d_3903e[p->level - 500];
   else
    p->w300 = 25;
  }
 } else {
  if (p->w30c & 0x1000) {
   if (p->level > 500)
    i = 500;
   else
    i = p->level;
   p->w300 = d_39fe6[i];
  } else if (p->w30c & 0x80) {
   p->w300 = 25;
  } else {
   if (p->level >= 500)
    p->w300 = d_39428[p->level - 500];
   else
    p->w300 = 25;
  }
 }
 if (p->w30c & 0x2000)
  p->w300 = d_3903e[499];
}

void f_32fc(struct player *p)
{
 f_2f68(p, 6);
 p->b2fe = 0;
}

void f_331a(struct player *p)
{
 struct slot *q = &g_60794c0[p->idx];
 short i;
 p->w300 = 360;
 p->w302 = 1;
 p->w306 = f_81cc(p);
 g_607928a[p->idx] = 96;
 p->l308 &= ~0x24000;
 p->l308 |= 0x50000;
 if (p->l308 & 0x20)
  if (p->w30c & 0x1085)
   if (p->l308 & 0x40) {
    if (0) p->w302 = 0;
    p->w300 = 180;
    goto skip;
   }
 p->w304 = 0;
 f_20c14(p);
skip:
 p->l308 &= ~0x20;
 p->l308 |= 0x40;
 f_2f68(p, 7);
 if (g_6064880 & 4) {
  p->other->l308 |= 0x80000040;
  p->other->l308 &= ~0x20000;
 }
 if (g_6064880 & 2) {
  f_12e2(p, 0);
  p->l350 = p->w38a = p->level = 0;
  for (i = 0; i < 6; i++)
   p->b33a[i] = 0;
  f_1175c(p);
  f_21454(p, 67, p->x + 49);
 }
 f_15f10(p, 0);
 q->l0c = 0;
 if (!(p->w30c & ~0x1f))
  if (!(p->w30c & 8))
   f_13848(p);
 if (p->w30c == 0x1000 || p->w30c == 0x80) {
  f_13848(p);
  p->l354 = p->l350;
 }
 f_8314(p);
 p->b39a = 0;
}

void f_3510(struct player *p)
{
 unsigned short m;
 f_2f68(p, 9);
 p->l308 |= 0x50000;
 p->w300 = 1;
 p->w306 = 12;
 m = p->idx == 0 ? 0x100 : 0x200;
 if (g_6064880 & 0x400)
  p->w302 = 0;
 else if (g_6064880 & m)
  p->w302 = 0x80;
 else
  p->w302 = 0;
}

void f_3574(struct player *p)
{
 f_2f68(p, 13);
 p->w300 = 0;
 p->w302 = 1;
 g_6079374[p->idx] = 0;
}

void f_35a0(struct player *p)
{
 if (g_606475a[p->idx] & 0x30)
  p->b349++;
 else
  p->b349 = 0;
}

unsigned char f_35f0(struct field *f, short x, short y, unsigned char rot)
{

  unsigned short i;
  unsigned short c;
  struct cell *p;
  unsigned short k;
  unsigned short j;
  g_6077608.b += 0;
  k = f->flags;
  g_6077608.b = 0;
  g_6077608.a = 0;
  k &= 15;
  if (f->flags & 0x200)
  {
    y += 1;
    x -= 2;
    for (i = 0; i < 8; i++)
    {
      if ((y - i) < f->h)
      {
        p = &f->cells[((y - i) * f->w) + x];
        for (j = 0; j < 8; j++, p++)
        {
          if ((x + j) >= 0)
          {
            c = p->v & 0xbfff;
            if (d_363cc[k][i >> 1][rot][j >> 1] && c)
            {
              if (p->v & 0x8000)
              {
                g_6077608.b++;
              }
              else
              {
                g_6077608.a++;
              }
            }
          }
        }

      }
    }

  }
  else
  {
    for (i = 0; i < 4; i++)
    {
      if ((y - i) < f->h)
      {
        for (j = 0; j < 4; j++)
        {
          if ((x + j) >= 0)
          {
            c = f->cells[(((y - i) * f->w) + x) + j].v & 0xbfff;
            if ((d_363cc[k][i][rot][j] != 0) && (c != 0))
            {
              if (f->cells[(((y - i) * f->w) + x) + j].v & 0x8000)
              {
                g_6077608.b++;
              }
              else
              {
                g_6077608.a++;
              }
            }
          }
        }

      }
    }

  }
  if (g_6077608.a + g_6077608.b)
  {
    return 1;
  }
  if (0) g_6077608.a = 0x8000;
  return 0;

}


/* Rotation test with wall kicks. Returns 0 when the piece fits at
 * (x, y) in rotation rot, possibly after kicking one column right or left
 * (two in big mode), and 1 when it doesn't. The kick is skipped for the I
 * piece (type 2) and when the first blocked cell, scanning rows top down,
 * is in the piece's centre column. */
unsigned char f_3a02(struct field *f, short x, short y, unsigned char rot)
{

  unsigned short n;
  unsigned short j;
  unsigned short m;
  unsigned short c;
  struct cell *p;
  unsigned short k;
  unsigned short i;
  k = (m = f->flags);
  k &= 15;
  if (m & 0x200)
  {
    y += 1;
    x -= 2;
    for (i = 0; i < 8; i++)
    {
      if ((y - i) < f->h)
      {
        f->cells = f->cells;
        for (j = 0; j < 8; j++)
        {
          if (((x + j) >= 0) && ((x + j) <= (f->w - 1)))
          {
            c = f->cells[(((y - i) * f->w) + x) + j].v & 0xbfff;
            if (d_363cc[k][i >> 1][rot][j >> 1] && c)
            {
              if (((k == 2) || (j == 2)) || (j == 3))
              {
                return 1;
              }
              for (n = 1; n < 2; n++)
              {
                if (!f_35f0(f, (x + (n * 2)) + 2, y - 1, rot))
                {
                  f->u.p.x += n * 2;
                  return 0;
                }
              }

              for (n = 1; n < 2; n++)
              {
                if (!f_35f0(f, (x - (n * 2)) + 2, y - 1, rot))
                {
                  f->u.p.x -= n * 2;
                  return 0;
                }
              }

              return 1;
            }
          }
        }

      }
    }

  }
  else
  {
    for (i = 0; i < 4; i++)
    {
      if ((y - i) < f->h)
      {
        p = &f->cells[((y - i) * f->w) + x];
        for (j = 0; j < 4; j++, p++)
        {
          if (((x + j) >= 0) && ((x + j) <= (f->w - 1)))
          {
            c = p->v & 0xbfff;
            if (d_363cc[k][i][rot][j] && c)
            {
              if ((k != 2) && (j != 1))
              {
                if (!f_35f0(f, x + 1, y, rot))
                {
                  f->u.p.x += 1;
                  return 0;
                }
                if (!f_35f0(f, x - 1, y, rot))
                {
                  f->u.p.x -= 1;
                  return 0;
                }
              }
              return 1;
            }
          }
        }

      }
    }

  }
  return 0;

}

extern unsigned char g_606475e[];
extern unsigned char g_6079288[];
extern void f_2e6fc(short);

/* Sideways movement with DAS: a fresh press moves at once and resets the
 * charge counter, a held direction moves once the counter reaches the
 * delay for the current level. Big mode moves two columns. */
void f_3c88(struct field *f)
{
  char idx;
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

long f_3f92(struct field *f, long g);

/* RETAINED RANGE STARTS HERE: gravity and lock delay for one frame.
 * The sound routine is ROM 0x2e6fc, copied to RAM 0x0602df7c. */
void f_3e2e(struct field *f, unsigned long g)
{
  char rot;
  short x;
  struct hits *h;
  union { long l; struct { short row; unsigned short frac; } s; } y;
  rot = f->rot;
  x = f->u.p.x;
  h = &g_6077608;
  y.l = f_3f92(f, g);
  if (!f_35f0(f, x, y.s.row, rot))
  {
    if (f->u.p.y.s.row > y.s.row)
      f->lock = f->lock_delay;
    f->u.p.y.l = y.l;
  }
  if (f_35f0(f, x, f->u.p.y.s.row - 1, rot) && (!(f->mode & 4) || h->a != 0))
  {
    if (f->lock == f->lock_delay)
      f_2e6fc(33);
    if (f->lock)
      f->lock--;
    f->u.p.y.s.frac = 0xffff;
    if ((g_606475a[f->idx] & 0xf0) == 0x40)
    {
      if (f->mode & 0x1000)
      {
        if (f->level >= 0)
        {
          if (g_6079288[f->idx])
            f->lock = 0;
        }
        else
          f->lock = 0;
      }
      else if (f->level >= 900)
      {
        if (g_6079288[f->idx])
          f->lock = 0;
      }
      else
        f->lock = 0;
    }
  }
  if (f->lock <= 0)
  {
    f_30d0((struct player *)f);
    if (g_6064880 & 4)
      f->other->l308 |= 0x80000000;
  }
}

/* Moves the piece down by g (16.16 rows): the fraction always, then one
 * whole row at a time while the row below is free. Returns the new y. */
long f_3f92(struct field *f, long g)
{
  short n;
  char rot;
  short x;
  union { long l; struct { short row; unsigned short frac; } s; } y;
  n = g >> 16;
  rot = f->rot;
  x = f->u.p.x;
  if (n < 1)
    y.l = f->u.p.y.l - g;
  else
  {
    y.l = f->u.p.y.l - (unsigned short)g;
    if (!f_35f0(f, x, y.s.row, rot))
      while (n > 0)
      {
        if (f_35f0(f, x, y.s.row - 1, rot))
          n = 0;
        else
          y.l -= 0x10000;
        n--;
      }
  }
  return y.l;
}
