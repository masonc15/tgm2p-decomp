/* rom: 0x7c08 len: 0x330 func: f_7c08 flags: -macsave=1 -optimize=1 -speed */
/* Non-matching, functionally equivalent C for ROM 0x7c08-0x7f38 (f_7c08):
 * the READY / GO start sequence (sprite slide-in, hold and slide-out states),
 * then piece preview update and IRS pre-rotation during the countdown. */
struct field {
	char pad0[0x2fc];
	unsigned char b2fc;        /* 0x2fc */
	char pad2fd[3];
	short w300;                /* 0x300 */
	short w302;                /* 0x302 */
	char pad304[0x30c - 0x304];
	unsigned short mode;       /* 0x30c */
	unsigned char id;          /* 0x30e */
	char pad30f[0x314 - 0x30f];
	short w314;                /* 0x314 */
	char pad316[0x362 - 0x316];
	unsigned char rotation;    /* 0x362 */
	char pad363[0x388 - 0x363];
	unsigned char b388;        /* 0x388 */
};
struct gs {
	unsigned char b0;
	char pad1[6];
	signed char b7;            /* 0x606488b */
	signed char b8;            /* 0x606488c */
};
extern struct gs g_6064884;
extern unsigned long g_6064880;
extern unsigned char g_606475a[];
extern char d_a6e0c[], d_a6e18[];
extern void f_2f68(struct field *, int);
extern void f_2efc(struct field *);
extern void f_11680(char *, int, int, int, int, int, int, int);
extern void f_1159c(char *, int, int, int, int);
extern void f_2e6fc(int);
extern void f_35a0(struct field *);

void f_7c08(struct field *f)
{
	unsigned char id = f->id;

	if (f->mode & 8) {
		if (!(g_6064880 & 0x8000)) {
			f->b388 = 0;
			f_2f68(f, 1);
			g_6064880 &= 0xfffff7ff;
		}
		goto done;
	}
	if (!(g_6064884.b8 == (signed char)f->id && g_6064884.b7 != 0)) {
		if (!(f->mode & 0x1005))
			f_2efc(f);
	}
	switch (f->b2fc) {
	case 0:
		f->b2fc++;
		f->w300 = 10;
		f->w302 = 70;
		/* fall through */
	case 1:
		f_11680(d_a6e0c, f->w302, f->w314, 0, 110, 63 - f->w300 * 5, 63, 0);
		if (--f->w300 != 0)
			goto slide;
		f->b2fc++;
		f->w300 = 30;
		f_2e6fc(29);
		break;
	case 2:
		f_1159c(d_a6e0c, f->w302, f->w314, 0, 110);
		if (--f->w300 == 0) {
			f->b2fc++;
			f->w300 = 10;
		}
		break;
	case 3:
		f_11680(d_a6e0c, f->w302, f->w314, 0, 110, f->w300 * 5 + 13, 63, 0);
		if (--f->w300 != 0)
			goto slide;
		f->b2fc++;
		f->w300 = 10;
		f->w302 = 70;
		break;
	case 4:
		f_11680(d_a6e18, f->w302, f->w314, 0, 110, 63 - f->w300 * 5, 63, 0);
		if (--f->w300 != 0)
			goto slide;
		f->b2fc++;
		f->w300 = 30;
		f_2e6fc(16);
		break;
	case 5:
		f_1159c(d_a6e18, f->w302, f->w314, 0, 110);
		if (--f->w300 == 0) {
			f->b2fc++;
			f->w300 = 10;
		}
		break;
	case 6:
		f_11680(d_a6e18, f->w302, f->w314, 0, 110, f->w300 * 5 + 13, 63, 0);
		if (--f->w300 != 0)
			goto slide;
		f->b388 = 0;
		f_2f68(f, 1);
		g_6064880 &= 0xfffff7ff;
		break;
	}
	goto done;
slide:
	f->w302 += 5;
done:
	f_35a0(f);
	if (!(g_6064880 & 2))
		return;
	if (f->w300 >= 70)
		return;
	if (!(g_606475a[id] & 14))
		return;
	if (g_606475a[id] & 10)
		f->rotation = (f->rotation + 1) % 4;
	else
		f->rotation = (f->rotation + 3) % 4;
}
