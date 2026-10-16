/* rom: 0x1326c len: 0x10c func: f_1326c flags: -macsave=1 -optimize=1 -speed */
/* Non-matching: functionally equivalent C for [0x1326c, 0x13378) (f_1326c),
 * linked only by the shiftable build (splits_nonmatching.txt). Not matching coverage.
 * From src/save_checksum_131e8.c. */
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

unsigned short f_131e8(void);

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

void f_13378(void);

int f_13380(void);
