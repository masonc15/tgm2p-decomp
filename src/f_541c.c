/* rom: 0x541c len: 0x3b8 func: f_541c flags: -macsave=1 -optimize=1 -speed */
/* Verified prefix 0x541c-0x57d4: f_541c, f_5760 and f_579c are retained.
 * Genuine f_57d4 below provides shared literals, including
 * the ring-index offset at 0x5808; their instructions are not linked/countable. */
struct cell { unsigned short v; char b2; unsigned char b3; char pad[2]; };
struct field {
 struct cell *cells;
 unsigned short rows[9][12];
 char pad0dc[2];
 unsigned char h,w;
 char pad0e0[0x2f4-0xe0];
 struct field *owner;
 char pad2f8[0x308-0x2f8];
 unsigned long l308;
 unsigned short w30c;
 char pad30e[0x330-0x30e];
 unsigned long score;
 char pad334[4];
 unsigned char b338,b339;
 char pad33a[0x344-0x33a];
 short w344;
 char pad346[0x378-0x346];
 unsigned char b378,pad379,b37a,b37b;
 char pad37c[8];
 long l384;
 unsigned char b388;
 char pad389[0x398-0x389];
 unsigned char b398;
 char pad399[5];
 unsigned char b39e,b39f;
};
struct grid { unsigned short c[22][20]; };
extern struct grid g_6077608;
extern unsigned long g_6064880;
/* Snapshot queued rows, restore their current positions, then duplicate them
 * after that segment. Preserve the original byte-truncated row offsets. */
unsigned char f_541c(struct field *f, unsigned char count)
{
 struct field *owner=f->owner;
 struct grid *g = &g_6077608;
 short y,x;

 for(y=0;y<count;y++) {
  for(x=1;x<11;x++)
   g->c[y][x] = ((unsigned short *)((char *)owner->rows + (unsigned char)((owner->b388+y)*24)))[x-1];
 }
 for(y=0;y<count;y++) {
  for(x=1;x<11;x++) {
   if(owner->b388+y<8)
    ((unsigned short *)((char *)owner->rows + (unsigned char)((owner->b388+y)*24)))[x-1]=g->c[y][x];
  }
 }
 for(y=0;y<count;y++) {
  for(x=1;x<11;x++) {
   if(owner->b388+y+count<8)
    ((unsigned short *)((char *)owner->rows + (unsigned char)((owner->b388+y+count)*24)))[x-1]=g->c[y][x];
  }
 }
 count*=2;
 if(owner->b388+count>8)
  count=8-owner->b388;
 return count;
}
extern unsigned char f_5262(struct field *, short);
extern long f_52b6(struct field *, long);
extern void f_5390(struct field *, short, int);
extern int f_51f4(struct field *);
extern long f_7658(struct field *, unsigned char);
extern void f_179b8(struct field *, int, int, int);
extern void f_2e6fc(int);
extern void f_3108(struct field *);
extern void f_31dc(struct field *, int);
extern short f_11e98(struct field *);
extern void f_21068(struct field *, unsigned char, unsigned char);
extern void f_219a8(struct field *);
extern void f_21a10(struct field *);
extern void f_21ad0(struct field *);

int f_5760(struct field *f, unsigned long rows)
{
 short y;
 int count=0;
 for(y=1;y<f->h-1;y++)
  if((1<<y)&rows) count++;
 return count;
}

void f_579c(struct field *f, int a, unsigned char b) { long limit=999999;if(f->w30c&1) f->score+=b*a*6; else f->score+=b*a;if(f->score>limit) f->score=limit; }

/* CONTEXT ONLY BELOW THIS LINE: genuine f_57d4 semantic draft. */
void f_57d4(struct field *f)
{
 struct field *owner=f->owner;
 unsigned char special=g_6064880&2;
 unsigned char count=0, all_clear, limit;
 short y;
 long rows=0, points;
 if(f->w30c&0x40) limit=8;
 else limit=4;
 if(f->w30c&0x100) special=0;
 for(y=1;y<f->h-1;y++) {
  if(f_5262(f,y)) {
   rows|=1<<y;
   if(special&&owner->b388+count<8) f_5390(f,y,count);
   count++;
  }
 }
 if(g_6064880&7) {
  if((f->w30c&0x10)&&((f->b338&0x75)==0x75)) {
   if(count>3) f->b39f++;
   f->b39e+=count;
  }
  if(!(f->l308&0x20)) points=f_7658(f,count);
 }
 if(count) rows=f_52b6(f,rows);
 if(f->b378) points=0;
 count=f_5760(f,rows);
 if(count) rows&=0x7fffffff;
 if(!(f->l308&0x20)) {
  all_clear=0;
  if(f->b378==0) all_clear=f_51f4(f);
  if(all_clear) f_179b8(f,10,(f->w30c&4)?6:4,0);
  if(special) {
   if(count>1&&all_clear) count=f_541c(f,count);
  } else f_579c(f,points,all_clear?4:1);
 }
 all_clear=count;
 if(f->l308&0x100000) {
  if(all_clear==0) f->l308&=~0x100000;
  else f_2e6fc(all_clear>=limit?26:9);
 } else if(all_clear>=limit) {
  f_2e6fc(31);
  f->l308|=0x100000;
 }
 if(special) {
  if(owner->b388+all_clear>8) count=8-owner->b388;
  if(f->b378==0&&count>1) owner->b388+=count;
 }
 if(rows) {
  f->l384=rows;
  f_3108(f);
  f_2e6fc(20);
 } else {
  if(f->w30c&~0x7f) f->b398++;
  f_31dc(f,0);
  if(g_6064880&4) f->owner->l308&=0x7fffffff;
 }
 f->w344=f_11e98(f);
 if(f->b378) {
  f_21068(f,f->b37b,f->b37a);
  switch(f->b378) {
  case 2: case 4: case 5: case 8: case 10: case 12: case 15:
   f->b339&=~4;
   break;
  }
 }
 if(!(f->l308&0x20)) {
  if(f->w344==0&&(g_6064880&1)&&(f->w30c&0x1002)) f_219a8(f);
  if((g_6064880&1)&&(f->w30c&0x1002)) f_21a10(f);
  if((g_6064880&1)&&(f->w30c&0x1002)) f_21ad0(f);
 }
}
