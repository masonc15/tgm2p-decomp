/* rom: 0x13380 len: 0x104 func: f_13380 flags: -macsave=1 -optimize=1 -speed */
/* Non-matching: functionally equivalent C for [0x13380, 0x13484) (f_13380),
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

void f_1326c(void);

void f_13378(void);

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
		/* f_13378, which the original compiled inline */
		g_60794bc = 0x7ffff;
		f_1326c();
		return 0;
	}
	return 1;
}
