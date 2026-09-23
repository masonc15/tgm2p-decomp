/* rom: 0x2fdd8 len: 0x714 func: eeprom_send_bit flags: -macsave=1 -optimize=0 */
/* EEPROM (93C56) bit-bang routines. 0x23000004 is the cache-through mirror of
 * the PS5V2 I/O port at 0x03000004: bit 5 = DI, bit 6 = CLK, bit 7 = CS. */
#define EEPROM_PORT (*(volatile unsigned char *)0x23000004)

extern short eeprom_shadow[];            /* 0x6060034 */

void eeprom_send_bit(unsigned char bit)             /* 0x2fdd8 */
{
	int i;

	EEPROM_PORT = (bit << 5) | 0x82;
	for (i = 0; i < 3; i++)
		;
	EEPROM_PORT = (bit << 5) | 0xc2;
	for (i = 0; i < 3; i++)
		;
	EEPROM_PORT = (bit << 5) | 0x82;
	for (i = 0; i < 3; i++)
		;
	EEPROM_PORT = (bit << 5) | 0x82;
	for (i = 0; i < 3; i++)
		;
}

void eeprom_write_enable(void)                     /* 0x2fe84 */
{
	int i;

	EEPROM_PORT = 0xa2;
	for (i = 0; i < 3; i++)
		;
	EEPROM_PORT = 0xe2;
	for (i = 0; i < 3; i++)
		;
	EEPROM_PORT = 0x82;
	for (i = 0; i < 3; i++)
		;
	eeprom_send_bit(0);
	eeprom_send_bit(0);
	eeprom_send_bit(1);
	eeprom_send_bit(1);
	for (i = 0; i < 6; i++)
		eeprom_send_bit(0);
	EEPROM_PORT = 0xc2;
	for (i = 0; i < 3; i++)
		;
	EEPROM_PORT = 2;
	for (i = 0; i < 3; i++)
		;
}

void eeprom_write_disable(void)                    /* 0x2ff5c */
{
	int i;

	EEPROM_PORT = 0xa2;
	for (i = 0; i < 3; i++)
		;
	EEPROM_PORT = 0xe2;
	for (i = 0; i < 3; i++)
		;
	EEPROM_PORT = 0x82;
	for (i = 0; i < 3; i++)
		;
	eeprom_send_bit(0);
	eeprom_send_bit(0);
	eeprom_send_bit(0);
	eeprom_send_bit(0);
	for (i = 0; i < 6; i++)
		eeprom_send_bit(0);
	EEPROM_PORT = 0xc2;
	for (i = 0; i < 3; i++)
		;
	EEPROM_PORT = 2;
	for (i = 0; i < 3; i++)
		;
}

void eeprom_read_all(void)                         /* 0x3003c */
{
	unsigned char addr;
	unsigned char i;
	unsigned char j;

	for (addr = 0; addr < 20; addr++) {
		EEPROM_PORT = 0xa2;
		for (i = 0; i < 3; i++)
			;
		EEPROM_PORT = 0xe2;
		for (i = 0; i < 3; i++)
			;
		EEPROM_PORT = 0x82;
		for (i = 0; i < 3; i++)
			;
		eeprom_send_bit(1);
		eeprom_send_bit(0);
		eeprom_send_bit(0);
		for (i = 0; i < 8; i++) {
			if ((addr >> (7 - i)) & 1)
				eeprom_send_bit(1);
			else
				eeprom_send_bit(0);
		}
		eeprom_shadow[addr] = 0;
		EEPROM_PORT = 0x82;
		for (j = 0; j < 1; j++)
			;
		EEPROM_PORT = 0xc2;
		for (j = 0; j < 1; j++)
			;
		for (i = 0; i < 8; i++) {
			if (EEPROM_PORT & 0x10)
				eeprom_shadow[addr] |= 1 << (7 - i);
			EEPROM_PORT = 0x82;
			for (j = 0; j < 1; j++)
				;
			EEPROM_PORT = 0xc2;
			for (j = 0; j < 1; j++)
				;
		}
		EEPROM_PORT = 0x42;
	}
}

void eeprom_erase_all(void)                        /* 0x301fe */
{
	unsigned char i;

	eeprom_write_enable();
	EEPROM_PORT = 0xa2;
	for (i = 0; i < 3; i++)
		;
	EEPROM_PORT |= 0xe2;
	for (i = 0; i < 3; i++)
		;
	EEPROM_PORT = 0x82;
	for (i = 0; i < 3; i++)
		;
	eeprom_send_bit(0);
	eeprom_send_bit(0);
	eeprom_send_bit(0);
	eeprom_send_bit(1);
	for (i = 0; i < 7; i++)
		eeprom_send_bit(0);
	for (i = 0; i < 8; i++) {
		if ((i < 2) | (i > 5))
			eeprom_send_bit(1);
		else
			eeprom_send_bit(0);
	}
	EEPROM_PORT = 0x42;
	for (i = 0; i < 3; i++)
		;
	EEPROM_PORT = 2;
	for (i = 0; i < 3; i++)
		;
	EEPROM_PORT = 0xc2;
	while (!(EEPROM_PORT & 0x10)) {
		EEPROM_PORT |= 0x82;
		for (i = 0; i < 3; i++)
			;
	}
	EEPROM_PORT = 0x42;
	eeprom_write_disable();
}

void eeprom_write_all(void)                        /* 0x3036e */
{
	int addr;
	int i;

	eeprom_write_enable();
	for (addr = 0; addr < 20; addr++) {
		EEPROM_PORT = 0xa2;
		for (i = 0; i < 3; i++)
			;
		EEPROM_PORT = 0xe2;
		for (i = 0; i < 3; i++)
			;
		EEPROM_PORT = 0x82;
		for (i = 0; i < 3; i++)
			;
		eeprom_send_bit(0);
		eeprom_send_bit(1);
		eeprom_send_bit(0);
		for (i = 0; i < 8; i++) {
			if ((addr >> (7 - i)) & 1)
				eeprom_send_bit(1);
			else
				eeprom_send_bit(0);
		}
		for (i = 0; i < 8; i++) {
			if ((eeprom_shadow[addr] >> (7 - i)) & 1)
				eeprom_send_bit(1);
			else
				eeprom_send_bit(0);
		}
		EEPROM_PORT = 0x42;
		for (i = 0; i < 3; i++)
			;
		EEPROM_PORT = 2;
		for (i = 0; i < 3; i++)
			;
		EEPROM_PORT = 0xc2;
		while (!(EEPROM_PORT & 0x10))
			;
		EEPROM_PORT = 0x42;
	}
	eeprom_write_disable();
}
