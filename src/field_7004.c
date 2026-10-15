/* rom: 0x7004 len: 0x320 func: f_7004 literal-tail-calls: 1 flags: -macsave=1 -optimize=1 -speed */
/* Verified retained prefix 0x7004-0x7324: f_7004/f_7264.
 * f_7324 is genuine unlinked following context. The long r3 tail requires
 * exact ordered setup/restores, CFG and original ELF relocation proof. */
struct cell { unsigned short flags; unsigned char b2,b3; short w4; };
struct field {
 struct cell *cells; char pad4[0xde-4]; unsigned char height,width;
 char pad_e0[0x2f4-0xe0]; struct field *owner;
 char pad_2f8[8]; short timer,w302,w304,w306;
 unsigned long flags; unsigned short mode; unsigned char id,pad30f;
 char pad310[4]; short column; char pad316[2]; short w318,w31a,w31c;
 char pad31e[4]; unsigned short level; char pad324[0x34c-0x324];
 void *p34c; char pad350[12]; unsigned short w35c;
 char pad35e[0x37e-0x35e]; unsigned char b37e;
 char pad37f[13]; unsigned short w38c; char pad38e[10];
 unsigned char b398,b399,b39a;
};
struct piece { short x,y; unsigned char b4,pad5,b6,pad7,b8,b9; short w10; };
extern unsigned char d_3a7cd[];
extern short d_38f58[],g_6060038[],g_607928a[];
extern unsigned char g_606475a[];
extern unsigned long g_6064880;
extern struct piece d_aae98[],d_a6f14[],*d_3ad6c[];
extern char g_6032870[],g_6032878[];
extern void f_2e6fc(int),f_35a0(struct field *),f_2f68(struct field *,int);
extern void f_23e3e(struct field *),f_211e8(void *),f_29cc6(long,long,long,long);
extern void f_1159c(struct piece *,long,long,unsigned char,short);
extern void f_11680(struct piece *,short,short,unsigned char,short,short,short,unsigned char);
extern int f_e39c(char *);
extern void f_e490(int,int,char *,int,int);
void f_7004(struct field *f) {
 unsigned char limit=13; short y,x;
 if(f->level>=100)limit=12;
 if(f->level>=200)limit=11;
 if(f->level>=300)limit=10;
 if(f->level>=400)limit=9;
 if(f->level>=500)limit=8;
 if(f->level>=600)limit=7;
 if(f->level>=700)limit=6;
 if(f->level>=800)limit=5;
 if(f->level>=900)limit=4;
 if(f->b398<limit)return;
 f->b398=0;
 for(y=21;y>1;y--) for(x=1;x<11;x++)
  f->cells[y*f->width+x]=f->cells[(y-1)*f->width+x];
 for(x=1;x<11;x++) {
  f->cells[f->width+x].flags=(d_3a7cd+(unsigned char)(f->b399*10))[x-1]?9:0;
  f->cells[f->width+x].b3=0;
  f->cells[f->width+x].b2=0;
 }
 f->b399=(f->b399+1)%24;
 f_2e6fc(18);
}
void f_7264(register struct field *f) {
 short max; register struct field *q;
 if(--f->timer!=0) {
  f_35a0(f);
  if(f->mode&128)f_7004(f);
 } else {
  q=f;
  f_2f68(f,12);
  q->w35c&=0x7fff;
  if(f->mode&8)max=d_38f58[*g_6060038]-1;
  else if(f->mode&4)max=d_38f58[2]-1;
  else if(f->mode&1)max=d_38f58[2]-1;
  else max=d_38f58[f->w38c]-1;
  if(!(f->flags&32)) {
   f->level++;
   if(f->level>max)f->level=max;
  }
 }
}
/* Unlinked following compiler context begins here. */
void f_7324(register struct field *f) {
 register int cooperative=4; register unsigned long *global=&g_6064880; register int pressed=1; short x,y,t;
 if((*global&pressed)&&(f->flags&0x80000))f_23e3e(f);
 else if(!(*global&cooperative)&&f->w306>=5) {
  f_1159c(d_aae98,158,f->column-35,2,110);
  t=g_607928a[f->id];
  x=-((t-63)*16384)/63;
  x>>=10;
  y=-((t-63)*16384)/63;
  y>>=10;
  f_11680(d_3ad6c[f->w306-1],x+170,f->column+y-13,1,110,t,t,0);
  g_607928a[f->id]-=2;
  if(g_607928a[f->id]<63)g_607928a[f->id]=63;
 }
 if(f->w302!=0){f->w302=0;f_2e6fc(39);}
 if(f->w304!=0){
  f_1159c(d_a6f14,100,f->column,0,125);
  if(f->owner->b39a!=0){
   f_e490(f->column-f_e39c(g_6032870)/2,65,g_6032870,15,0);
   f_e490(f->column-f_e39c(g_6032878)/2,77,g_6032878,15,0);
  }
 }
 if(--f->timer>0){
  if((g_606475a[f->id]&pressed)&&f->timer>10)f->timer=10;
  if((*global&cooperative)&&(g_606475a[f->owner->id]&pressed)&&f->timer>10)f->timer=10;
  if((*global&cooperative)&&f->timer==10)f_29cc6(2,10,0,6);
  f->flags&=0xfffdbfff;f->flags|=0x50000;
  return;
 }
 f->w31c=0;f->w31a=0;f->w318=0;f->b37e=0;
 f->flags=0x10050010;
 f->mode&=0x108f;f->mode&=0xfffb;
 f_211e8(f->p34c);
 if(*global&cooperative){f->owner->flags=0x10000010;f->owner->mode&=0xfffb;}
}
