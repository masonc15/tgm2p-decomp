/* rom: 0xead0 len: 0x13c func: f_ead0 flags: -macsave=1 -optimize=1 -speed */
struct obj {
	char pad0[0x10];
	long x10;
	char pad1[0x16 - 0x14];
	short x16;
};
struct spr {
	char pad0[0x14];
	unsigned char b14;
	unsigned char b15;
	char pad1[0x40 - 0x16];
};
struct vm {
	char pad0[0x20];
	struct obj *obj;           /* 0x20 */
	char pad1[0x48 - 0x24];
	short x48;                 /* 0x48 */
	char pad2[0x52 - 0x4a];
	short x52;                 /* 0x52 */
	char pad3[0x80 - 0x54];
	long regs[1];              /* 0x80 */
};
extern struct vm *g_606005c;
extern struct spr g_606006c[];
extern void f_277ac(short, long);
extern void f_2c3ca(int);
extern short g_607936e;
struct s30 { char c[30]; };
struct s2 { char a, b; };
extern struct s30 g_607930c[];
extern struct s2 g_6079366[];

void f_ead0(unsigned long a)
{
	long v;

	if (a & 0x8000)
		v = g_606005c->regs[(unsigned char)a];
	else
		v = a;
	((unsigned char *)0x2405ffe0)[a >> 16] = v;
}

void f_eaf2(unsigned long a)
{
	long y = g_606005c->regs[a + 1];
	long x = g_606005c->regs[a];
	short i = g_606005c->x48;

	g_606006c[i].b14 = x;
	g_606006c[i].b15 = y;
}

void f_eb22(unsigned long a)
{
	f_277ac(g_606005c->x52, g_606005c->regs[a]);
}

void f_eb40(unsigned long a)
{
	g_606005c->obj->x10 = g_606005c->regs[a];
}

void f_eb56(unsigned long a)
{
	g_606005c->obj->x16 = g_606005c->regs[a];
}

void f_eb6c(void)
{
}

void f_eb70(void)
{
}

void f_eb74(void)
{
}

void f_eb78(void)
{
}

void f_eb7c(void)
{
}

void f_eb80(void)
{
	int i;

	g_607936e = 0;
	f_2c3ca(15);
	for (i = 0; i < 3; i++)
		((struct s30 *)((char *)&g_607930c[0] + (char)(i * 30)))->c[0] = 0;
	for (i = 0; i < 4; i++)
		g_6079366[i].b = 0;
}
