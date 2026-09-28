/* rom: 0x175c4 len: 0x11c func: task_init_all flags: -macsave=1 -optimize=1 -speed */
/* The task (game object) pool: 256 slots of 0x114 bytes at 0x060661a8.
 * Objects take a slot with task_alloc, set func, and task_run_all calls
 * func(t) for every active slot. Callers in other files reach these with
 * jsr, and this unit matches from a fresh file, so it is probably a whole
 * source file of its own. */
struct task {
	unsigned char active;          /* 0x00 */
	unsigned char b[4];            /* 0x01 */
	short w[4];                    /* 0x06 */
	void (*func)(struct task *);   /* 0x10 */
	long data[64];                 /* 0x14, owner-defined */
};
extern struct task g_60661a8[256];

void task_init_all(void)
{
	struct task *t;
	int i, j;

	for (i = 0, t = g_60661a8; i < 256; i++, t++) {
		t->active = t->b[0] = t->b[1] = t->b[2] = t->b[3] = t->w[0] = t->w[1] = t->w[2] = t->w[3] = 0;
		t->func = 0;
		for (j = 0; j < 64; j++)
			t->data[j] = 0;
	}
}

/* Returns a free slot marked active, or 0 when all 256 are in use. */
struct task *task_alloc(void)
{
	struct task *t;
	int i;

	t = g_60661a8;
	for (i = 0; i < 256; i++) {
		if (t->active == 0) {
			t->active = 1;
			return t;
		}
		t++;
	}
	return 0;
}

void task_free(struct task *t)
{
	int j;

	t->active = t->b[0] = t->b[1] = t->b[2] = t->b[3] = t->w[0] = t->w[1] = t->w[2] = t->w[3] = 0;
	t->func = 0;
	for (j = 0; j < 64; j++)
		t->data[j] = 0;
}

void task_run_all(void)
{
	struct task *t;
	int i;

	for (t = g_60661a8, i = 0; i < 256; i++, t++)
		if (t->active)
			t->func(t);
}
