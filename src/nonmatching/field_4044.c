/* rom: 0x4044 len: 0x490 func: f_4044 flags: -macsave=1 -optimize=1 -speed */
/* Non-matching, functionally equivalent C for ROM 0x4044-0x44d4 (f_4044):
 * per-frame control of the active piece: debug level/mode buttons, rotation
 * input with IRS colour swap, then gravity speed selection and the gravity
 * step (f_3e2e). */
struct cell { unsigned short flags; unsigned char b2, b3; short w4; };
struct field {
	struct cell *cells;        /* 0x000 */
	char pad4[0x308 - 4];
	unsigned long flags;       /* 0x308 */
	unsigned short mode;       /* 0x30c */
	unsigned char id;          /* 0x30e */
	char pad30f[0x322 - 0x30f];
	unsigned short level;      /* 0x322 */
	char pad324[0x334 - 0x324];
	long l334;                 /* 0x334 */
	unsigned char b338;        /* 0x338 */
	unsigned char b339;        /* 0x339 */
	char pad33a[0x358 - 0x33a];
	long l358;                 /* 0x358 */
	unsigned short w35c;       /* 0x35c */
	short w35e;                /* 0x35e */
	unsigned short piece;      /* 0x360 */
	unsigned char rotation;    /* 0x362 */
	char pad363;
	short x;                   /* 0x364 */
	short pad366;
	short y;                   /* 0x368 */
	char pad36a[0x37c - 0x36a];
	short w37c;                /* 0x37c */
	char pad37e[0x38c - 0x37e];
	short w38c;                /* 0x38c */
	char pad38e[0x396 - 0x38e];
	short w396;                /* 0x396 */
};
extern unsigned char g_606475a[], g_606475e[];
extern signed char g_607cf0c;
extern unsigned char g_6079378[];
extern unsigned char g_6079284[], g_6079286[];
extern unsigned char d_3af9f[];
extern void f_8164(struct field *);
extern void f_31dc(struct field *, int);
extern void f_11926(struct field *);
extern void f_8314(struct field *);
extern unsigned char f_3a02(struct field *, int, int, int);
extern long f_2beca(int);
extern void f_3c88(struct field *);
extern long f_837a(struct field *);
extern long f_3f92(struct field *, long);
extern void f_3e2e(struct field *, long);

void f_4044(struct field *f)
{
	unsigned char id = f->id;
	short lock, irs, saved;
	unsigned char in;
	long g, d;

	if (f->w35c & 0x8000) {
		f_8164(f);
		f_31dc(f, 0);
		return;
	}
	f_11926(f);
	if (g_607cf0c != 0 && (g_606475e[id] & 1)) {
		if ((g_606475a[id] & 0xf0) == 32)
			f->level += 100;
		if ((g_606475a[id] & 0xf0) == 16) {
			f->w38c = 8;
			f->level = 0x383;
			f->l334 = 3600;
			f->b339 |= 3;
			g_6079378[(unsigned char)(f->id * 76)] = d_3af9f[16];
			f->b338 |= 51;
		}
		if ((g_606475a[id] & 0xf0) == 0x80) {
			f->w38c = 8;
			f->level = 0x383;
			f->l334 = 3600;
			f->b339 |= 3;
			g_6079378[(unsigned char)(f->id * 76)] = d_3af9f[17];
			f->b338 = 117;
		}
		if ((g_606475a[id] & 0xf0) == 64)
			f->b339 |= 8;
		if (f->flags & 0x200000) {
			f->flags &= 0xffdfffff;
		} else {
			f->flags |= 0x200000;
			f_8314(f);
		}
	}
	lock = f->w37c & 2;
	irs = f->w35e & 0x100;
	in = g_606475e[id];
	if (((in & 8) || (in & 2) || lock) &&
	    !f_3a02(f, f->x, f->y, (f->rotation + 1) % 4)) {
		if (irs) {
			saved = f->w35e;
			f->w35e &= 0xfff0;
			f->w35e |= f_2beca(7) + 2;
			if (f_3a02(f, f->x, f->y, (signed char)f->rotation))
				f->w35e = saved;
			if (!f_3a02(f, f->x, f->y, (f->rotation + 1) % 4))
				f->rotation = (f->rotation + 1) % 4;
		} else {
			f->rotation = (f->rotation + 1) % 4;
		}
		f->w396++;
	} else if (g_606475e[id] & 4) {
		if (!f_3a02(f, f->x, f->y, (f->rotation + 3) % 4)) {
			if (irs) {
				saved = f->w35e;
				f->w35e &= 0xfff0;
				f->w35e |= f_2beca(7) + 2;
				if (f_3a02(f, f->x, f->y, (signed char)f->rotation))
					f->w35e = saved;
				if (!f_3a02(f, f->x, f->y, (f->rotation + 3) % 4))
					f->rotation = (f->rotation + 3) % 4;
			} else {
				f->rotation = (f->rotation + 3) % 4;
			}
			f->w396++;
		}
	}
	if (lock)
		f->w37c &= 0xfffd;
	f_3c88(f);
	g = f_837a(f);
	f->l358 = g;
	if ((g_606475a[f->id] & 0xf0) == 64) {
		if ((unsigned long)g < 0x10000)
			g = 0x10000;
		g_6079284[f->id]++;
	} else if ((g_606475a[f->id] & 0xf0) == 0x80) {
		g = 0x140000;
		d = f->y - (short)((unsigned long)f_3f92(f, 0x140000) >> 16);
		if (d < 0)
			d = 0;
		if (d > g_6079286[f->id])
			g_6079286[f->id] = d;
	}
	if ((f->mode & 32) || ((f->flags & 32) && (f->mode & 1))) {
		f->l358 = 0x140000;
		g = 0x140000;
	}
	f_3e2e(f, g);
}
