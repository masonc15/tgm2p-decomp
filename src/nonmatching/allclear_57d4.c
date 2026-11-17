/* rom: 0x57d4 len: 0x3ac func: f_57d4 flags: -macsave=1 -optimize=1 -speed */
/* Non-matching: functionally equivalent C for [0x57d4, 0x5b80) (f_57d4),
 * linked only by the shiftable build (splits_nonmatching.txt). Not matching coverage.
 * From research/2026-10-16-57d4-allclear/t3.c. */
struct cell { unsigned short flags; unsigned char b2,b3; short timer; };
union fixed { long full; struct { short hi; unsigned short lo; } part; };
struct pos { short x; char pad[2]; union fixed y; };
struct field {
 struct cell *cells;
 unsigned short rows[9][12];
 char pad0dc[2];
 unsigned char height,width;
 char pade0[0x2f4-0xe0]; struct field *owner;
 char pad2f8[8]; short timer;
 char pad302[6]; unsigned long flags;
 unsigned short mode; unsigned char id,pad30f; unsigned long seed;
 char pad314[0x322-0x314]; unsigned short level,lines;
 char pad326[0x330-0x326]; unsigned long score;
 char pad334[4]; unsigned char b338,b339;
 char pad33a[0x344-0x33a]; short w344; unsigned char milestone;
 char pad347[0x350-0x347]; unsigned long time;
 char pad354[8]; unsigned short w35c,pad35e,piece; unsigned char rotation,pad363;
 struct pos position; char pad36c[4]; long remaining,available;
 unsigned char task,pad379,cxy[2]; char pad37c[2]; unsigned char delay,pad37f,last;
 char pad381[3]; unsigned long rowmask;
 unsigned char b388,pad389; unsigned short w38a;
 char pad38c[0x398-0x38c]; unsigned char b398; char pad399[0x39e-0x399];
 unsigned char b39e,b39f,pad3a0,b3a1;
};
struct grid { unsigned short c[22][20]; };
struct task { char active,kind; char pad2[0x80c-2]; struct field *field; };
extern struct grid g_6077608;
extern unsigned long g_6064880,g_606487c;
extern void f_17d48(struct field *f, short y);
extern long f_7658(struct field *, unsigned char);
extern void f_179b8(struct field *, int, int, int);
extern void f_2e6fc(int);
extern void f_3108(struct field *);
extern void f_31dc(struct field *, unsigned char);
extern short f_11e98(struct field *);
extern void f_21068(struct field *, short, short);
extern void f_219a8(struct field *);
extern void f_21a10(struct field *);
extern void f_21ad0(struct field *);
extern struct task *f_1913c(void);
extern short d_38f58[];
extern struct field g_6064898;
extern void f_21d94(struct field *),f_3574(struct field *);
extern unsigned char f_35f0(struct field *,short,short,unsigned char);
extern unsigned short f_2f3c(unsigned long *,unsigned short);
extern const unsigned char d_3a7ca[];
extern void f_7fa0(struct field *);
/* 1 if no interior cell (rows 1..h-2, columns 1..w-2) is set, ignoring bit 0x4000. */
int f_51f4(struct field *f);

/* 1 if every interior cell of row y is set, ignoring bit 0x4000. */
unsigned char f_5262(struct field *f, short y);

/* Clear the rows flagged in rows. Returns rows with each row that kept a
 * 0x400 cell unflagged and bit 31 set. */
long f_52b6(struct field *f, long rows);

void f_5390(struct field *pl, short idx, unsigned char add);

/* Snapshot queued rows, restore their current positions, then duplicate them
 * after that segment. Preserve the original byte-truncated row offsets. */
unsigned char f_541c(struct field *f, unsigned char count);

int f_5760(struct field *f, unsigned long rows);

void f_579c(struct field *f, int a, unsigned char b);


void f_57d4(struct field *f)
{
 struct field *owner = f->owner;
 unsigned long *mode = &g_6064880;
 unsigned char special = *mode & 2;
 unsigned char count;
 unsigned char limit;
 unsigned char all_clear;
 unsigned char m;
 short y;
 long rows;
 long points;

 count = 0;
 if (f->mode & 0x40)
  limit = 8;
 else
  limit = 4;
 rows = 0;
 if (f->mode & 0x100)
  special = 0;
 for (y = 1; y < f->height - 1; y++) {
  if (f_5262(f, y)) {
   rows |= 1 << y;
   if (special) {
    if (owner->b388 + count < 8)
     f_5390(f, y, count);
   }
   count++;
  }
 }
 m = f->b338 & 0x75;
 if (*mode & 7) {
  if ((f->mode & 0x10) && m == 0x75) {
   if (count > 3)
    f->b39f++;
   f->b39e += count;
  }
  if (!(f->flags & 0x20))
   points = f_7658(f, count);
 }
 if (count)
  rows = f_52b6(f, rows);
 if (f->task)
  points = 0;
 count = f_5760(f, rows);
 if (count)
  rows &= 0x7fffffff;
 if (!(f->flags & 0x20)) {
  all_clear = f->task ? 0 : f_51f4(f);
  if (all_clear) {
   if (f->mode & 4)
    f_179b8(f, 10, 6, 0);
   else
    f_179b8(f, 10, 4, 0);
  }
  if (special) {
   if (count > 1 && all_clear)
    count = f_541c(f, count);
  } else
   f_579c(f, points, all_clear ? 4 : 1);
 }
 if (f->flags & 0x100000) {
  if (!count)
   f->flags &= ~0x100000;
  else if (count >= limit)
   f_2e6fc(26);
  else
   f_2e6fc(9);
 } else if (count >= limit) {
  f_2e6fc(31);
  f->flags |= 0x100000;
 }
 if (special) {
  if (owner->b388 + count > 8)
   count = 8 - owner->b388;
  if (f->task == 0 && count > 1)
   owner->b388 = owner->b388 + count;
 }
 if (rows) {
  f->rowmask = rows;
  f_3108(f);
  f_2e6fc(20);
 } else {
  if (f->mode & 0x80)
   f->b398++;
  f_31dc(f, 0);
  if (*mode & 4)
   f->owner->flags &= 0x7fffffff;
 }
 f->w344 = f_11e98(f);
 if (f->task) {
  f_21068(f, f->cxy[1], f->cxy[0]);
  switch (f->task) {
  case 2: case 4: case 5: case 8: case 10: case 12: case 15:
   f->b339 &= ~4;
   break;
  }
 }
 if (!(f->flags & 0x20)) {
  if (f->w344 == 0) {
   if ((*mode & 1) && (f->mode & 0x1002))
    f_219a8(f);
  }
  if ((*mode & 1) && (f->mode & 0x1002))
   f_21a10(f);
  if ((*mode & 1) && (f->mode & 0x1002))
   f_21ad0(f);
 }
}
/* Finish line removal, then apply the active mode's level cap and tasks. */
void f_5b80(struct field *f);
unsigned char f_601a(struct field *f);
void f_608c(struct field *f,unsigned char index);
void f_60e8(struct field *f);
