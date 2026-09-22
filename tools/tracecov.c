/*
 * Reduce a MAME debugger trace ("XXXXXXXX: MNEMONIC ...") to coverage data.
 *
 * Reads a trace on stdin and merges it into two bitmaps covering ROM
 * (0x00000000-0x000FFFFF) and work RAM (0x06000000-0x060FFFFF):
 *   - executed:     every PC seen
 *   - call target:  the PC reached after a JSR/BSR/BSRF delay slot
 * The bitmaps persist in <prefix>.exec.bin / <prefix>.call.bin so separate
 * trace windows accumulate. Text listings are written with -d.
 *
 * usage: tracecov <prefix> < trace.log      (merge)
 *        tracecov -d <prefix>               (dump <prefix>.exec.txt / .call.txt)
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SPAN 0x100000u
#define RAM_BASE 0x06000000u
#define BITS (2 * SPAN / 2) /* one bit per halfword, ROM then RAM */

static uint8_t exec_map[BITS / 8], call_map[BITS / 8];

static long slot(uint32_t pc)
{
	if (pc < SPAN)
		return pc >> 1;
	if (pc >= RAM_BASE && pc < RAM_BASE + SPAN)
		return (SPAN >> 1) + ((pc - RAM_BASE) >> 1);
	return -1;
}

static uint32_t slot_pc(long i)
{
	return i < (long)(SPAN >> 1) ? (uint32_t)i << 1 : RAM_BASE + ((uint32_t)(i - (SPAN >> 1)) << 1);
}

static void load(const char *prefix, const char *kind, uint8_t *map)
{
	char path[1024];
	snprintf(path, sizeof path, "%s.%s.bin", prefix, kind);
	FILE *f = fopen(path, "rb");
	if (f) {
		fread(map, 1, BITS / 8, f);
		fclose(f);
	}
}

static void save(const char *prefix, const char *kind, const uint8_t *map)
{
	char path[1024];
	snprintf(path, sizeof path, "%s.%s.bin", prefix, kind);
	FILE *f = fopen(path, "wb");
	if (!f || fwrite(map, 1, BITS / 8, f) != BITS / 8) {
		perror(path);
		exit(1);
	}
	fclose(f);
}

static long dump(const char *prefix, const char *kind, const uint8_t *map)
{
	char path[1024];
	long n = 0;
	snprintf(path, sizeof path, "%s.%s.txt", prefix, kind);
	FILE *f = fopen(path, "w");
	if (!f) {
		perror(path);
		exit(1);
	}
	for (long i = 0; i < BITS; i++)
		if (map[i >> 3] & (1 << (i & 7))) {
			fprintf(f, "%08X\n", slot_pc(i));
			n++;
		}
	fclose(f);
	return n;
}

static int hexval(int c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	return -1;
}

int main(int argc, char **argv)
{
	if (argc == 3 && !strcmp(argv[1], "-d")) {
		load(argv[2], "exec", exec_map);
		load(argv[2], "call", call_map);
		printf("executed halfwords: %ld\n", dump(argv[2], "exec", exec_map));
		printf("call targets:       %ld\n", dump(argv[2], "call", call_map));
		return 0;
	}
	if (argc != 2) {
		fprintf(stderr, "usage: tracecov <prefix> < trace | tracecov -d <prefix>\n");
		return 2;
	}
	load(argv[1], "exec", exec_map);
	load(argv[1], "call", call_map);

	static char line[512];
	int pending = 0; /* 2 = saw call, 1 = saw delay slot, next PC is target */
	unsigned long lines = 0;
	while (fgets(line, sizeof line, stdin)) {
		uint32_t pc = 0;
		int i;
		for (i = 0; i < 8; i++) {
			int v = hexval(line[i]);
			if (v < 0)
				break;
			pc = pc << 4 | (uint32_t)v;
		}
		if (i != 8 || line[8] != ':')
			continue; /* loop summaries, blank lines */
		lines++;
		long s = slot(pc);
		if (pending == 1 && s >= 0)
			call_map[s >> 3] |= 1 << (s & 7);
		if (pending)
			pending--;
		if (s >= 0)
			exec_map[s >> 3] |= 1 << (s & 7);
		const char *m = line + 10;
		if (!strncmp(m, "JSR ", 4) || !strncmp(m, "BSR ", 4) || !strncmp(m, "BSRF ", 5))
			pending = 2;
	}
	save(argv[1], "exec", exec_map);
	save(argv[1], "call", call_map);
	fprintf(stderr, "merged %lu trace lines\n", lines);
	return 0;
}
