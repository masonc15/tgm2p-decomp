/* rom: 0x1260 len: 0x18 func: f_1260 flags: -macsave=1 -optimize=1 -speed */
/* 0x2004002c is the cache-through view of the ROM header's pointer at 0x4002c. */
struct rom_hdr {
	char pad0[28];
	long l28;                  /* 0x1c */
};
extern struct rom_hdr *rom_hdr_ptr;        /* 0x2004002c */
extern long g_607926c;

void f_1260(void)
{
	g_607926c = rom_hdr_ptr->l28;
}

void f_126c(void)
{
}
