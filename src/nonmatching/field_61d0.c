/* rom: 0x61d0 len: 0xe34 func: f_61d0 flags: -macsave=1 -optimize=1 -speed */
/* Non-matching, functionally equivalent C for ROM 0x61d0-0x7004.
 * f_61d0: spawn the next piece (randomizer with 4-piece history, IRS, top-out
 * handling); f_667c: re-place the current piece (IRS, gravity, task step);
 * f_67f4: rising-garbage state machine (state 0 shifts the playfield up one
 * row and inserts the next queued row, states 1/2 count down timers). */
struct cell { unsigned short flags; unsigned char b2, b3; short w4; };
struct field {
	struct cell *cells;        /* 0x000 */
	short rows[8][12];         /* 0x004 */
	char pad_c4[0xde - 0xc4];
	unsigned char height;      /* 0x0de */
	unsigned char width;       /* 0x0df */
	char pad_e0[0x2f4 - 0xe0];
	struct field *owner;       /* 0x2f4 */
	char pad2f8[0x2fe - 0x2f8];
	unsigned char b2fe;        /* 0x2fe */
	char pad2ff;
	short w300;                /* 0x300 */
	short w302;                /* 0x302 */
	short w304;                /* 0x304 */
	char pad306[2];
	unsigned long flags;       /* 0x308 */
	unsigned short mode;       /* 0x30c */
	unsigned char id;          /* 0x30e */
	char pad30f[0x322 - 0x30f];
	unsigned short level;      /* 0x322 */
	short w324;
	short w326;                /* 0x326 */
	char pad328[0x35c - 0x328];
	union {
		unsigned short w;      /* 0x35c */
		struct { unsigned char b35c, b35d; } b;
	} u35c;
	short w35e;                /* 0x35e */
	unsigned short piece;      /* 0x360 */
	unsigned char rotation;    /* 0x362 */
	char pad363;
	union { long l; short s[2]; } px;  /* 0x364 */
	union { long l; short s[2]; } py;  /* 0x368 */
	unsigned char hist[4];     /* 0x36c */
	long remaining;            /* 0x370 */
	char pad374[0x379 - 0x374];
	signed char b379;          /* 0x379 */
	char pad37a[2];
	unsigned short w37c;       /* 0x37c */
	unsigned char delay;       /* 0x37e */
	unsigned char b37f;        /* 0x37f */
	unsigned char b380;        /* 0x380 */
	char pad381[0x388 - 0x381];
	signed char b388;          /* 0x388 */
	char pad389[0x396 - 0x389];
	short w396;                /* 0x396 */
};
struct g7608 { char pad[0x370]; short w370; };
extern struct g7608 g_6077608;
extern struct field g_6064898, g_6064c4c;
extern unsigned long g_6064880;
extern long g_606487c;
extern unsigned char g_606475a[];
extern short d_38f6a[];
extern short d_3a7bc[];
extern unsigned char f_601a(struct field *);
extern void f_60e8(struct field *);
extern unsigned char f_35f0(struct field *, int, int, int);
extern void f_20c14(struct field *);
extern void f_44d4(struct field *, int);
extern void f_331a(struct field *);
extern long f_837a(struct field *);
extern long f_3f92(struct field *, long);
extern void f_2fc0(struct field *);
extern void f_35a0(struct field *);
extern void f_2e6fc(int);
extern void f_8518(struct field *);
extern void f_2f68(struct field *, int);

void f_61d0(struct field *f)
{
	unsigned char id = f->id;
	unsigned char tries, piece, rot;
	short i, n;
	long g;

	if (f->level > d_38f6a[0])
		f->level = d_38f6a[0];
	f->w326++;
	if (f->delay < 20 && f->b379 == 0)
		f->delay++;
	if (f->piece & 0x2000)
		f->delay = 0;
	f->w35e = f->piece;
	f->b37f = f->b380;
	f->b380 = 0;
	tries = g_606487c ? 6 : 5;
	while (tries) {
		tries--;
		piece = f_601a(f);
		n = 0;
		for (i = 0; i < 4; i++)
			if (f->hist[i] == piece)
				n++;
		if (n > 0)
			piece = f_601a(f);
		else
			tries = 0;
	}
	for (i = 3; i > 0; i--)
		f->hist[i] = f->hist[i - 1];
	f->hist[0] = piece;
	f->piece = piece + 2;
	if ((f->w37c & 1) | (f->mode & 64))
		f->piece |= 0x200;
	if ((f->mode & 0x200) && f->remaining == 0)
		f->remaining = 0x7ffff;
	if (!(f->mode & 0x900))
		f_60e8(f);
	f->rotation = 0;
	if (g_6064880 & 4)
		f->px.l = id == 0 ? 0x30000 : 0xa0000;
	else
		f->px.l = 0x40000;
	if (f->mode & 64)
		f->px.s[0]++;
	f->py.l = 0x15ffff;
	if (g_606475a[id] & 14) {
		if (g_606475a[id] & 10)
			rot = (f->rotation + 1) % 4;
		else
			rot = (f->rotation + 3) % 4;
		f->w396++;
		if (!f_35f0(f, f->px.s[0], f->py.s[0], rot)) {
			f->rotation = rot;
			f_2e6fc(18);
		}
	}
	if (f_35f0(f, f->px.s[0], f->py.s[0], (signed char)f->rotation)) {
		if (g_6064880 & 4) {
			if (g_6077608.w370 == 0) {
				f_2f68(f, 8);
			} else if ((f->flags & 32) && (f->owner->flags & 32)) {
				g_6064898.w302 = 1;
				g_6064898.w304 = 0;
				f_20c14(&g_6064898);
				f_44d4(&g_6064898, 2);
				f_44d4(&g_6064c4c, 2);
				g_6064898.flags &= 0xfffdbfff;
				g_6064898.flags |= 0x50000;
				g_6064898.flags |= 0x80000040;
				g_6064c4c.flags &= 0xfffdbfff;
				g_6064c4c.flags |= 0x50000;
				g_6064c4c.flags |= 0x80000000;
			} else {
				f_44d4(f, 0);
				f_331a(f);
			}
		} else if (g_6064880 & 2) {
			f_44d4(f, 0);
			f->flags &= 0xfffdbfff;
			if (f->id == 0)
				g_6064880 |= 0x200;
			else
				g_6064880 |= 0x100;
		} else if ((f->flags & 32) && (f->mode & 0x1081)) {
			f->w302 = 1;
			f->w304 = 0;
			f_20c14(f);
			f_44d4(f, 0);
			f->flags &= 0xfffdbfff;
			f->flags |= 0x50000;
			f->flags |= 0x80000040;
		} else {
			f_44d4(f, 0);
			f->flags &= 0xfffdbfff;
			f_331a(f);
		}
	} else {
		g = f_837a(f);
		if ((f->mode & 32) || ((f->flags & 32) && (f->mode & 1)))
			g = 0x140000;
		if ((short)((unsigned long)g >> 16) >= 1)
			f->py.l = f_3f92(f, g);
		f_2fc0(f);
	}
	if (f->u35c.b.b35d == 8)
		return;
	f_2e6fc((f->piece & 0x2000) ? 1 : d_3a7bc[(f->piece & 15) - 2]);
}

void f_667c(struct field *f)
{
	unsigned char id = f->id;
	unsigned char rot;
	long g;

	if (f_35f0(f, f->px.s[0], f->py.s[0], (signed char)f->rotation)) {
		if (g_6077608.w370 != 0) {
			f_44d4(f, 0);
			f_331a(f);
		}
		return;
	}
	f_2e6fc((f->piece & 0x2000) ? 1 : d_3a7bc[(f->piece & 15) - 2]);
	if (g_606475a[id] & 14) {
		if (g_606475a[id] & 10)
			rot = (f->rotation + 1) % 4;
		else
			rot = (f->rotation + 3) % 4;
		f->w396++;
		if (!f_35f0(f, f->px.s[0], f->py.s[0], rot)) {
			f->rotation = rot;
			f_2e6fc(18);
		}
	}
	g = f_837a(f);
	if ((f->mode & 32) || ((f->flags & 32) && (f->mode & 1)))
		g = 0x140000;
	if ((short)((unsigned long)g >> 16) >= 1)
		f->py.l = f_3f92(f, g);
	f_2fc0(f);
}

void f_67f4(struct field *f)
{
	short y, x, i;

	f_35a0(f);
	switch (f->b2fe) {
	case 0:
		for (y = 21; y > 1; y--)
			for (x = 1; x < 11; x++)
				f->cells[y * f->width + x] = f->cells[(y - 1) * f->width + x];
		for (x = 1; x < 11; x++) {
			if ((f->mode & 2) && (f->mode & 16)) {
				if ((unsigned short)f->rows[0][x - 1] & 15) {
					f->cells[f->width + x].flags = (f->rows[0][x - 1] & 15) | 0x1000;
					f->cells[f->width + x].w4 = 180;
				} else {
					f->cells[f->width + x].flags = f->rows[0][x - 1] & 15;
				}
			} else {
				f->cells[f->width + x].flags = f->rows[0][x - 1] & 15;
			}
			f->cells[f->width + x].b3 = 0;
			f->cells[f->width + x].b2 = 0;
		}
		for (y = 0; y < 7; y++)
			for (i = 0; i < 10; i++)
				f->rows[y][i] = f->rows[y + 1][i];
		for (i = 0; i < 10; i++)
			f->rows[7][i] = 0;
		f->b388--;
		f->b2fe = 1;
		f->w300 = 4;
		f_2e6fc(18);
		break;
	case 1:
		if (--f->w300 == 0) {
			if (f->b388 != 0) {
				f->b2fe = 0;
			} else {
				f->b2fe = 2;
				f->w300 = 15;
				f_8518(f);
			}
		}
		break;
	case 2:
		if (--f->w300 == 0) {
			f_2f68(f, 12);
			f->u35c.w &= 0x7fff;
		}
		break;
	}
}
