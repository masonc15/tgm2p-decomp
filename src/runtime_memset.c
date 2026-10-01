/* rom: 0x313e4 len: 0x18 func: f_313e4 flags: -macsave=1 -optimize=1 -speed */
void *f_313e4(void *s, int c, unsigned long n)
{
    unsigned long i;
    char *p = s;
    for (i = 0; i < n; i++) *p++ = c;
    return s;
}
