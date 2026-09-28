/* rom: 0x2b430 len: 0xb8 func: frt_init flags: -macsave=1 -optimize=1 -speed */
/* SH7604 free-running timer setup, plus the two interrupt handlers the ROM
 * vector table points at: vector 0x42 (IRQ level 4, vblank) is irq_vblank and
 * 0x46 is irq_nop. MAME's psikyosh driver notes the vblank handler writes 0x00
 * to 0x0405ffdd on entry and 0xc0 on exit. The on-chip registers aren't
 * declared volatile; the counters the handlers share with the game are. */
#define FRT_TIER  (*(unsigned char *)0xfffffe10)
#define FRT_FTCSR (*(unsigned char *)0xfffffe11)
#define FRT_FRCH  (*(unsigned char *)0xfffffe12)
#define FRT_FRCL  (*(unsigned char *)0xfffffe13)
#define FRT_OCRH  (*(unsigned char *)0xfffffe14)
#define FRT_OCRL  (*(unsigned char *)0xfffffe15)
#define FRT_TCR   (*(unsigned char *)0xfffffe16)
#define IRQ_ACK   (*(unsigned char *)0x2405ffdd)  /* cache-through 0x0405ffdd */

#pragma interrupt(irq_nop, irq_vblank)

extern volatile long g_6060028;
extern unsigned short g_6060024;
extern volatile long g_6060030;
extern volatile long g_606002c;

void frt_init(void)
{
	FRT_TIER = 1;
	FRT_FRCH = 0;
	FRT_FRCL = 0;
	FRT_FTCSR = 1;
	FRT_TCR = 2;
	g_6060028 = 0;
	g_6060024 = 0xffff;
	FRT_OCRH = 0xff;
	FRT_OCRL = 0xff;
}

void frt_set_compare(unsigned short v)
{
	FRT_TIER = 1;
	FRT_FRCH = 0;
	FRT_FRCL = 0;
	g_6060028 = 0;
	g_6060024 = v;
	FRT_OCRH = g_6060024 >> 8;
	FRT_OCRL = g_6060024;
	FRT_TIER = 9;
}

void irq_nop(void)
{
}

void irq_vblank(void)
{
	IRQ_ACK = 0;
	g_6060030 = 1;
	g_606002c++;
	IRQ_ACK = 0xc0;
}
