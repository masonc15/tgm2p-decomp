/* rom: 0x3127c len: 0x3e func: f_3127c flags: -macsave=1 -optimize=1 -speed */
int f_3127c(const char *a, const char *b, unsigned long n)
{
    unsigned long i; const char *p = b;
    if (!n) return 0;
    for (i = 0; i < n; i++) {
        if (*a++ != *p++ || !a[-1]) break;
    }
    return (unsigned char)a[-1] - (unsigned char)p[-1];
}
