/* rom: 0x23048 len: 0x60 func: f_23048 */
struct P { char pad0[0x308]; long l308; char pad1[0x314 - 0x30c]; short w314[4]; };
struct A { char pad0[20]; struct P *p; };
extern char g_60356bc[];
extern int f_600dc1c(char *s);
extern void f_600dd10(int x, int y, char *s, int pal);
void f_22f4e(struct A *a, struct P *p, int n)
{
}
void f_23048(struct A *a)
{
	struct P *p = a->p;
	int i;
	p->l308 |= 0x80000000;
	for (i = 0; i < 1; i++)
		f_600dd10(p->w314[i] - f_600dc1c(g_60356bc) / 2, 120, g_60356bc, 15);
	f_22f4e(a, p, 30);
}
