/* rom: 0x255dc len: 0xd80 func: f_255dc flags: -macsave=1 -optimize=1 -speed */
struct field {
	short w0;
	short w2;
	short w4;
	short w6;
	short w8;
	short w10;
	short w12;
	short w14;
	short w16;
	char pad18[2];
	long l20;
	short a24[11];
	char pad46[2];
	void (*cb48)();
	void (*cb52)();
	short a56[4];
	long l64;
	long l68;
	short w72;
	char pad74[2];
	long l76;
	long x80[224];
	long x976[224];
	char pad1872[0xe50 - 1872];
};
struct anim {
	long flags;
	short w4;
	short w6;
};
struct animent {
	struct anim *a;
	long n;
};
struct rec104 {
	short w0;
	char pad2[2];
	long *l4[2];
	short w12[2];
	struct animent *ae;
	long l20;
	long l24[2];
	long l32[2];
	long l40[2];
	short w48[2];
	short w52[2];
	short w56[2];
	short w60[2];
	short w64[2];
	short w68;
	short w70[2];
	short w74;
	long l76;
	long l80;
	short w84;
	short w86;
	short w88;
	short w90[2];
	char pad94[2];
	long l96;
	long l100;
};
struct cell {
	void (*fn)();
	long a4, a8, a12, a16, a20;
};
struct qent {
	long fn;
	long pad[3];
};
extern struct field g_60ad6a0[4];
extern void (*f_f928(short))();
extern char *g_606005c;
extern short g_60b13b0, g_60b13b2, g_60b13ae;
struct tsk {
	char pad0[8];
	short w8;
	char pad10[6];
	short w16;
};
#define REC(i) ((struct field *)((char *)g_60ad6a0 + (short)((i) * (short)sizeof(struct field))))
extern struct rec104 g_60ad228[11];
extern struct cell g_60b0fe0[2][20];
extern short g_60b13a4;
extern short g_60b13a0[2];
extern unsigned short g_6060000;
extern unsigned short g_606434c;
extern struct qent g_6064350[];
void f_259ea(void);
void f_25806(void);
short f_26098(void);

void f_26e18(short, struct animent *, struct rec104 *, short, short, short, unsigned long, long);
void f_26cb0(short, struct rec104 *, short, short, short);
struct animent *f_2722c(short, short, short *, short *);
void f_26ff0(short, short, short);
void f_26f64(short, int, long *);
void f_274c0(short, short, short, long, long, long);
void f_27310();
extern int g_dummy;


void f_255dc(short i)
{
	int j;
	int k;
	g_60ad6a0[i].w2 = 0;
	g_60ad6a0[i].w4 = 1;
	g_60ad6a0[i].w6 = 0;
	g_60ad6a0[i].w10 = 3;
	g_60ad6a0[i].w12 = 0;
	g_60ad6a0[i].w14 = 0;
	g_60ad6a0[i].w16 = 0;
	g_60ad6a0[i].l20 = 0;
	g_60ad6a0[i].cb48 = 0;
	g_60ad6a0[i].cb52 = 0;
	g_60ad6a0[i].w8 = 0;
	g_60ad6a0[i].l64 = 0;
	g_60ad6a0[i].l68 = 0;
	g_60ad6a0[i].l76 = 0;
	for (j = 0; j < 11; j++)
		g_60ad6a0[i].a24[j] = -1;
	for (k = 0; k < 4; k++)
		g_60ad6a0[i].a56[k] = 0;
}

void f_25658(void)
{
	int i, j;

	*(char *)0x2405fff8 = 10;
	*(char *)0x2405fff9 = 12;
	*(char *)0x2405fffa = 14;
	*(char *)0x2405fffb = 16;
	*(char *)0x2405fffe = 0;
	*(char *)0x2405ffff = 0;
	*(char *)0x2405ffeb |= 0x40;
	g_60b13a4 = 0;
	for (i = 0; i < 4; i++) {
		g_60ad6a0[i].w0 = 0;
		f_255dc(i);
	}
	for (i = 0; i < 11; i++) {
		g_60ad228[i].w0 = 0;
		for (j = 0; j < 2; j++) {
			g_60ad228[i].l4[j] = (long *)(0x24005000 + i * 4096 + j * 2048);
			g_60ad228[i].w12[j] = j + i * 2 + 10;
		}
		g_60ad228[i].ae = 0;
		g_60ad228[i].l20 = 0;
		g_60ad228[i].l96 = 0;
		g_60ad228[i].l100 = 0;
	}
	for (i = 0; i < 2; i++) {
		for (j = 0; j < 20; j++) {
			g_60b0fe0[i][j].fn = 0;
			g_60b0fe0[i][j].a4 = 0;
			g_60b0fe0[i][j].a8 = 0;
			g_60b0fe0[i][j].a12 = 0;
			g_60b0fe0[i][j].a16 = 0;
			g_60b0fe0[i][j].a20 = 0;
		}
	}
}

void f_257f8(void)
{
	if (g_6060000 > 0)
		return;
	f_25806();
}

void f_25806(void)
{
	int i;
	short b;

	b = (g_60b13a4 == 0) ? 1 : 0;
	for (i = 0; i < 20; i++) {
		if (g_60b0fe0[b][i].fn) {
			g_60b0fe0[b][i].fn(g_60b0fe0[b][i].a4, g_60b0fe0[b][i].a8, g_60b0fe0[b][i].a12, g_60b0fe0[b][i].a16, g_60b0fe0[b][i].a20);
			g_60b0fe0[b][i].fn = 0;
		}
	}
	g_60b13a0[b] = 0;
}

void f_25938(short i, long *a, long *b)
{
	int j;
	long *p, *q;

	p = g_60ad6a0[i].x80;
	q = g_60ad6a0[i].x976;
	for (j = 0; j < 224; j += 4) {
		*a++ = *p++;
		*a++ = *p++;
		*a++ = *p++;
		*a++ = *p++;
		*b++ = *q++;
		*b++ = *q++;
		*b++ = *q++;
		*b++ = *q++;
	}
}

void f_259bc(void)
{
	if (g_6060000 > 0)
		return;
	g_60b13a4 = (g_60b13a4 == 0) ? 1 : 0;
	g_6064350[g_606434c++].fn = (long)f_259ea;
}

void f_259ea(void)
{
	register short k;
	struct rec104 *r13;
	struct field *f;
	struct rec104 *r11;
	short old;
	char o0, f0, f1;
	int o1, o2, o3;

	f0 = f1 = 0;
	f = &g_60ad6a0[0];
	k = f->w6;
	if (f->w0 == 1 && f->a24[k] >= 0) {
		r11 = &g_60ad228[f->w8];
		r13 = &g_60ad228[f->a24[k]];
		k = r13->w84;
		old = k;
		if (r13->w86) {
			r13->w84 = (old + 1) & 1;
			r13->w86 = 0;
		} else {
			k = (old + 1) & 1;
		}
		o0 = r11->w12[k];
		if (f->w16) {
			o0 |= 0x80;
			f_25938(0, r11->l4[k], r11->l4[k] + 256);
		} else {
			*(long *)((char *)r11->l4[k] + 1008) = ((unsigned char)r13->l24[old] << 16) | (r13->l32[old] & 0x1ff);
			*(long *)((char *)r11->l4[k] + 2032) = (f->w10 << 24) | (unsigned short)r13->w12[k] | ((unsigned short)f->w14 << 8) | ((unsigned short)f->w12 << 15);
		}
		if (!(r13->ae->a->flags & 0x200000))
			f0 |= 64;
		if (f->w2)
			f0 |= 128;
	} else {
		o0 = 10;
	}
	f = &g_60ad6a0[1];
	k = f->w6;
	if (f->w0 == 1 && f->a24[k] >= 0) {
		r11 = &g_60ad228[f->w8];
		r13 = &g_60ad228[f->a24[k]];
		k = r13->w84;
		old = k;
		if (r13->w86) {
			r13->w84 = (old + 1) & 1;
			r13->w86 = 0;
		} else {
			k = (old + 1) & 1;
		}
		o1 = r11->w12[k];
		if (f->w16) {
			o1 |= 0x80;
			f_25938(1, r11->l4[k], r11->l4[k] + 256);
		} else {
			*(long *)((char *)r11->l4[k] + 1012) = ((unsigned char)r13->l24[old] << 16) | (r13->l32[old] & 0x1ff);
			*(long *)((char *)r11->l4[k] + 2036) = (f->w10 << 24) | (unsigned short)r13->w12[k] | ((unsigned short)f->w14 << 8) | ((unsigned short)f->w12 << 15);
		}
		if (!(r13->ae->a->flags & 0x200000))
			f0 |= 4;
		if (f->w2)
			f0 |= 8;
	} else {
		o1 = 10;
	}
	f = &g_60ad6a0[2];
	k = f->w6;
	if (f->w0 == 1 && f->a24[k] >= 0) {
		r11 = &g_60ad228[f->w8];
		r13 = &g_60ad228[f->a24[k]];
		k = r13->w84;
		old = k;
		if (r13->w86) {
			r13->w84 = (old + 1) & 1;
			r13->w86 = 0;
		} else {
			k = (old + 1) & 1;
		}
		o2 = r11->w12[k];
		if (f->w16) {
			o2 |= 0x80;
			f_25938(2, r11->l4[k], r11->l4[k] + 256);
		} else {
			*(long *)((char *)r11->l4[k] + 1016) = ((unsigned char)r13->l24[old] << 16) | (r13->l32[old] & 0x1ff);
			*(long *)((char *)r11->l4[k] + 2040) = (f->w10 << 24) | (unsigned short)r13->w12[k] | ((unsigned short)f->w14 << 8) | ((unsigned short)f->w12 << 15);
		}
		if (!(r13->ae->a->flags & 0x200000))
			f1 |= 64;
		if (f->w2)
			f1 |= 128;
	} else {
		o2 = 10;
	}
	f = &g_60ad6a0[3];
	k = f->w6;
	if (f->w0 == 1 && f->a24[k] >= 0) {
		r11 = &g_60ad228[f->w8];
		r13 = &g_60ad228[f->a24[k]];
		k = r13->w84;
		old = k;
		if (r13->w86) {
			r13->w84 = (old + 1) & 1;
			r13->w86 = 0;
		} else {
			k = (old + 1) & 1;
		}
		o3 = r11->w12[k];
		if (f->w16) {
			o3 |= 0x80;
			f_25938(3, r11->l4[k], r11->l4[k] + 256);
		} else {
			*(long *)((char *)r11->l4[k] + 1020) = ((unsigned char)r13->l24[old] << 16) | (r13->l32[old] & 0x1ff);
			*(long *)((char *)r11->l4[k] + 2044) = (f->w10 << 24) | (unsigned short)r13->w12[k] | ((unsigned short)f->w14 << 8) | ((unsigned short)f->w12 << 15);
		}
		if (!(r13->ae->a->flags & 0x200000))
			f1 |= 4;
		if (f->w2)
			f1 |= 8;
	} else {
		o3 = 10;
	}
	*(char *)0x2405fff8 = o0;
	*(char *)0x2405fff9 = o1;
	*(char *)0x2405fffa = o2;
	*(char *)0x2405fffb = o3;
	*(char *)0x2405fffe = f0;
	*(char *)0x2405ffff = f1;
}

short f_25f6e(void)
{
	int i;

	for (i = 0; i < 4; i++)
		if (g_60ad6a0[i].w0 == 0)
			break;
	if (i >= 4)
		return -1;
	f_255dc(i);
	g_60ad6a0[i].w0 = 1;
	return i;
}

void f_25fde(short i)
{
	int j;

	g_60ad6a0[i].w0 = 0;
	for (j = 0; j < 11; j++)
		if (g_60ad6a0[i].a24[j] > 0)
			g_60ad228[g_60ad6a0[i].a24[j]].w0 = 0;
}

short f_26098(void)
{
	int i, j;

	for (i = 1; i < 11; i++)
		if (g_60ad228[i].w0 == 0)
			break;
	if (i >= 11)
		return -1;
	g_60ad228[i].w0 = 1;
	g_60ad228[i].ae = 0;
	g_60ad228[i].l96 = 0;
	g_60ad228[i].w84 = 0;
	g_60ad228[i].w86 = 0;
	g_60ad228[i].l76 = 0;
	g_60ad228[i].l80 = 0;
	g_60ad228[i].w88 = 0;
	for (j = 0; j < 2; j++) {
		g_60ad228[i].l24[j] = 0;
		g_60ad228[i].l32[j] = 0;
		g_60ad228[i].l40[j] = 0;
		g_60ad228[i].w48[j] = 0;
		g_60ad228[i].w52[j] = 0;
		g_60ad228[i].w56[j] = 0;
		g_60ad228[i].w60[j] = 0;
		g_60ad228[i].w64[j] = 0;
		g_60ad228[i].w70[j] = 0;
	}
	return i;
}

void f_26156(short i)
{
	g_60ad228[i].w0 = 0;
}

short f_2616a(short i, struct animent *ae)
{
	struct field *f = &g_60ad6a0[i];
	register short id = f->w6;
	struct rec104 *l;
	register long d;

	if (f->a24[id] == -1)
		f->a24[id] = f_26098();
	id = f->a24[id];
	l = &g_60ad228[id];
	l->w88 = 1;
	d = f->l64;
	l->l40[0] -= d;
	l->l40[1] -= d;
	f->w72 = 1;
	if (l->ae != (struct animent *)l->l96) {
		l->l96 = (long)l->ae;
		l->l100 = l->l20;
	}
	l->ae = ae;
	l->l20 = 0;
	while (ae->a) {
		l->l20 += ae->a->w6 * ae->n;
		ae++;
	}
	l->l76 = 0;
	l->l80 = 0;
	return id;
}

void f_2622c(short i, struct animent *ae, long dx, long dy)
{
	short id = f_2616a(i, ae);

	g_60ad228[id].l76 += dx;
	g_60ad228[id].l80 += dy;
}

short f_26264(short i, struct animent *ae)
{
	short id;
	struct animent *e;
	short x;
	struct field *f;
	short *p;
	char *b;
	int k;
	if (g_60ad6a0[i].a24[g_60ad6a0[i].w6] < 0)
		id = g_60ad6a0[i].a24[g_60ad6a0[i].w6] = f_26098();
	else
		id = g_60ad6a0[i].a24[g_60ad6a0[i].w6];
	f = &g_60ad6a0[i];
	p = f->a24;
	x = *p;
	if (0) f->a24[3] = x;
	g_60ad228[id].ae = ae;
	g_60ad228[id].l20 = 0;
	for (e = ae; e->a; e++)
		g_60ad228[id].l20 += e->a->w6 * e->n;
	g_60ad228[id].l76 = 0;
	g_60ad228[id].l80 = 0;
	return id;
}

void f_2631a(short i, struct animent *ae, long dx, long dy)
{
	short id = f_26264(i, ae);

	g_60ad228[id].l76 += dx;
	g_60ad228[id].l80 += dy;
}
