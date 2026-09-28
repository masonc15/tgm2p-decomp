/* rom: 0x2f3c len: 0x2c func: rand_next flags: -macsave=1 -optimize=1 -speed */
/* Linear congruential generator (the classic 1103515245/12345 pair) returning
 * bits 10-24 of the next state. The state only advances when update is set,
 * so callers can peek at the next value. */
int rand_next(unsigned long *seed, unsigned short update)
{
	unsigned long x = *seed;

	x = x * 0x41c64e6d + 12345;
	if (update)
		*seed = x;
	return (x >> 10) & 0x7fff;
}
