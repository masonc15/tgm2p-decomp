/* rom: 0x1108 len: 0x158 func: f_1108 context-rom: 0x780 flags: -macsave=1 -optimize=1 -speed */
/* Retain only the video initializer f_1108, ROM [1108,1260). The genuine
 * startup predecessor f_780 is unlinked context: its 292-byte frame (256
 * bytes never addressed), string literals, indexed text tables and
 * unprototyped callee declarations reproduce its original size and the
 * compiler state that f_1108 depends on. */
struct frame;

struct frame;
struct rom_hdr { char pad[28]; long *table; };
struct font_owner { char pad[0x118]; short *palette; };
struct scores { char pad[0x44]; struct { unsigned long a,b; } section[10]; };
struct object { char pad[0x2c]; unsigned short flags; char tail[0x40-0x2e]; };
extern struct rom_hdr *rom_hdr_ptr;
extern struct font_owner *g_607926c;
extern struct scores g_6065650;
extern struct object g_606006c[];
extern unsigned long g_60775a8[6];
extern unsigned long g_6065750[10],g_6065778[10];
extern unsigned long g_6060008,g_6079538;
extern long g_6060004,g_6079268;
extern short g_606434c,g_6060022,g_6060034;
extern unsigned char g_6064754,g_6064758,g_6064759;
extern char *g_6030f34[7],*g_6030f68[5];
extern void *g_6030f50[6],*g_6030f7c[5],*g_6030f90[5];

extern void f_1260(void);
extern void f_9b90(void);
extern int f_b2f4(void);
extern void f_b352(void);
extern void f_b36e(void);
extern void f_e3b8(void);
extern void f_e3e0();
extern void f_e412();
extern void f_e520();
extern void f_11fe8(void);
extern void f_120c0(void);
extern void f_13378(void);
extern int f_13380(void);
extern void f_1715c(void);
extern void f_175c4(void);
extern void f_185f0();
extern void f_211a2(void);
extern void f_21206(void);
extern int f_212f8(void);
extern int f_213da();
extern void f_2140e();
extern void f_23f10(void);
extern void f_2488c(void);
extern void f_2491c(void);
extern short f_2528c(void);
extern void f_252f8();
extern void f_253bc();
extern void f_25658(void);
extern void f_29bf8(void);
extern void f_29c50();
extern void f_29f94();
extern void f_2a08c(void);
extern void f_2af5e();
extern void f_2b3e8();
extern void f_2b430(void);
extern int f_2b4e8(void);
extern short f_2bf58();
extern void f_2c28c(void);
extern void f_2c3d0();
extern void f_2d2bc(void);
extern void f_2e6fc();
extern void f_2ef3e(void);
extern void f_3003c(void);
extern int f_30a44(void);
void f_1108(void);
void f_780(void);
extern unsigned long g_6065644;
extern short g_606564c;
extern unsigned char g_6030c7c[];
extern unsigned char g_6030cbc[];
extern unsigned char g_6030cfc[];
extern unsigned char g_6030d08[];
extern unsigned char g_6030d0c[];
extern unsigned char g_6030d10[];
extern unsigned char g_6030d1c[];
extern unsigned char g_6030d24[];



#define PORT0 (*(volatile unsigned char *)0x23000000)
#define PORT3 (*(volatile unsigned char *)0x23000003)
#define PORT4 (*(volatile unsigned char *)0x23000004)
#define VIDEO_MODE (*(volatile unsigned char *)0x2405ffee)

/* Recovering the genuine startup sequence. ROM reserves 292 frame bytes and
 * never forms an r15 address: 256 bytes belong to an unreferenced local. */
void f_780(void)
{
 unsigned int i, j, timer, interrupted, errors, n;
 short object, x;
 long *table;
 void **sprites;
 char work[256];

 errors = 0;
 table = rom_hdr_ptr->table;
 f_2b430();
 g_6060008 = 0;
 g_606434c = 0;
 f_2ef3e();
 f_2e6fc(29);
 f_23f10();
 f_2d2bc();
 f_1108();
 f_25658();
 f_1715c();
 f_175c4();
 f_2a08c();
 f_2491c();
 f_1260();
 f_120c0();
 g_6065644 = 0;
 g_606564c = 0;
 f_2488c();
 g_6060022 = 0;
 f_2b3e8(g_607926c->palette);
 f_2af5e(1,1,(long *)g_6030c7c);
 f_2c3d0(0);
 f_2af5e(1,15,(long *)0x63b50);
 f_2af5e(158,1,(long *)0x68450);
 f_2af5e(149,7,(long *)0x68250);
 f_2af5e(156,1,(long *)0x68190);
 f_29c50(0);
 f_29bf8();
 if ((~PORT3 & 0x40) && (~PORT0 & 8)) {
  f_b352(); f_211a2(); f_11fe8(); f_b36e(); f_21206(); f_13378();
  f_2140e((unsigned short)g_60775a8[5]);
 }
 f_3003c();
 g_60775a8[4]=f_b2f4();
 g_60775a8[4]&=f_212f8();
 g_60775a8[4]&=f_13380();
 for (i=0;i<10;i++) {
  g_6065750[i]=g_6065650.section[i].b & 0xfffff;
  g_6065778[i]=2520;
 }
 if(g_6060034==1) VIDEO_MODE|=0xc0;
 if(!f_213da((unsigned short)g_60775a8[5]) && (~PORT3&0x40)) { }
 if(g_60775a8[0]&g_60775a8[1]&g_60775a8[2]&g_60775a8[3]&g_60775a8[4])
  timer=60;
 else {timer=600;errors=1;}
 g_6079268=0;
 if((PORT4&15)==1) g_6079268=1;
 if(g_6060004==1 && !(PORT4&15)) g_6060004=0;
 f_2af5e(0,1,(long *)g_6030cbc);
 f_2af5e(10,1,(long *)g_6030c7c);
 f_2af5e(81,1,(long *)0x67910);
 interrupted=0;
 while(interrupted ? f_30a44() : timer!=0) {
  if((g_6064754&1) && (~PORT3&0x40)) interrupted=1;
  f_2b4e8(); f_2c28c(); f_e3b8();
  if(timer) timer--;
  if((g_6064754&15) && errors) timer=0;
  if(g_60775a8[0]) {
   f_e412(65,20,"WORK RAM",0);
   f_e520(146,20,"O",0);
  } else {
   f_e412(65,20,"WORK RAM",0);
   f_e520(146,20,"X",0);
  }
  if(g_60775a8[1]&g_60775a8[2]&g_60775a8[3]) {
   f_e412(65,40,"VIDEO RAM",0);
   f_e520(146,40,"O",0);
  } else {
   f_e412(65,40,"VIDEO RAM",0);
   f_e520(146,40,"X",0);
  }
  if(g_60775a8[4]) {
   f_e412(65,60,"EEP-ROM",0);
   f_e520(146,60,"O",0);
  } else {
   f_e412(65,60,"EEP-ROM",0);
   f_e520(146,60,"X",0);
  }
 }
 f_29c50(0); f_29bf8();
 object=f_2528c();
 f_253bc(object,60,266,*table);
 if(PORT3&32) {
  switch(g_6060004) {
  case 0:
   f_29f94(0x7f00,0x7f00,0,255);
   for(timer=0;timer<500;timer++) {
    f_2b4e8();
    if((((g_6064758&15)||(g_6064759&15)) && timer>60) || !(PORT3&32)) timer=600;
    f_e3e0(30,"- Warning! -",0);
    for(i=0;i<7;i++) f_e412(55,60+i*20,g_6030f34[i],0);
   }
   break;
  case 1:
   if(!(PORT4&15)) {
    g_6060004=0;
    f_29f94(0x7f00,0x7f00,0,255);
    for(timer=0;timer<500;timer++) {
     f_2b4e8();
     if((((g_6064758&15)||(g_6064759&15)) && timer>60) || !(PORT3&32)) timer=600;
     f_e3e0(30,"- Warning! -",0);
     for(i=0;i<7;i++) f_e412(55,60+i*20,g_6030f34[i],0);
    }
   }
   break;
  case 2:
   f_29f94(0x7f00,0x7f00,0,255);
   for(timer=0;timer<500;timer++) {
    f_2b4e8();
    if((((g_6064758&15)||(g_6064759&15)) && timer>60) || !(PORT3&32)) timer=600;
    f_e3e0(30,"- Warning! -",0);
    for(i=0;i<5;i++) f_e412(55,70+i*20,g_6030f68[i],0);
   }
   break;
  case 3:
   f_29f94(0x7f00,0x7f00,0,255);
   for(timer=0;timer<500;timer++) {
    f_2b4e8();
    if((((g_6064758&15)||(g_6064759&15)) && timer>60) || !(PORT3&32)) timer=600;
    switch(PORT4&15) {
    case 1:
     x=46;
     g_606006c[object].flags&=0x7fff;
     sprites=g_6030f50;
     for(i=0;i<6;i++) {f_2bf58(*sprites++,x,40);x+=24;}
     break;
    case 2:
     x=46;
     g_606006c[object].flags&=0x7fff;
     sprites=(void **)g_6030f68;
     for(i=0;i<5;i++) {f_2bf58(*sprites++,x,40);x+=24;}
     break;
    case 4:
     x=46;
     g_606006c[object].flags&=0x7fff;
     sprites=g_6030f7c;
     for(i=0;i<5;i++) {f_2bf58(*sprites++,x,40);x+=24;}
     break;
    case 8:
     x=46;
     g_606006c[object].flags&=0x7fff;
     sprites=g_6030f90;
     for(i=0;i<5;i++) {f_2bf58(*sprites++,x,40);x+=24;}
     break;
    }
   }
   break;
  }
  g_606006c[object].flags|=0x8000;
  f_29bf8();
  f_29f94(0,0,0,255);
  f_2af5e(80,1,(long *)0x678d0);
  for(i=0;i<500;i++) {
   f_2b4e8();
   f_185f0((struct frame *)0xa7a0c,104,32,80,125);
   f_185f0((struct frame *)0xa79f4,0,0,80,125);
   if((g_6064758&15)||(g_6064759&15)||!(PORT3&32)) i=600;
  }
  f_252f8(object);f_2b4e8();
 }
 g_6079538=1;
 f_9b90();
}

extern void f_2e18c(short, long);
extern short g_6030d32[];

void f_1108(void)
{
	register unsigned int i, j;

	*(unsigned char *)0x2405ffea = 32;
	*(unsigned char *)0x2405ffec = 10;
	*(unsigned char *)0x2405ffed = 23;
	*(unsigned char *)0x2405ffee = 40;
	*(unsigned char *)0x2405ffef = 184;
	*(unsigned char *)0x2405fff0 = 0;
	*(unsigned char *)0x2405fff1 = 170;
	*(unsigned char *)0x2405fff2 = 48;
	*(unsigned char *)0x2405ffdc = 0;
	*(unsigned char *)0x2405ffdc |= 0x84;
	*(unsigned char *)0x2405ffdd |= 0x80;
	*(unsigned char *)0x2405fff4 = 131;
	*(unsigned char *)0x2405fff5 = 255;
	*(unsigned char *)0x2405fff7 = 14;
	*(unsigned char *)0x2405fffe = 0;
	*(unsigned char *)0x2405ffff = 0;
	*(unsigned char *)0x2405ffe8 = 53;
	*(unsigned char *)0x2405ffe9 = 103;
	*(unsigned char *)0x2405ffeb = 135;
	*(unsigned char *)0x2405fffc = 8;
	for (i = 0; i < 8; i++)
		((unsigned char *)0x2405ffe0)[i] = 0;
	for (i = 0; i < 256; i++)
		((short *)0x24050000)[i] = g_6030d32[i];
	for (i = 0; i < 256; i++)
		for (j = 0; j < 16; j++)
			*(long *)(0x24040000 + (i * 64 + j * 4)) = -1;
	f_2e18c(0, 0);
	*(short *)0x24003800 = 0;
	*(short *)0x24003802 = 0x4000;
}
