/* rom: 0x2d2bc len: 0x910 func: f_2d2bc flags: -macsave=1 -optimize=1 -speed */
struct dip {
	char pad0[10];
	short w10;                 /* 0x0a */
	short w12;                 /* 0x0c */
	short w14;                 /* 0x0e */
	short w16;                 /* 0x10 */
};
#define IN0 (*(unsigned char *)0x23000000)
#define IN1 (*(unsigned char *)0x23000001)
#define IN2 (*(unsigned char *)0x23000002)
#define IN3 (*(unsigned char *)0x23000003)
extern unsigned char g_6064754, g_6064755, g_6064756, g_6064757;
extern unsigned char g_6064758, g_6064759;
extern unsigned char g_606475a[2];
extern unsigned char g_606475e[2];
extern unsigned char g_6064760, g_6064761, g_6064762, g_6064763, g_6064764;
extern unsigned char g_6064765, g_6064766;
extern unsigned char g_60b1630, g_60b1631, g_60b1632, g_60b1635;
extern int g_60b161c, g_60b1620;
extern short g_60b13bc;
extern int g_6065640;
extern unsigned char g_607cf0c;
extern volatile short g_606003e, g_6060040, g_6060042;
extern struct dip g_6060034;
extern void f_2e6fc(int);

void f_2d2bc(void)
{
	g_6064765 = 20;
	g_6064766 = 10;
	g_6064761 = 0;
	g_6064760 = 0;
	g_6064763 = 0;
	g_6064762 = 0;
	g_6064764 = 0;
}

void f_2d2e0(void)
{
	register unsigned char a, b;
	unsigned char c3;
	unsigned char t2;
	int t;
	unsigned char *p0, *p1;
	char kk;
	struct dip *d;

	if (0) g_6064766 = g_6064765;
	g_6064756 = (~IN2 >> 4) & 15;
	g_6064757 = ~IN2 & 15;
	a = ~IN0;
	g_6064754 = a;
	b = ~IN1;
	g_6064755 = b;
	c3 = IN3;
	t2 = ~c3;
	if (g_60b1631 == (~c3 & 0x10)) {
		g_60b1630 = 0;
	} else {
		g_60b1631 = ~IN3 & 0x10;
		if (g_60b1632 > 8) {
			g_60b1630 = 1;
			f_2e6fc(0);
			g_60b1632 = 0;
		}
	}
	
	t = 1;
	if (~IN3 & 0x10)
		g_60b1632++;
	else
		g_60b1632 = 0;
	if (g_60b13bc != 0 && (g_6064754 & 1) == 0)
		t = 0;
	
	if (t) {
		p1 = &g_606475a[2];
		p0 = &g_606475a[0];
		p1[0] = p0[0];
		p1[1] = p0[1];
		p0[0] = a;
		p0[1] = b;
		g_606475e[0] = ~p1[0] & p0[0];
		g_606475e[1] = ~p1[1] & p0[1];
	}
	
	if (g_60b1630 != 0 && g_6060040 != 2) {
		if (g_606003e == 1)
			g_6064764++;
		else
			g_6064762++;
	}
	
	t2 |= ~IN3;
	if (g_6060040 != 2) {
		if (t2 & 1) {
			*(char *)&g_60b161c = 1;
		} else {
			if (g_60b161c != 0) {
				g_6065640++;
				g_6064760++;
				f_2e6fc(0);
			}
			g_60b161c = 0;
		}
		if (t2 & 2) {
			*(char *)&g_60b1620 = 1;
		} else {
			if (g_60b1620 != 0) {
				g_6065640++;
				g_6064761++;
				f_2e6fc(0);
			}
			g_60b1620 = 0;
		}
	}
	if (0) g_6064766 = 0;
	if (g_6064760) {
		if (g_6060040 == 0) {
			switch (g_6060042) {
			case 0:
				while (g_6064760 > 0) {
					g_6064762 += 1;
					g_6064760 -= 1;
				}
				break;
			case 1:
				while (g_6064760 > 1) {
					g_6064762 += 1;
					g_6064760 -= 2;
				}
				break;
			case 2:
				while (g_6064760 > 2) {
					g_6064762 += 1;
					g_6064760 -= 3;
				}
				break;
			case 3:
				while (g_6064760 > 3) {
					g_6064762 += 1;
					g_6064760 -= 4;
				}
				break;
			case 4:
				while (g_6064760 > 4) {
					g_6064762 += 1;
					g_6064760 -= 5;
				}
				break;
			case 5:
				while (g_6064760 > 5) {
					g_6064762 += 1;
					g_6064760 -= 6;
				}
				break;
			case 6:
				while (g_6064760 > 0) {
					g_6064762 += 2;
					g_6064760 -= 1;
				}
				break;
			case 7:
				while (g_6064760 > 0) {
					g_6064762 += 3;
					g_6064760 -= 1;
				}
				break;
			case 8:
				while (g_6064760 > 0) {
					g_6064762 += 4;
					g_6064760 -= 1;
				}
				break;
			}
		} else if (g_6060040 == 1) {
			while (g_6064760 > 0) {
				g_6064762 += 1;
				g_6064760 -= 1;
			}
		} else if (g_6060040 == 2) {
			g_6064760 = 0;
		}
	}
	
	if (g_6064761) {
		d = &g_6060034;
		if (d->w10 == 0) {
			if (d->w12 == 2) {
				g_6064761 = 0;
			} else if (d->w12 == 1) {
				while (g_6064761 > 0) {
					g_6064762 += 1;
					g_6064761 -= 1;
				}
			} else {
				switch (d->w16) {
				case 0:
					while (g_6064761 > 0) {
						g_6064762 += 1;
						g_6064761 -= 1;
					}
					break;
				case 1:
					while (g_6064761 > 1) {
						g_6064762 += 1;
						g_6064761 -= 2;
					}
					break;
				case 2:
					while (g_6064761 > 2) {
						g_6064762 += 1;
						g_6064761 -= 3;
					}
					break;
				case 3:
					while (g_6064761 > 3) {
						g_6064762 += 1;
						g_6064761 -= 4;
					}
					break;
				case 4:
					while (g_6064761 > 4) {
						g_6064762 += 1;
						g_6064761 -= 5;
					}
					break;
				case 5:
					while (g_6064761 > 5) {
						g_6064762 += 1;
						g_6064761 -= 6;
					}
					break;
				case 6:
					while (g_6064761 > 0) {
						g_6064762 += 2;
						g_6064761 -= 1;
					}
					break;
				case 7:
					while (g_6064761 > 0) {
						g_6064762 += 3;
						g_6064761 -= 1;
					}
					break;
				case 8:
					while (g_6064761 > 0) {
						g_6064762 += 4;
						g_6064761 -= 1;
					}
					break;
				}
			}
		} else if (d->w12 == 2) {
			g_6064761 = 0;
		} else if (d->w12 == 1) {
			while (g_6064761 > 0) {
				g_6064763 += 1;
				g_6064761 -= 1;
			}
		} else {
			switch (d->w14) {
			case 0:
				while (g_6064761 > 0) {
					g_6064763 += 1;
					g_6064761 -= 1;
				}
				break;
			case 1:
				while (g_6064761 > 1) {
					g_6064763 += 1;
					g_6064761 -= 2;
				}
				break;
			case 2:
				while (g_6064761 > 2) {
					g_6064763 += 1;
					g_6064761 -= 3;
				}
				break;
			case 3:
				while (g_6064761 > 3) {
					g_6064763 += 1;
					g_6064761 -= 4;
				}
				break;
			case 4:
				while (g_6064761 > 4) {
					g_6064763 += 1;
					g_6064761 -= 5;
				}
				break;
			case 5:
				while (g_6064761 > 5) {
					g_6064763 += 1;
					g_6064761 -= 6;
				}
				break;
			case 6:
				while (g_6064761 > 0) {
					g_6064763 += 2;
					g_6064761 -= 1;
				}
				break;
			case 7:
				while (g_6064761 > 0) {
					g_6064763 += 3;
					g_6064761 -= 1;
				}
				break;
			case 8:
				while (g_6064761 > 0) {
					g_6064763 += 4;
					g_6064761 -= 1;
				}
				break;
			}
		}
	}
	
	
	if (g_6064762 > 9)
		g_6064762 = 9;
	if (g_6064763 > 9)
		g_6064763 = 9;
	if (g_6064764 > 9)
		g_6064764 = 9;
	
	 if (g_607cf0c != 0 && ((~g_60b1635 & t2) & 0x40) != 0 && (signed char)g_607cf0c != 0) {
		if (g_60b13bc == 0)
			g_60b13bc = 1;
		else
			g_60b13bc = 0;
	}
 
	g_6064758 = a;
	g_6064759 = b;
	g_60b1635 = t2;
}

int f_2da90(short a, short b)
{
	unsigned char *p;
	struct dip *d = &g_6060034;
	unsigned char is3;
	int need;

	if (d->w12 == 2)
		return 1;
	if (d->w10 == 1) {
		if (a == 1) {
			if (g_6064764 > 0)
				p = &g_6064764;
			else if (g_6064763 > 0)
				p = &g_6064763;
		} else {
			if (g_6064764 > 0)
				p = &g_6064764;
			else if (g_6064762 > 0)
				p = &g_6064762;
		}
	} else if (g_6064764 > 0) {
		p = &g_6064764;
	} else {
		p = &g_6064762;
	}
	is3 = p == &g_6064764;
	if (d->w12 == 1) {
		if (b != 0) {
			if (*p > 0)
				return 1;
			return 0;
		}
		if (d->w10 == 1) {
			if (is3)
				need = 0;
			else
				need = 1;
		} else {
			need = 1;
		}
		if (*p > (unsigned char)need)
			return 1;
		return 0;
	}
	if (*p > 0)
		return 1;
	return 0;
}

void f_2db32(short a, short b)
{
	unsigned char *p;
	struct dip *d = &g_6060034;
	short is3;

	if (d->w12 == 2)
		return;
	if (d->w10 == 1) {
		if (a == 1) {
			if (g_6064764 > 0)
				p = &g_6064764;
			else if (g_6064763 > 0)
				p = &g_6064763;
		} else {
			if (g_6064764 > 0)
				p = &g_6064764;
			else if (g_6064762 > 0)
				p = &g_6064762;
		}
	} else if (g_6064764 > 0) {
		p = &g_6064764;
	} else {
		p = &g_6064762;
	}
	is3 = p == &g_6064764;
	if (d->w12 == 1 && b == 0 && is3 == 0)
		*p -= 2;
	else
		*p -= 1;
	if ((signed char)*p < 0)
		*p = 0;
}
