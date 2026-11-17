/* rom: 0x15fae len: 0x44e func: f_15fae flags: -macsave=1 -optimize=1 -speed */
/* Non-matching, functionally equivalent C (shiftable build only).
 * f_15fae: draws a 4x4 piece sprite (k 0: active piece, 1: next-piece
 * preview, 2: ghost piece at the drop position from f_3f92) cell by cell
 * with f_11680, picking frame and palette from the piece flags. The ROM
 * keeps the y, x and palette arithmetic in 32 bits. */
struct frame { char pad[12]; };
struct point { short x, y; };
struct field {
	char pad0[0xde];
	unsigned char height;      /* 0x0de */
	unsigned char width;       /* 0x0df */
	short pos[2];              /* 0x0e0 */
	char pad1[0x30c - 0xe4];
	unsigned short mode;       /* 0x30c */
	unsigned char b30e;        /* 0x30e */
	char pad2[0x314 - 0x30f];
	short shake[2];            /* 0x314 */
	char pad3[0x347 - 0x318];
	signed char b347;          /* 0x347 */
	char pad4[0x35c - 0x348];
	union {
		unsigned short w;
		unsigned char b[2];
	} u35c;                    /* 0x35c */
	short w35e;                /* 0x35e */
	short w360;                /* 0x360 */
	char b362;                 /* 0x362 */
	char pad5;
	short p364[3];             /* 0x364 */
	char pad6[0x37f - 0x36a];
	unsigned char b37f;        /* 0x37f */
	unsigned char b380;        /* 0x380 */
};
extern void f_2af5e(int, int, void *);
extern void f_11680(struct frame *, int, int, int, int, int, int, int);
extern unsigned long f_3f92(struct field *, long);
extern long f_2beca(int);
extern unsigned long g_6064880;
extern char d_64310[], d_65310[], d_64b10[], d_63f10[], d_64710[];
extern char *d_3b37c[];
extern short d_3b340[];
extern short d_3b356[];
extern char d_363cc[][4][4][4];
extern struct frame d_a5e1c[], d_a5e28[], d_a5d5c[];

void f_15fae(struct field *f, unsigned char k, int draw)
{
	short x0, y0;
	char t;
	short v;
	char rot;
	short n, d, lvl;
	int cx, cy;
	short px, step, i, j, s;
	int py, c, x;
	long fl;
	short type;
	struct frame *frm;
	short pal;

	if (!draw)
		return;
	t = f->u35c.w;
	x0 = f->shake[0] + f->pos[0];
	y0 = f->shake[1] + f->pos[1];
	if (k == 1) {
		v = t == 8 ? f->w35e : f->w360;
		if ((g_6064880 & 2) && t == 10)
			rot = f->b362;
		else
			rot = 0;
		if (g_6064880 & 4)
			n = (short)((f->b30e == 0 ? 0x30000 : 0xa0000) >> 16);
		else
			n = 4;
		d = 0;
		if (v & 0x200) {
			d = 16;
			n--;
		}
		px = n * 8 + x0 - (f->width / 2) * 8;
		py = y0 - (f->height + 3) * 8 - d;
		c = 9;
	} else {
		v = f->w35e;
		rot = f->b362;
		cx = f->p364[0];
		cy = f->p364[2];
		if (v & 0x200) {
			cy++;
			cx -= 2;
		}
		px = cx * 8 + x0 - (f->width / 2) * 8;
		py = (f->height - cy - 2) * 8 + y0 - (f->height - 2) * 8 - 6;
		lvl = f->b347 / 6;
		if (f->b347 % 6 > 0)
			lvl++;
		if (lvl <= 0)
			lvl = 0;
		else if (lvl >= 4)
			lvl = 4;
		c = lvl + 4;
	}
	if (k == 2) {
		v = f->w35e;
		rot = f->b362;
		cx = f->p364[0];
		cy = (short)(f_3f92(f, 0x140000) >> 16);
		if (v & 0x200) {
			cy++;
			cx -= 2;
		}
		px = cx * 8 + x0 - (f->width / 2) * 8;
		py = (f->height - cy - 2) * 8 + y0 - (f->height - 2) * 8 - 6;
		c = 4;
	}
	fl = v;
	if (fl & 0x400)
		frm = d_a5e1c;
	else if (fl & 0x2000) {
		frm = d_a5e28;
		if (k == 1)
			frm += f->b380 - 1;
		else
			frm += f->b37f - 1;
	} else
		frm = d_a5d5c;
	if (fl & 0x2000)
		pal = d_3b356[(k == 1 ? f->b380 : f->b37f) - 1];
	else if (fl & 0x800)
		pal = 48;
	else if (fl & 0x400)
		pal = 0x80;
	else
		pal = d_3b340[(fl & 0x100) ? f_2beca(7) : (fl & 15) - 2];
	c += pal;
	if (fl & 0x200) {
		n = 2;
		s = 127;
	} else {
		n = 1;
		s = 63;
	}
	type = v & 15;
	step = n * 8;
	for (i = 0; i < 4; i++) {
		x = px;
		for (j = 0; j < 4; j++) {
			if (d_363cc[type][i][rot][j])
				f_11680(frm, py, x, c, 100, s, s, 0);
			x += step;
		}
		py += step;
	}
}
