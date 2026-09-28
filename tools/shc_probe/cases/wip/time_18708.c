/* rom: 0x18708 len: 0x1a4 func: f_18708 flags: -macsave=1 -optimize=1 -speed */
/* Work in progress. f_18708 (100%) queues a frame count to be drawn as
 * minutes:seconds:hundredths; f_18780 (45.8%) draws it next pass. In
 * f_18780 everything up to the first colon matches; after each colon call
 * SHC here emits a stray "mov r11,r3" and pushes the 110 one slot early,
 * which shifts the rest. The same happens appended to final_185f0.c.
 * v / 3600 etc. are unsigned: SHC calls its unsigned divide at RAM
 * 0x06030674 (needs a symbols.txt entry, presumably _divlu). */
struct frame { char pad[12]; };
struct task {
	char pad0[6];
	short tick;                /* 0x06 */
	char pad1[0x10 - 8];
	void (*func)(struct task *); /* 0x10 */
	long data[64];             /* 0x14 */
};
struct time {                  /* the task's data, from 0x14 */
	unsigned char m;           /* 0x14 */
	unsigned char s;           /* 0x15 */
	unsigned char c;           /* 0x16 */
	short a;                   /* 0x18 */
	short b;                   /* 0x1a */
};
extern struct task *f_17614(void);
extern void f_17638(struct task *);
extern void f_1159c(struct frame *, short, short, short, short);
extern struct frame *d_3b0c4[];
extern struct frame d_a7130[];

void f_18780(struct task *t);

void f_18708(unsigned long v, short a, short b)
{
	struct task *t;
	struct time *p;
	unsigned char m, s, c;

	if ((t = f_17614()) != 0) {
		t->func = f_18780;
		t->tick = 1;
		m = v / 3600;
		v -= m * 3600;
		s = v / 60;
		v -= s * 60;
		c = v * 100 / 60;
		p = (struct time *)t->data;
		p->m = m;
		p->s = s;
		p->c = c;
		p->a = a;
		p->b = b;
	}
}

void f_18780(struct task *t)
{
	struct time *p = (struct time *)t->data;
	unsigned char m, s, c;
	int a, b;

	a = p->a;
	b = p->b;
	m = p->m;
	s = p->s;
	c = p->c;
	f_1159c(d_3b0c4[m / 10], a, b, 0, 110);
	f_1159c(d_3b0c4[m % 10], a, b + 11, 0, 110);
	f_1159c(d_a7130, a, b + 22, 0, 110);
	f_1159c(d_3b0c4[s / 10], a, b + 27, 0, 110);
	f_1159c(d_3b0c4[s % 10], a, b + 38, 0, 110);
	f_1159c(d_a7130, a, b + 49, 0, 110);
	f_1159c(d_3b0c4[c / 10], a, b + 54, 0, 110);
	f_1159c(d_3b0c4[c % 10], a, b + 65, 0, 110);
	f_17638(t);
}
