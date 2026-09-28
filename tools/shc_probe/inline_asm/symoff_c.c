struct s { char pad[0x2a]; unsigned char b; char pad2[0x40-0x2b]; };
extern struct s g_606006c[];
int f(int i) { return g_606006c[3].b; }
