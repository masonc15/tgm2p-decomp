/* rom: 0x10b28 len: 0x26c func: f_10b28 flags: -macsave=1 -optimize=1 -speed */
/* HUD source file (0xf958-0x11518), span 0x10b28-0x10d94: f_10b28 draws the owner's w30c status labels
 * (strings at g_6033620..g_6033638) and the status icons.  Byte-exact compiled alone. */
struct frame { char pad[12]; };
struct pl {
	char pad0[0xdf];
	unsigned char bdf;         /* 0xdf */
	char pad1[0x2f4 - 0xe0];
	struct pl *owner;          /* 0x2f4 */
	char pad1b[0x308 - 0x2f8];
	unsigned long l308;        /* 0x308 */
	unsigned short w30c;       /* 0x30c */
	unsigned char b30e;        /* 0x30e */
	char pad2[0x314 - 0x30f];
	short pos[2];              /* 0x314 */
	short w318, w31a, w31c, w31e, w320;
	unsigned short w322;
	char pad3[0x330 - 0x324];
	long l330;                 /* 0x330 */
	char pad4[0x339 - 0x334];
	unsigned char b339;        /* 0x339 */
	char pad5[0x350 - 0x33a];
	unsigned long l350;        /* 0x350 */
	char pad6[0x358 - 0x354];
	unsigned long l358;        /* 0x358 */
	char pad7[0x379 - 0x35c];
	unsigned char b379;        /* 0x379 */
	char pad8[0x37e - 0x37a];
	unsigned char b37e;        /* 0x37e */
	char b37f;
	char b380;                 /* 0x380 */
	char pad9[0x38a - 0x381];
	unsigned short w38a;       /* 0x38a */
	unsigned short w38c;       /* 0x38c */
	unsigned short w38e;       /* 0x38e */
	unsigned short w390;       /* 0x390 */
	char padA[0x3b4 - 0x392];
};
struct pair { short a, b; };
extern char g_6033620[], g_6033628[], g_6033630[], g_6033638[];
extern struct pair d_36304[];
extern short d_3ae6c[];
extern unsigned short d_3ae4c[];
extern struct frame *d_3ae58[];
extern int f_e39c(char *);
extern void f_e490(int, int, char *, short, char);
extern void f_185f0(struct frame *, short, short, char, short);

void f_10b28(struct pl *p)
{
	short x, y, i;
	unsigned short m;

	x = 170;
	y = d_36304[p->b30e].a;
	if (p->owner->w30c & 1)
		f_e490((unsigned short)y - f_e39c(g_6033620) / 2, 155, g_6033620, 15, 0);
	if (p->owner->w30c & 2)
		f_e490((unsigned short)y - f_e39c(g_6033628) / 2, 155, g_6033628, 15, 0);
	if (p->owner->w30c & 0x80)
		f_e490((unsigned short)y - f_e39c(g_6033630) / 2, 155, g_6033630, 15, 0);
	if (p->owner->w30c & 0x1000)
		f_e490((unsigned short)y - f_e39c(g_6033638) / 2, 155, g_6033638, 15, 0);
	m = p->owner->w30c;
	if ((p->owner->w30c & 0x80) && !(p->owner->l308 & 0x10))
		m &= 0xfdbf;
	if ((p->owner->w30c & 0x1000) && !(p->owner->l308 & 0x10))
		m &= 0xf99f;
	for (i = 0; i < 5; i++) {
		if (d_3ae4c[i] & m) {
			f_185f0(d_3ae58[i], x, y - d_3ae6c[i], 0, 110);
			x += 8;
		}
	}
}
