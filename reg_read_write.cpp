/*
 * Copyright © 2010 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Authors:
 *	Zhenyu Wang <zhenyuw@linux.intel.com>
 *
 */

/*
   Customized by:
	Khairul Anuar Romli <khairul.anuar.bin.romli@gmail.com>
	- Remove full dump support.
	- Add Write register support
*/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "reg_util.h"

static void bit_decode(uint32_t reg)
{
	int i;

	for (i=31; i >= 0; i--)
		printf(" %2d", i);
	printf("\n");

	for (i=31; i >= 0; i--)
		printf(" %2d", (reg & (1 << i)) && 1);
	printf("\n");
}

static void reg_read(uint32_t reg_addr, uint32_t end)
{
	uint32_t i;

	for (i = reg_addr; i < end; i += 4) {
		printf("0x%X : 0x%X\n", i,
		       *(volatile uint32_t *)((volatile char*)mmio + i));
		bit_decode(
		       *(volatile uint32_t *)((volatile char*)mmio + i));
	}
}

static int reg_write(uint32_t reg_addr, uint32_t end, uint32_t reg_val)
{
	uint32_t i;
	for (i = reg_addr; i < end; i +=4) {
		*(volatile uint32_t *)((volatile char*)mmio + i) = reg_val;
	}
}

static void usage(char *cmdname)
{
	printf("Usage: %s [-r|-w] [addr1] [addr2] .. [addrN]\n", cmdname);
	printf("\t -r :read register bits.\n");
	printf("\t -w :write register bits.\n");
	printf("\t addr : in 0xXXXX format\n");
}

int main(int argc, char** argv)
{
	int ret = 0;
	uint32_t reg_addr;
	uint32_t reg_val;
	int i, ch;
	char *cmdname = strdup(argv[0]);
	int full_dump = 0;
	int decode_bits = 0;
	int write_bits = 0;
	int dwords = 1;
	int status = 0;

	while ((ch = getopt(argc, argv, "rwh:")) != -1) {
		switch(ch) {
		case 'r':
			decode_bits = 1;
			break;
		case 'w':
			write_bits = 1;
			break;
		case 'h':
		default:
			usage(cmdname);
			ret = 1;
			goto out;
		}
	}
	argc -= optind;
	argv += optind;

	if (argc < 1) {
		usage(cmdname);
		ret = 1;
		goto out;
	}

	if ((dwords > 1) && (argc != 1 || decode_bits)) {
		usage(cmdname);
		ret = 1;
		goto out;
	}

	if (decode_bits || write_bits ) {
		reg_access_init(get_pci_device(), 0);
		if (decode_bits) {
			for (i=0; i < argc; i++) {
				sscanf(argv[i], "0x%x", &reg_addr);
				reg_read(reg_addr, reg_addr + (dwords * 4));
			}
		}
		if (write_bits) {
			char *endp;
			sscanf(argv[0], "0x%x", &reg_addr);
			sscanf(argv[1], "%x", &reg_val);
			status = reg_write(reg_addr, reg_addr + (dwords *4), reg_val);
		}
		reg_access_fini();
	}

out:
	free(cmdname);
	return ret;
}
