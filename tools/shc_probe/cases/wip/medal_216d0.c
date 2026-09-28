/* rom: 0x216d0 len: 0x23c func: f_216d0 flags: -macsave=1 -optimize=1 -speed */
/* NOT FINAL: 99.3% (316/318 halfwords). f_217a2 (SK medal) and f_21862 (CO
 * medal) match; f_216d0 has the right code and registers but its first two
 * pool words come out swapped (ROM: d_3b55c, d_3b58c; here d_3b58c first,
 * because the user variable t is assigned before the loop, while in the ROM
 * the d_3b58c load looks compiler-hoisted into the loop preheader after the
 * d_3b55c induction pointer). Without t, SHC keeps y in r8 and spills the
 * table instead. */
struct medal {
	unsigned char changed[4];  /* 0x0 */
	unsigned char grade;       /* 0x4 */
	char pad[7];
};
struct medals {
	struct medal m[6];         /* AC ST SK RE RO CO */
};
struct field {
	char pad0[0x30c];
	unsigned short mode;       /* 0x30c */
	unsigned char id;          /* 0x30e */
	char pad1[0x33c - 0x30f];
	unsigned char sk_grade;    /* 0x33c */
	char pad2[0x33f - 0x33d];
	unsigned char co_grade;    /* 0x33f */
	unsigned char skills;      /* 0x340 */
	char pad3[0x343 - 0x341];
	signed char co;            /* 0x343 */
};
struct icon {
	long a;
	long b;
	signed char c;
	signed char e;
	signed char d;
	char padb;
};
struct frame;
extern struct medals g_607cd48[];
extern struct frame *d_3b55c[];
extern struct icon d_3b58c[];
extern signed char d_3b5bc[], d_3b5bf[];
extern signed char d_3b5c2[], d_3b5c5[];
extern void f_1159c(struct frame *, short, short, short, short);

#define MEDALS(f) ((struct medals *)((char *)g_607cd48 + (unsigned char)((f)->id * 72)))

void f_216d0(unsigned short v, short y, int x)
{
	short i;
	struct icon *t;

	t = d_3b58c;
	for (i = 0; i < 6; i++)
		f_1159c(d_3b55c[i], y, x + i * 16,
			((struct icon *)((char *)t + (signed char)(((v >> (i * 2)) & 3) * 12)))->e, 110);
}

void f_217a2(struct field *f)
{
	signed char n;

	f->skills++;
	if (f->sk_grade < 3) {
		if (f->mode & 0x40)
			n = d_3b5bf[f->sk_grade];
		else if (f->mode & 0x1000)
			n = d_3b5bc[f->sk_grade] >> 1;
		else
			n = d_3b5bc[f->sk_grade];
		if (n <= f->skills) {
			f->sk_grade++;
			MEDALS(f)->m[2].grade = f->sk_grade;
			MEDALS(f)->m[2].changed[0] = 1;
		}
	}
}

void f_21862(struct field *f)
{
	unsigned char n;

	for (n = 0; n < 3; n++) {
		if (f->mode & 0x40) {
			if (d_3b5c5[n] == f->co)
				break;
		} else if (d_3b5c2[n] == f->co)
			break;
	}
	if (n < 3) {
		n++;
		MEDALS(f)->m[5].grade = n;
		MEDALS(f)->m[5].changed[0] = 1;
		if (f->co_grade < n)
			f->co_grade = n;
	}
}
