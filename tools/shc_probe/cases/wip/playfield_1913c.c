/* rom: 0x1913c len: 0x1a4 func: f_1913c flags: -macsave=1 -optimize=1 -speed */
/* f_1913c, f_1919e 100%; f_191a4 91.4%: head-pointer address (want r11) and v (want r12)
 * swap registers, and the tail jmp uses r3 instead of r2. */
struct player {
	char b0;
	char b1;
	short w[3];
	char c[4];
	char d[0x800];
	void *field;               /* 0x80c */
	struct player *prev;       /* 0x810 */
	struct player *next;       /* 0x814 */
};
extern struct player g_6079544[6];
extern short g_607c5d4;
extern struct player *g_607c5d8, *g_607c5dc;
extern unsigned short g_6060000;
extern void (*d_3b460[])(struct player *);
extern void f_10228(struct player *);

struct player *f_1913c(void)
{
	struct player *p;
	short i;

	if (g_607c5d4 == 0)
		return 0;
	p = g_6079544;
	for (i = 0; i < 6; i++, p++) {
		if (p->b0 == 0) {
			g_607c5d4--;
			if (g_607c5d8 == 0) {
				g_607c5d8 = p;
				g_607c5dc = p;
				p->prev = 0;
				p->next = 0;
			} else {
				p->next = g_607c5dc->next;
				g_607c5dc->next = p;
				p->prev = g_607c5dc;
				g_607c5dc = p;
			}
			return p;
		}
	}
	return 0;
}

void f_1919e(struct player *p)
{
	p->b0 = 2;
}

void f_191a4(void)
{
	struct player *p;
	short v, j;

	if (g_607c5d8 == 0)
		return;
	if (g_6060000 >= 40)
		return;
	p = g_607c5d8;
	do {
		switch (p->b0) {
		case 1:
			if ((v = p->b1) >= 1 && v <= 19)
				d_3b460[v - 1](p);
			else
				f_1919e(p);
			break;
		case 2:
		default:
			if (p->prev == 0)
				g_607c5d8 = p->next;
			else
				p->prev->next = p->next;
			if (p->next == 0)
				g_607c5dc = p->prev;
			else
				p->next->prev = p->prev;
			p->b0 = 0;
			p->b1 = 0;
			for (j = 0; j < 3; j++)
				p->w[j] = 0;
			for (j = 0; j < 4; j++)
				p->c[j] = 0;
			g_607c5d4++;
			break;
		}
		p = p->next;
	} while (p != 0);
	if (g_607c5d8 != 0)
		f_10228(g_607c5d8);
}
