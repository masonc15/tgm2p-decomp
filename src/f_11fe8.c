/* rom: 0x11fe8 len: 0xd8 func: f_11fe8 flags: -macsave=1 -optimize=1 -speed */
/* Copies two ROM tables into the state block and clears a third. */
struct pair { long a, b; };
struct quad { short a, b, c, d; };
struct state {
	char pad0[0x44];
	struct pair t44[19];       /* 0x044 */
	struct quad tdc[3];        /* 0x0dc */
	short tf4[3];              /* 0x0f4 */
};
extern long g_60794bc;
extern struct state g_6065650;
extern struct pair d_3afb4[19];
extern struct quad d_3b04c[3];

void f_11fe8(void)
{
	short i;

	g_60794bc = 0;
	for (i = 0; i < 19; i++)
		g_6065650.t44[i] = d_3afb4[i];
	for (i = 0; i < 3; i++)
		g_6065650.tdc[i] = d_3b04c[i];
	for (i = 0; i < 3; i++)
		g_6065650.tf4[i] = 0;
}
