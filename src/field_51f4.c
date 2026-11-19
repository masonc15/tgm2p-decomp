/* rom: 0x51f4 len: 0xfdc func: f_51f4 flags: -macsave=1 -optimize=1 -speed */
/* Line-clear compiler unit 0x51f4-0x61d0: row tests (f_51f4, f_5262), row
 * removal (f_52b6), the rising-garbage row queue (f_5390, f_541c), the line
 * clear handler (f_57d4) and the line-collapse step that applies the mode
 * level caps (f_5b80), then the piece randomizer helpers f_601a, f_608c and
 * f_60e8. f_57d4 reaches its helpers with bsr, so they share this unit; f_61d0
 * (still asm) follows in the original file and calls f_601a/f_60e8 the same way. */
struct cell { unsigned short flags; unsigned char b2,b3; short timer; };
union fixed { long full; struct { short hi; unsigned short lo; } part; };
struct pos { short x; char pad[2]; union fixed y; };
struct field {
	struct cell *cells;
	unsigned short rows[9][12];
	char pad0dc[2];
	unsigned char height,width;
	char pade0[0x2f4-0xe0]; struct field *owner;
	char pad2f8[8]; short timer;
	char pad302[6]; unsigned long flags;
	unsigned short mode; unsigned char id,pad30f; unsigned long seed;
	char pad314[0x322-0x314]; unsigned short level,lines;
	char pad326[0x330-0x326]; unsigned long score;
	char pad334[4]; unsigned char b338,b339;
	char pad33a[0x344-0x33a]; short w344; unsigned char milestone;
	char pad347[0x350-0x347]; unsigned long time;
	char pad354[8]; unsigned short w35c,pad35e,piece; unsigned char rotation,pad363;
	union fixed pos[2]; char pad36c[4]; long remaining,available;
	unsigned char task,pad379,cxy[2]; char pad37c[2]; unsigned char delay,pad37f,last;
	char pad381[3]; unsigned long rowmask;
	unsigned char b388,pad389; unsigned short w38a;
	char pad38c[0x398-0x38c]; unsigned char b398; char pad399[0x39e-0x399];
	unsigned char b39e,b39f,pad3a0,b3a1;
};
struct grid { unsigned short c[22][20]; };
struct task { char active,kind; char pad2[0x80c-2]; struct field *field; };
extern struct grid g_6077608;
extern unsigned long g_6064880,g_606487c;
extern void f_17d48(struct field *f, short y);
extern long f_7658(struct field *, unsigned char);
extern void f_179b8(struct field *, int, int, int);
extern void f_2e6fc(int);
extern void f_3108(struct field *);
extern void f_31dc(struct field *, unsigned char);
extern short f_11e98(struct field *);
extern void f_21068(struct field *, short, short);
extern void f_219a8(struct field *);
extern void f_21a10(struct field *);
extern void f_21ad0(struct field *);
extern struct task *f_1913c(void);
extern short d_38f58[];
extern struct field g_6064898;
extern void f_21d94(struct field *),f_3574(struct field *);
extern unsigned char f_35f0(struct field *,short,short,unsigned char);
extern unsigned short f_2f3c(unsigned long *,unsigned short);
extern const unsigned char d_3a7ca[];
extern void f_7fa0(struct field *);
/* 1 if no interior cell (rows 1..h-2, columns 1..w-2) is set, ignoring bit 0x4000. */
int f_51f4(struct field *f)
{
	short y, x;

	for (y = 1; y < f->height - 1; y++)
		for (x = 1; x < f->width - 1; x++)
			if (f->cells[y * f->width + x].flags & ~0x4000)
				return 0;
	return 1;
}

/* 1 if every interior cell of row y is set, ignoring bit 0x4000. */
unsigned char f_5262(struct field *f, short y)
{
	register short x;

	for (x = 1; x < f->width - 1; x++)
		if (!(f->cells[y * f->width + x].flags & ~0x4000))
			return 0;
	return 1;
}

/* Clear the rows flagged in rows. Returns rows with each row that kept a
 * 0x400 cell unflagged and bit 31 set. */
long f_52b6(struct field *f, long rows)
{
	short y, x;
	long mask;
	struct cell *p;

	for (y = 1; y < f->height - 1; y++) {
		if (rows & (1 << y)) {
			f_17d48(f, y);
			x = 1;
			p = &f->cells[y * f->width + 1];
			mask = ~(1 << y);
			for (; x < f->width - 1; x++, p++) {
				if (p->flags & 0x400) {
					rows |= 0x80000000;
					rows &= mask;
					p->flags &= 0xf0ff;
				} else {
					if (p->flags & 0x2000) {
						f->task = p->b3;
						f->cxy[0] = x;
						f->cxy[1] = y;
					}
					p->flags = 0;
					p->b3 = 0;
				}
			}
		}
	}
	return rows;
}

void f_5390(struct field *pl, short idx, unsigned char add)
{
	struct field *f;
	register unsigned short *s;
	short x;
	struct grid *g = &g_6077608;

	s = &g->c[idx][1];
	f = pl->owner;
	for (x = 1; x < 11; x++)
		f->rows[(unsigned char)(f->b388 + add)][x - 1] = *s++;
}

/* Snapshot queued rows, restore their current positions, then duplicate them
 * after that segment. Preserve the original byte-truncated row offsets. */
unsigned char f_541c(struct field *f, unsigned char count)
{
	struct field *owner=f->owner;
	struct grid *g = &g_6077608;
	short y,x;

	for(y=0;y<count;y++) {
		for(x=1;x<11;x++)
			g->c[y][x] = ((unsigned short *)((char *)owner->rows + (unsigned char)((owner->b388+y)*24)))[x-1];
	}
	for(y=0;y<count;y++) {
		for(x=1;x<11;x++) {
			if(owner->b388+y<8)
				((unsigned short *)((char *)owner->rows + (unsigned char)((owner->b388+y)*24)))[x-1]=g->c[y][x];
		}
	}
	for(y=0;y<count;y++) {
		for(x=1;x<11;x++) {
			if(owner->b388+y+count<8)
				((unsigned short *)((char *)owner->rows + (unsigned char)((owner->b388+y+count)*24)))[x-1]=g->c[y][x];
		}
	}
	count*=2;
	if(owner->b388+count>8)
		count=8-owner->b388;
	return count;
}

int f_5760(struct field *f, unsigned long rows)
{
	short y;
	int count=0;
	for(y=1;y<f->height-1;y++)
		if((1<<y)&rows) count++;
	return count;
}

void f_579c(struct field *f, int a, unsigned char b) { long limit=999999;if(f->mode&1) f->score+=b*a*6; else f->score+=b*a;if(f->score>limit) f->score=limit; }

/* f_57d4's mode-0x80 counter increment goes through an inline helper. */
#pragma inline(bump)
static void bump(struct field *g)
{
	g->b398++;
}
/* Clear the completed rows: score them, queue garbage rows, start the clear
 * animation, and record any item task found in a cleared cell. */
void f_57d4(struct field *f)
{
	struct field *owner = f->owner;
	unsigned char special = g_6064880 & 2;
	unsigned char count;
	unsigned char limit;
	unsigned char all_clear;
	unsigned char m;
	short y;
	long rows;
	long points;

	count = 0;
	if (f->mode & 0x40)
		limit = 8;
	else
		limit = 4;
	rows = 0;
	if (f->mode & 0x100)
		special = 0;
	for (y = 1; y < f->height - 1; y++) {
		if (f_5262(f, y)) {
			rows |= 1 << y;
			if (special && owner->b388 + count < 8)
				f_5390(f, y, count);
			count++;
		}
	}
	m = f->b338 & 0x75;
	if (g_6064880 & 7) {
		if ((f->mode & 0x10) && m == 0x75) {
			if (count > 3)
				f->b39f++;
			f->b39e += count;
		}
		if (!(f->flags & 0x20))
			points = f_7658(f, count);
	}
	if (count)
		rows = f_52b6(f, rows);
	if (f->task)
		points = 0;
	count = f_5760(f, rows);
	if (count)
		rows &= 0x7fffffff;
	if (!(f->flags & 0x20)) {
		all_clear = 0;
		if (f->task == 0)
			all_clear = f_51f4(f);
		if (all_clear) {
			if (f->mode & 4)
				f_179b8(f, 10, 6, 0);
			else
				f_179b8(f, 10, 4, 0);
		}
		if (special) {
			if (count > 1 && all_clear)
				count = f_541c(f, count);
		} else
			f_579c(f, points, all_clear ? 4 : 1);
	}
	if (f->flags & 0x100000) {
		if (count == 0)
			f->flags &= ~0x100000;
		else if (count >= limit)
			f_2e6fc(26);
		else
			f_2e6fc(9);
	} else if (count >= limit) {
		f_2e6fc(31);
		f->flags |= 0x100000;
	}
	if (special) {
		if (owner->b388 + count > 8)
			count = 8 - owner->b388;
		if (!f->task && count > 1)
			owner->b388 += count;
	}
	if (rows) {
		f->rowmask = rows;
		f_3108(f);
		f_2e6fc(20);
	} else {
		if (f->mode & 0x80)
			bump(f);
		f_31dc(f, 0);
		if (g_6064880 & 4)
			f->owner->flags &= 0x7fffffff;
	}
	f->w344 = f_11e98(f);
	if (f->task) {
		f_21068(f, f->cxy[1], f->cxy[0]);
		switch (f->task) {
		case 2: case 4: case 5: case 8: case 10: case 12: case 15:
			f->b339 &= ~4;
		}
	}
	if (!(f->flags & 0x20)) {
		if (f->w344 == 0) {
			if ((g_6064880 & 1) && (f->mode & 0x1002))
				f_219a8(f);
		}
		if ((g_6064880 & 1) && (f->mode & 0x1002))
			f_21a10(f);
		if ((g_6064880 & 1) && (f->mode & 0x1002))
			f_21ad0(f);
	}
}
#pragma inline(finish)
static void finish(struct field *g)
{
	g->flags |= 32;
	f_21d94(g);
}
/* Finish line removal, then apply the active mode's level cap and tasks. */
void f_5b80(struct field *f)
{
	unsigned long rows;
	short row, y, x;

	if (--f->timer)
		return;
	rows = f->rowmask;
	if (rows & 0x80000000)
		rows &= 0x7fffffff;
	else
		f_2e6fc(25);
	if (rows) {
		row = 1;
		while (rows) {
			if (rows & (1 << row)) {
				for (y = row; y < f->height - 1; y++) {
					for (x = 1; x < f->width - 1; x++) {
						f->cells[y * f->width + x] = f->cells[(y + 1) * f->width + x];
						f->cells[y * (short)f->width + x].flags &= 0xff7f;
					}
				}
				for (x = 1; x < f->width - 1; x++) {
					f->cells[(f->height - 1) * f->width + x].flags = 0;
					f->cells[(f->height - 1) * f->width + x].b3 = 0;
				}
				if (g_6064880 & 4) {
					unsigned char kind = f->owner->w35c;
					if (kind == 2) {
						if (f_35f0(f->owner, f->owner->pos[0].part.hi, f->owner->pos[1].part.hi, f->owner->rotation))
							f->owner->pos[1].full -= 0x10000;
					}
				}
				rows &= ~(1 << row);
				rows >>= 1;
				f->lines++;
			} else
				row++;
		}
	}
	f->rowmask = 0;
	if (g_6064880 & 4)
		f->owner->flags &= 0x7fffffff;
	f_31dc(f, 1);
	if (!(f->flags & 32)) {
		if ((f->mode & 1) && (g_6064880 & 1)) {
			if (d_38f58[2] <= f->level) {
				f->level = d_38f58[2];
				finish(f);
				if (f->time <= 18000U) {
					unsigned long bonus = (18000 - f->time) / 60;

					if ((18000 - f->time) % 60)
						bonus++;
					f->score += bonus * 1253;
					if (f->score > 999999U)
						f->score = 999999;
				}
			}
		} else if ((f->mode & 2) && (g_6064880 & 1)) {
			if (d_38f58[9] <= f->level) {
				f->level = d_38f58[9];
				f_2e6fc(15);
				f_3574(f);
				f->b39f = 0;
				f->b39e = 0;
			}
		} else if ((f->mode & 0x80) && (g_6064880 & 1)) {
			if (d_38f58[9] <= f->level) {
				f->level = d_38f58[9];
				finish(f);
			}
		} else if ((f->mode & 0x1000) && (g_6064880 & 1)) {
			if (f->b3a1) {
				if (d_38f58[9] <= f->level) {
					f->level = d_38f58[9];
					f->w38a = 19;
					finish(f);
				}
			} else if (d_38f58[4] <= f->level) {
				f->level = d_38f58[4];
				finish(f);
			}
		} else if ((f->mode & 4) && d_38f58[2] <= f->level) {
			f->level = d_38f58[2];
			f->flags |= 32;
			if ((f->owner->flags & 32) && !(f->owner->flags & 64)) {
				finish(&g_6064898);
				return;
			}
		}
	}
	if (f->task) {
		struct task *t = f_1913c();
		if (t) {
			t->active = 1;
			t->kind = f->task;
			t->field = f;
		}
		f->task = 0;
	}
}
unsigned char f_601a(struct field *f)
{
	unsigned char value;
	if(g_6064880&4) {
		if(f->id==1) value=f_2f3c(&f->owner->seed,1)%7;
		else value=f_2f3c(&f->seed,1)%7;
	} else value=f_2f3c(&f->seed,1)%7;
	if(g_606487c==0) value%=7;
	return value;
}
void f_608c(struct field *f,unsigned char index)
{
	f->remaining &= ~(1<<index);
	f->last=index+1;
	f->piece|=0x2000;
}
void f_60e8(struct field *f)
{
	struct field *alias;
	unsigned int one=1;
	int mode_mask=0x208;
	int mode=f->mode;
	int delay_limit=20;
	if((mode&one)&&!(mode&8)&&!(f->flags&32)) {
		if(f->level>=(f->milestone+1)*100) {
			unsigned char index=d_3a7ca[f->milestone];
			if(f->available&(one<<index)) {
				f->available&=~(one<<index);
				alias=f; f_608c(alias,index);
				f->milestone++;
			}
		} else if((f->mode&mode_mask)&&f->delay>=delay_limit&&f->remaining) f_7fa0(f);
	} else if((f->mode&mode_mask)&&f->delay>=delay_limit&&f->remaining) f_7fa0(f);
}
