/* rom: 0x1199c len: 0x4fc func: f_1199c flags: -macsave=1 -optimize=1 -speed */
union gp {
	short w;
	struct {
		char lv;
		unsigned char pts;
	} b;
};
struct sect {
	union gp cur;          /* 0x00 */
	union gp best;         /* 0x02 */
	union gp sec[10];      /* 0x04 */
	union gp start;        /* 0x18 */
	short pad26;
	unsigned long time[10]; /* 0x1c */
	unsigned long t68;      /* 0x44 */
	char b72;               /* 0x48 */
	char pad73[3];
};
struct fld {
	char pad0[0x308];
	long l308;                 /* 0x308 */
	unsigned short w30c;       /* 0x30c */
	unsigned char b30e;        /* 0x30e */
	char pad1[0x322 - 0x30f];
	unsigned short w322;       /* 0x322 */
	char pad2[0x334 - 0x324];
	unsigned long l334;        /* 0x334 */
	unsigned char b338;        /* 0x338 */
	unsigned char b339;        /* 0x339 */
	char pad3[0x350 - 0x33a];
	unsigned long l350;        /* 0x350 */
	unsigned long l354;        /* 0x354 */
	char pad4[0x38a - 0x358];
	unsigned short w38a;       /* 0x38a */
	unsigned short w38c;       /* 0x38c */
	char pad5[0x3a1 - 0x38e];
	char b3a1;                 /* 0x3a1 */
};
extern struct sect g_6079378[];
struct tm { unsigned long time[10]; char pad[36]; }; /* section times at 0x6079394 */
extern char g_6079394[];

extern long g_6064880;
extern unsigned char g_6066198;
extern short g_6060038[];
extern long g_6065750[], g_6065778[];
extern unsigned char d_3af9f[];
extern short d_38f58[];
extern void f_2e6fc(int);
extern void f_2190c(struct fld *, unsigned long, long);

#define TIME(p) (SECT(p)->time)
#define SECT(p) ((struct sect *)((char *)g_6079378 + (unsigned char)((p)->b30e * 76)))

void f_1199c(struct fld *p)
{
	if (!(p->l308 & 32) && !(p->w30c & 0x1080)) {
		for (;;) {
			if (SECT(p)->cur.b.lv > d_3af9f[p->w38a] && p->w38a < 17) {
				p->w38a++;
				p->l354 = p->l350;
				if (!(p->w30c & 13))
					f_2e6fc(22);
			} else {
				break;
			}
		}
	}
}

int f_11a20(struct fld *p)
{
	struct sect *s = SECT(p);
	union gp t;
	int k;

	if (p->w30c != 2 && p->w30c != 0x1000)
		return;
	t = s->cur;
	if (t.w < s->start.w) {
		s->sec[p->w38c].w = 0;
	} else {
		if (t.b.pts < s->start.b.pts) {
			t.b.lv--;
			t.b.pts += 100;
		}
		s->sec[p->w38c].b.lv = t.b.lv - s->start.b.lv;
		s->sec[p->w38c].b.pts = t.b.pts - s->start.b.pts;
	}
	s->start = s->cur;
	s->time[p->w38c] = p->l350 - s->t68;
	s->t68 = p->l350;
	k = p->b339 & 3;
	switch (p->w38c) {
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
		if (k < 2)
			p->b338 &= 0xdf;
		break;
	case 5:
	case 6:
	case 7:
	case 8:
		if (k < 1)
			p->b338 &= 0xdf;
		break;
	}
	p->b339 &= 0xfc;
	if (p->w38c == 0) {
		if (p->l350 <= 0xf3c)
			p->b338 |= 1;
	} else if (p->w38c == 4) {
		if (p->l350 <= 0x5460)
			p->b338 |= 2;
		p->l334 = p->l350 / 5;
	} else if (p->w38c == 9) {
		if (p->l350 <= 0x7b0c)
			p->b338 |= 4;
		if (s->time[p->w38c] <= 0xa8c)
			p->b338 |= 8;
		if (p->w38a >= 17)
			p->b338 |= 64;
		p->l354 = p->l350;
	}
	switch (p->w38c) {
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
		if (s->time[p->w38c] > 0xf3c)
			p->b338 &= 0xef;
		break;
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
		if (p->l334 + 120 < s->time[p->w38c])
			p->b338 &= 0xef;
		p->l334 = s->time[p->w38c];
		break;
	}
	return 0;
}

void f_11cc6(struct fld *p)
{
	unsigned short flag = 0;
	unsigned short i;

	if (p->l308 & 32)
		return;
	if (p->w38c >= 10)
		return;
	for (i = p->w38c; i < 10; i++) {
		if (p->w322 >= d_38f58[i]) {
			if (0) g_6064880 = 1; /* dead code: steers SHC register ranking */
			f_11a20(p);
			if (g_6064880 & 1) {
				if (p->w30c & 2)
					f_2190c(p, ((struct tm *)(g_6079394 + (unsigned char)(p->b30e * 76)))->time[p->w38c], g_6065750[p->w38c]);
				if (p->w30c & 0x1000)
					f_2190c(p, ((struct tm *)(g_6079394 + (unsigned char)(p->b30e * 76)))->time[p->w38c], g_6065778[p->w38c]);
			}
			if (p->w38c < 9) {
				if (p->w30c & 0x1000) {
					if (p->b3a1)
						p->w38c++;
					else if (p->w38c < 4)
						p->w38c++;
				} else {
					p->w38c++;
				}
				g_6066198 |= p->b30e == 0 ? 2 : 4;
				if (p->w30c & 8) {
					if (p->w38c >= g_6060038[0] + 1)
						flag = 1;
				} else if (p->w30c & 5) {
					if (p->w38c == 5)
						flag = 1;
				} else {
					flag = 1;
				}
				if (flag)
					f_2e6fc(35);
			}
		}
	}
}
