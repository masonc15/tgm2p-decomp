/* rom: 0x23828 len: 0x80 func: f_23828 flags: -macsave=1 -optimize=1 -speed */
struct code_step {
	signed char key;
	unsigned char val;
};
extern signed char g_607cf0d;
extern unsigned char g_607cf0c;
extern short g_6060040;
extern unsigned char g_606475e[];
extern struct code_step d_3b5d8[];
extern void f_2e6fc(int);

void f_23828(void)
{
	g_607cf0d = 0;
}

int f_23830(void)
{
	struct code_step *p = &d_3b5d8[g_607cf0d];

	if (g_6060040 == 2) {
		if (g_606475e[0] || g_606475e[1]) {
			if (g_606475e[p->key] == p->val) {
				g_607cf0d++;
				p++;
			} else {
				g_607cf0d = 0;
			}
			if (p->key < 0) {
				g_607cf0d = 0;
				g_607cf0c = 1;
				f_2e6fc(36);
				return 1;
			}
		}
	}
	return 0;
}
