/* rom: 0x2190c len: 0x2bc func: medal_st flags: -macsave=1 -optimize=1 -speed */
/* Medal awards. Each player has six medals in the order the game shows them
 * (AC, ST, SK, RE, RO, CO), with a grade from 0 to 3 (none, bronze, silver,
 * gold). Setting changed[0] tells the display a medal was just upgraded.
 * This matches from a fresh file, so it is probably where a source file
 * starts. */
struct medal {
	unsigned char changed[4];  /* 0x0 */
	unsigned char grade;       /* 0x4 */
	char pad[7];
};
struct medals {
	struct medal m[6];         /* AC ST SK RE RO CO */
};
struct field {
	char pad0[0x30c];
	unsigned short mode;       /* 0x30c */
	unsigned char id;          /* 0x30e, player number */
	char pad1[0x322 - 0x30f];
	unsigned short level;      /* 0x322 */
	char pad2[0x339 - 0x324];
	unsigned char flags;       /* 0x339 */
	unsigned char all_clears;  /* 0x33a */
	unsigned char best_st;     /* 0x33b */
	char pad3[0x33d - 0x33c];
	unsigned char re_grade;    /* 0x33d */
	unsigned char ro_grade;    /* 0x33e */
	char pad4[0x341 - 0x33f];
	unsigned char recovers;    /* 0x341 */
	unsigned char ro_checks;   /* 0x342 */
	char pad5[0x344 - 0x343];
	unsigned short blocks;     /* 0x344, filled cells in the stack */
	char pad6[0x38c - 0x346];
	unsigned short section;    /* 0x38c */
	char pad7[0x392 - 0x38e];
	unsigned short rotations;  /* 0x392, total since the last check */
	unsigned short pieces;     /* 0x394, pieces since the last check */
	unsigned short turns;      /* 0x396, rotations of the current piece */
};
extern struct medals g_607cd48[];
extern signed char d_3b5c8[];                 /* recoveries needed per RE grade */
extern short d_3b5cc[];                       /* levels where RO is checked */
extern unsigned char d_3b5d2[];               /* 10x average rotations needed */
extern unsigned long g_6065750[], g_6065778[]; /* gold section times, by mode */

/* The byte offset is truncated to 8 bits after the multiply, which is what
 * the ROM does; with two players it never matters. */
#define MEDALS(f) ((struct medals *)((char *)g_607cd48 + (unsigned char)((f)->id * 72)))

#define RE_FLAG 4                  /* stack went above 150 cells */

/* ST: gold for a section under par, silver within 5 seconds of it, bronze
 * within 10. A gold time in mode 2 or 0x1000 is also recorded per section. */
void medal_st(struct field *f, unsigned long time, unsigned long par)
{
	unsigned char n;

	par += 600;
	for (n = 0; n < 3; n++) {
		if (time >= par)
			break;
		par -= 300;
	}
	if (n) {
		MEDALS(f)->m[1].grade = n;
		MEDALS(f)->m[1].changed[0] = 1;
		if (f->best_st < n)
			f->best_st = n;
		if (n == 3 && f->mode == 2)
			g_6065750[f->section] = time;
		if (n == 3 && f->mode == 0x1000)
			g_6065778[f->section] = time;
	}
}

/* AC: one grade per all clear, up to gold. */
void medal_ac(struct field *f)
{
	if (f->all_clears < 3) {
		f->all_clears++;
		MEDALS(f)->m[0].grade = f->all_clears;
		MEDALS(f)->m[0].changed[0] = 1;
	}
}

/* RE: build the stack above 150 cells, then dig it back down to 70 or fewer.
 * Each such recovery counts, and d_3b5c8 says how many each grade needs. */
void medal_re(struct field *f)
{
	if (f->re_grade < 3) {
		if (f->flags & RE_FLAG) {
			if (f->blocks <= 70) {
				f->recovers++;
				f->flags &= ~RE_FLAG;
				if (d_3b5c8[f->re_grade] <= f->recovers) {
					f->re_grade++;
					MEDALS(f)->m[3].grade = f->re_grade;
					MEDALS(f)->m[3].changed[0] = 1;
				}
			}
		} else if (f->blocks >= 150) {
			f->flags |= RE_FLAG;
		}
	}
}

/* RO: at each checkpoint level, the average number of rotations per piece
 * since the last one (counting at most 4 per piece) decides the grade. */
void medal_ro(struct field *f)
{
	if (f->ro_grade < 3) {
		if (f->turns > 4)
			f->turns = 4;
		if (f->rotations < 3000)
			f->rotations += f->turns;
		f->pieces++;
		if (d_3b5cc[f->ro_checks] <= f->level) {
			f->ro_checks++;
			if (d_3b5d2[f->ro_checks - 1] <= f->rotations * 10 / f->pieces) {
				f->ro_grade = f->ro_checks;
				MEDALS(f)->m[4].grade = f->ro_grade;
				MEDALS(f)->m[4].changed[0] = 1;
			}
			f->pieces = 0;
			f->rotations = 0;
		}
	}
	f->turns = 0;
}
