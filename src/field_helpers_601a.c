/* rom: 0x601a len: 0x1b6 func: f_601a context-rom: 0x5b80 literal-tail-calls: 1 flags: -macsave=1 -optimize=1 -speed */
/* Verified c-range 0x601a-0x61d0: f_601a, f_608c, and f_60e8.  The preceding
 * f_5b80 body is genuine context only; it places the shared SHC literal pools
 * and is excluded from the linked range. */
struct cell { unsigned short flags; unsigned char b2,b3; short timer; };
union fixed { long full; struct { short hi; unsigned short lo; } part; };
struct pos { short x; char pad[2]; union fixed y; };
struct field {
 struct cell *cells; char pad4[0xde-4]; unsigned char height,width;
 char pade0[0x2f4-0xe0]; struct field *owner;
 char pad2f8[8]; short timer;
 char pad302[6]; unsigned long flags;
 unsigned short mode; unsigned char id,pad30f; unsigned long seed;
 char pad314[0x322-0x314]; unsigned short level,lines;
 char pad326[0x330-0x326]; unsigned long score;
 char pad334[0x346-0x334]; unsigned char milestone;
 char pad347[0x350-0x347]; unsigned long time;
 char pad354[8]; unsigned short w35c,pad35e,piece; unsigned char rotation,pad363;
 struct pos position; char pad36c[4]; long remaining,available;
 unsigned char task; char pad379[5]; unsigned char delay,pad37f,last;
 char pad381[3]; unsigned long rowmask;
 char pad388[2]; unsigned short w38a;
 char pad38c[0x39e-0x38c]; unsigned char b39e,b39f,pad3a0,b3a1;
};
struct task { char active,kind; char pad2[0x80c-2]; struct field *field; };
extern struct task *f_1913c(void);
extern short d_38f58[];
extern struct field g_6064898;
extern void f_2e6fc(int),f_31dc(struct field *,unsigned char),f_21d94(struct field *),f_3574(struct field *);
extern unsigned char f_35f0(struct field *,short,short,unsigned char);
extern unsigned long g_6064880,g_606487c;
extern unsigned short f_2f3c(unsigned long *,unsigned short);
extern const unsigned char d_3a7ca[];
extern void f_7fa0(struct field *);

/* Finish line removal, then apply the active mode's level cap and tasks. */
void f_5b80(struct field *f)
{
 unsigned long rows;
 short row,y,x;
 if(--f->timer) return;
 rows=f->rowmask;
 if(rows&0x80000000) rows&=0x7fffffff;
 else f_2e6fc(25);
 if(rows) {
  row=1;
  while(rows) {
   if(!(rows&(1<<row))) row++;
   else {
    unsigned long cell_mask=0xff7f;
    for(y=row;y<f->height-1;y++) {
     unsigned short next=y+1;
     for(x=1;x<f->width-1;x++) {
      f->cells[y*f->width+x]=f->cells[next*f->width+x];
      f->cells[y*(short)f->width+x].flags&=cell_mask;
     }
    }
    for(x=1;x<f->width-1;x++) {
     f->cells[(f->height-1)*f->width+x].flags=0;
     f->cells[(f->height-1)*f->width+x].b3=0;
    }
    if((g_6064880&4)&&(unsigned char)f->owner->w35c==2) {
     struct field *other[1];
     struct pos *position[1];
     other[0]=f->owner;
     position[0]=&other[0]->position;
     if(f_35f0(other[0],position[0]->x,position[0]->y.part.hi,other[0]->rotation)) f->owner->position.y.full-=0x10000;
    }
    rows=(rows&~(1<<row))>>1;
    f->lines++;
   }
  }
 }
 f->rowmask=0;
 if(g_6064880&4) f->owner->flags&=0x7fffffff;
 f_31dc(f,1);
 if(!(f->flags&32)) {
  if((f->mode&1)&&(g_6064880&1)) {
   if(d_38f58[2]<=f->level) {
    f->level=d_38f58[2]; f->flags|=32; f_21d94(f);
    if(f->time<=18000U) {
     unsigned long remain=18000-f->time;
     unsigned long bonus=remain/60;
     if(remain%60) bonus++;
     f->score+=bonus*1253;
     if(f->score>999999U) f->score=999999;
    }
   }
  } else if((f->mode&2)&&(g_6064880&1)) {
   if(d_38f58[9]<=f->level) {
    f->level=d_38f58[9]; f_2e6fc(15); f_3574(f);
    f->b39f=0; f->b39e=0;
   }
  } else if((f->mode&0x80)&&(g_6064880&1)) {
   if(d_38f58[9]<=f->level) {
    f->level=d_38f58[9]; f->flags|=32; f_21d94(f);
   }
  } else if((f->mode&0x1000)&&(g_6064880&1)) {
   if(f->b3a1) {
    if(d_38f58[9]<=f->level) {
     f->level=d_38f58[9]; f->w38a=19; f->flags|=32; f_21d94(f);
    }
   } else if(d_38f58[4]<=f->level) {
    f->level=d_38f58[4]; f->flags|=32; f_21d94(f);
   }
  } else if((f->mode&4)&&d_38f58[2]<=f->level) {
   f->level=d_38f58[2]; f->flags|=32;
   if((f->owner->flags&32)&&!(f->owner->flags&64)) {
    struct field *other=&g_6064898;
    other->flags|=32; f_21d94(other); return;
   }
  }
 }
 if(f->task) {
  struct task *t=f_1913c();
  if(t) {t->active=1; t->kind=f->task; t->field=f;}
  f->task=0;
 }
}
unsigned char f_601a(struct field *f)
{
 unsigned char value;
 if(g_6064880&4) {
  if(f->id==1) value=f_2f3c(&f->owner->seed,1)%7;
  else value=f_2f3c(&f->seed,1)%7;
 } else value=f_2f3c(&f->seed,1)%7;
 if(g_606487c==0) value%=7;
 return value;
}
void f_608c(struct field *f,unsigned char index)
{
 f->remaining &= ~(1<<index);
 f->last=index+1;
 f->piece|=0x2000;
}
void f_60e8(struct field *f)
{
 struct field *alias;
 unsigned int one=1;
 int mode_mask=0x208;
 int mode=f->mode;
 int delay_limit=20;
 if((mode&one)&&!(mode&8)&&!(f->flags&32)) {
  if(f->level>=(f->milestone+1)*100) {
   unsigned char index=d_3a7ca[f->milestone];
   if(f->available&(one<<index)) {
    f->available&=~(one<<index);
    alias=f; f_608c(alias,index);
    f->milestone++;
   }
  } else if((f->mode&mode_mask)&&f->delay>=delay_limit&&f->remaining) f_7fa0(f);
 } else if((f->mode&mode_mask)&&f->delay>=delay_limit&&f->remaining) f_7fa0(f);
}
