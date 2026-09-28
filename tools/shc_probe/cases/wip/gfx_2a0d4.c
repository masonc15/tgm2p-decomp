/* rom: 0x2a0d4 len: 0xb78 func: f_2a0d4 flags: -macsave=1 -optimize=1 -speed */
typedef struct { unsigned long c[16]; } LPAL;
struct rgb {
	short r, g, b;
};
struct pfade {
	short reload;              /* 0x00 */
	short timer;               /* 0x02 */
	short pal;                 /* 0x04 */
	unsigned short mode;       /* 0x06 */
	short step;                /* 0x08 */
	short pos;                 /* 0x0a */
	short max;                 /* 0x0c */
	short pad;                 /* 0x0e */
	LPAL from;                 /* 0x10 */
	LPAL to;                   /* 0x50 */
	struct rgb cur[16];        /* 0x90 */
	struct rgb delta[16];      /* 0xf0 */
};
typedef struct { short c[32]; } SPAL;

extern short g_60b13b8;
extern unsigned short g_6060000;
extern struct pfade g_606194c[];

void f_2a0d4(void)
{
	register SPAL *pals = (SPAL *)0x24040000;
	struct pfade *o;
	register unsigned short k;
	register unsigned short i;
	long buf[16];
	register long v;

	if (g_60b13b8)
		return;
	if (g_6060000 >= 30)
		return;
	for (o = g_606194c, k = 0; k < 32; k++, o++) {
		if (o->pal < 0)
			continue;
		if (--o->timer > 0)
			continue;
		o->timer = o->reload;
		if (o->step > 0) {
			for (i = 0; i < 16; i++) {
				o->cur[(char)i].r -= o->delta[(char)i].r;
				v = (o->cur[(char)i].r & 0xfc00) << 16;
				o->cur[(char)i].g -= o->delta[(char)i].g;
				v |= (o->cur[(char)i].g & 0xfc00) << 8;
				o->cur[(char)i].b -= o->delta[(char)i].b;
				v |= o->cur[(char)i].b & 0xfc00;
				*(long *)((int)buf + i * 4) = v;
			}
		} else {
			for (i = 0; i < 16; i++) {
				o->cur[(char)i].r += o->delta[(char)i].r;
				v = (o->cur[(char)i].r & 0xfc00) << 16;
				o->cur[(char)i].g += o->delta[(char)i].g;
				v |= (o->cur[(char)i].g & 0xfc00) << 8;
				o->cur[(char)i].b += o->delta[(char)i].b;
				v |= o->cur[(char)i].b & 0xfc00;
				*(long *)((int)buf + i * 4) = v;
			}
		}
		for (i = 0; i < 32; i++)
			pals[o->pal].c[i] = *(short *)((char *)buf + i * 2);
		o->pos += o->step;
		if (o->pos > o->max || o->pos < 0) {
			switch (o->mode) {
			case 0x40:
			case 0x44:
				o->mode = 0;
				o->pal = -1;
				o->pos = o->max;
				break;
			case 0x41:
				o->pos = 0;
				o->timer = o->reload;
				for (i = 0; i < 16; i++) {
					o->cur[(char)i].r = (o->from.c[i] & 0xfc000000) >> 16;
					o->cur[(char)i].g = (o->from.c[i] & 0xfc0000) >> 8;
					o->cur[(char)i].b = (short)o->from.c[i] & 0xfc00;
				}
				break;
			case 0x42:
				o->timer = o->reload;
				o->step *= -1;
				if (o->pos < 0)
					o->pos = 0;
				else
					o->pos = o->max;
				break;
			case 0x43:
				o->timer = o->reload;
				o->pos = o->max;
				for (i = 0; i < 16; i++) {
					o->cur[(char)i].r = (o->to.c[i] & 0xfc000000) >> 16;
					o->cur[(char)i].g = (o->to.c[i] & 0xfc0000) >> 8;
					o->cur[(char)i].b = (short)o->to.c[i] & 0xfc00;
				}
				break;
			}
		}
	}
}

#define PFX(i) (*(struct pfade *)((char *)g_606194c + (short)((i) * 0x150)))
#define PFP(i) (*(struct pfade *)((char *)p + (short)((i) * 0x150)))
#define PFO(i) (*(struct pfade *)((char *)o + (short)((i) * 0x150)))

void f_2a576(short pal, LPAL *from, LPAL *to, short reload, unsigned char mode, unsigned char step, unsigned char max)
{
	register unsigned short i;
	register unsigned short n;
	short m;
	struct pfade *p;
	struct pfade *o;
	register unsigned short f, t;

	p = g_606194c;
	n = 32;
	for (i = 0; i < 32; i++) {
		if (PFP(i).pal == pal) {
			n = i;
			break;
		}
	}
	if (n == 32) {
		for (i = 0, n = 32; i < 32; i++) {
			if (PFP(i).mode <= 0) {
				n = i;
				break;
			}
		}
	}
	PFX(n).pal = pal;
	PFX(n).timer = 0;
	PFX(n).mode = mode;
	PFX(n).max = m = max;
	switch (mode) {
	case 0x40:
	case 0x41:
	case 0x42:
		PFX(n).reload = reload;
		PFX(n).pos = 0;
		PFX(n).step = step;
		break;
	case 0x43:
	case 0x44:
		PFX(n).reload = reload;
		PFX(n).pos = m;
		PFX(n).step = -step;
		break;
	}
	o = &PFX(n);
	PFX(n).from = *from;
	PFX(n).to = *to;
	for (i = 0; i < 16; i++) {
		f = (o->from.c[i] & 0xfc000000) >> 16;
		t = (o->to.c[i] & 0xfc000000) >> 16;
		if (o->step > 0)
			o->cur[(char)i].r = f;
		else
			o->cur[(char)i].r = t;
		o->delta[(char)i].r = (f - t) * step >> 6;
		f = (o->from.c[i] & 0xfc0000) >> 8;
		t = (o->to.c[i] & 0xfc0000) >> 8;
		if (o->step > 0)
			o->cur[(char)i].g = f;
		else
			o->cur[(char)i].g = t;
		o->delta[(char)i].g = (f - t) * step >> 6;
		f = o->from.c[i] & 0xfc00;
		t = o->to.c[i] & 0xfc00;
		if (o->step > 0)
			o->cur[(char)i].b = f;
		else
			o->cur[(char)i].b = t;
		o->delta[(char)i].b = (f - t) * step >> 6;
	}
}

#define PF(i) (*(struct pfade *)((char *)p + (short)((i) * 0x150)))

void f_2ab56(unsigned short pal)
{
	struct pfade *p = g_606194c;
	register unsigned long i;

	if (pal > 0xff) {
		for (i = 0; i < 32; i++) {
			PF(i).mode = 0;
			PF(i).pal = -1;
		}
	} else {
		for (i = 0; i < 32; i++) {
			if (PF(i).pal == pal) {
				PF(i).pal = -1;
				PF(i).mode = 0;
				break;
			}
		}
	}
}

void f_2abce(void)
{
	g_60b13b8 = 1;
}

void f_2abd6(void)
{
	g_60b13b8 = 0;
}

struct task {
	void (*func)();
	long a;
	long b;
	long c;
};
extern struct task g_6064350[];
extern unsigned short g_606434c;
void f_2ac4c(long a, long b, long c);

/* Queues f_2ac4c(t << 24 | n << 16 | pal, from, to): blend n palettes. */
void f_2abde(unsigned char pal, unsigned char n, char t, long from, long to)
{
	g_6064350[g_606434c].func = f_2ac4c;
	g_6064350[g_606434c].a = t << 24 | n << 16 | pal;
	g_6064350[g_606434c].b = from;
	g_6064350[g_606434c++].c = to;
}
