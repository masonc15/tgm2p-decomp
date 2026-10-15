/* rom: 0x131e8 len: 0x84 func: f_131e8 context-rom: 0x131e8 flags: -macsave=1 -optimize=1 -speed */
/* C identifier-list EEPROM definitions receive promoted integer arguments.
 * Callers use no-prototype declarations; the implementation stores byte addresses.
 * Both sides are verified against the original machine code (checkpoint122). */
struct state {
	char pad0[0x44];
	short t44[19][4];          /* 0x044 */
	short tdc[3][4];           /* 0x0dc */
	short tf4[3];              /* 0x0f4 */
	unsigned short sum;        /* 0x0fa */
};
extern struct state g_6065650;
extern long g_60794bc;
extern char g_607cf0c;
extern void f_3068c();
extern void f_30844();
extern void f_2b4e8(void);
extern void f_11fe8(void);

/* EEPROM entries authenticated from current build/obj/eeprom.o GLOBAL STT_FUNC exports:
 * origin2fdd8 +8b4=3068c, +a6c=30844. These are genuine C entry aliases. */
#define E g_6065650
#pragma inline(diff)
static short diff(void *b, void *p)
{
	return (char *)p - (char *)b;
}
#define ofs(p) diff(&E, p)
#define OFS(p) ofs(p)

unsigned short f_131e8(void)
{
	int sum = 0;
	int i;
	int j;
	short *p;

	for (i = 0; i < 19; i++) {
		p = E.t44[i];
		for (j = 0; j < sizeof(E.t44[0]) / sizeof(short); j++)
			sum += *p++;
	}
	for (i = 0; i < 3; i++) {
		p = E.tdc[i];
		for (j = 0; j < sizeof(E.tdc[0]) / sizeof(short); j++)
			sum += *p++;
	}
	for (i = 0; i < 3; i++)
		sum += E.tf4[i];
	sum++;
	return sum;
}

void f_1326c(void)
{
	short o;
	unsigned short s;
	int i;
	long m;

	if (g_607cf0c)
		return;
	if (g_60794bc == 0)
		return;
	s = f_131e8();
	E.sum = s;
	o = OFS(&E.sum);
	f_30844(o, (unsigned char *)(&s), 2);
	f_2b4e8();
	for (i = 0; i < 19; i++) {
		m = 1;
		m <<= i;
		if (!(g_60794bc & m))
			continue;
			g_60794bc &= ~m;
		f_30844(OFS(E.t44[i]), (unsigned char *)(E.t44[i]), 8);
		if (i >= 16)
			f_30844(OFS(E.tdc[i - 16]), (unsigned char *)(E.tdc[i - 16]), 8);
		if (i >= 10 && i < 13)
			f_30844(OFS(&E.tf4[i - 10]), (unsigned char *)(&E.tf4[i - 10]), 2);
		f_2b4e8();
	}
}

void f_13378(void)
{
	g_60794bc = 0x7ffff;
	f_1326c();
}

int f_13380(void)
{
	unsigned short s;
	short o;
	int i;
	void *p;

	o = OFS(&E.sum);
	f_3068c(o, (unsigned char *)(&s), 2);
	E.sum = s;
	for (i = 0; i < 19; i++) {
		p = E.t44[i];
		f_3068c(OFS(p), (unsigned char *)(p), 8);
	}
	for (i = 0; i < 3; i++) {
		p = E.tdc[i];
		f_3068c(OFS(p), (unsigned char *)(p), 8);
	}
	for (i = 0; i < 3; i++) {
		p = &E.tf4[i];
		f_3068c(OFS(p), (unsigned char *)(p), 2);
	}
	if (s != f_131e8()) {
		f_11fe8();
		f_13378();
		return 0;
	}
	return 1;
}
