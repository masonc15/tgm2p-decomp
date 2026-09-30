/* rom: 0xd28c len: 0x108 func: f_d28c flags: -macsave=1 -optimize=1 -speed */
void f_d28c(unsigned long t, char *s)
{
	unsigned short sec, min, hour;

	s[5] = ':';
	s[2] = ':';
	if (t >= 5184000UL)
		t %= 5184000UL;
	t /= 60;
	sec = t % 60;
	t /= 60;
	min = t % 60;
	hour = t / 60;
	s[0] = hour / 10 + '0';
	s[1] = hour % 10 + '0';
	s[3] = min / 10 + '0';
	s[4] = min % 10 + '0';
	s[6] = sec / 10 + '0';
	s[7] = sec % 10 + '0';
	s[8] = 0;
}

void f_d33a(unsigned short g, char *s)
{
	if ((g >> 8) >= 16) {
		s[0] = 'A';
		s[1] = 'L';
		s[2] = 'L';
	} else {
		if ((g >> 8) >= 8) {
			s[0] = '2';
			s[2] = (g >> 8) + 41;
		} else {
			s[0] = '1';
			s[2] = (g >> 8) + 49;
		}
		s[1] = '-';
	}
	s[3] = 0;
}

void f_d37c(void)
{
}
