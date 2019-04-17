#include "reg_util.h"
#include "intel_chipset.h"

#define INTEL_RANGE_RSVD        (0<<0) /*  Shouldn't be read or written */
#define INTEL_RANGE_READ        (1<<0)
#define INTEL_RANGE_WRITE       (1<<1)
#define INTEL_RANGE_RW          (INTEL_RANGE_READ | INTEL_RANGE_WRITE)
#define INTEL_RANGE_END         (1<<31)

struct reg_register_range {
        uint32_t base;
        uint32_t size;
        uint32_t flags;
};

struct reg_register_map {
        struct reg_register_range *map;
        uint32_t top;
        uint32_t alignment_mask;
};

/* The documentation is a little sketchy on these register ranges. */
static struct reg_register_range gen6_gt_register_map[] = {
        {0x00000000, 0x00000fff, INTEL_RANGE_RW},
        {0x00001000, 0x00000fff, INTEL_RANGE_RSVD},
        {0x00002000, 0x00000fff, INTEL_RANGE_RW},
        {0x00003000, 0x000001ff, INTEL_RANGE_RW},
        {0x00003200, 0x00000dff, INTEL_RANGE_RW},
        {0x00004000, 0x00000fff, INTEL_RANGE_RW},
        {0x00005000, 0x0000017f, INTEL_RANGE_RW},
        {0x00005180, 0x00000e7f, INTEL_RANGE_RW},
        {0x00006000, 0x00001fff, INTEL_RANGE_RW},
        {0x00008000, 0x000007ff, INTEL_RANGE_RW},
        {0x00008800, 0x000000ff, INTEL_RANGE_RSVD},
        {0x00008900, 0x000006ff, INTEL_RANGE_RW},
        {0x00009000, 0x00000fff, INTEL_RANGE_RSVD},
        {0x0000a000, 0x00000fff, INTEL_RANGE_RW},
        {0x0000b000, 0x00004fff, INTEL_RANGE_RSVD},
        {0x00010000, 0x00001fff, INTEL_RANGE_RW},
        {0x00012000, 0x000003ff, INTEL_RANGE_RW},
        {0x00012400, 0x00000bff, INTEL_RANGE_RW},
        {0x00013000, 0x00000fff, INTEL_RANGE_RW},
        {0x00014000, 0x00000fff, INTEL_RANGE_RW},
        {0x00015000, 0x0000cfff, INTEL_RANGE_RW},
        {0x00022000, 0x00000fff, INTEL_RANGE_RW},
        {0x00023000, 0x00000fff, INTEL_RANGE_RSVD},
        {0x00024000, 0x00000fff, INTEL_RANGE_RW},
        {0x00025000, 0x0000afff, INTEL_RANGE_RSVD},
        {0x00030000, 0x0000ffff, INTEL_RANGE_RW},
        {0x00040000, 0x0000ffff, INTEL_RANGE_RW},
        {0x00050000, 0x0000ffff, INTEL_RANGE_RW},
        {0x00060000, 0x0000ffff, INTEL_RANGE_RW},
        {0x00070000, 0x00003fff, INTEL_RANGE_RW},
        {0x00074000, 0x0008bfff, INTEL_RANGE_RSVD},
        {0x00100000, 0x00007fff, INTEL_RANGE_RW},
        {0x00108000, 0x00037fff, INTEL_RANGE_RSVD},
        {0x00140000, 0x0003ffff, INTEL_RANGE_RW},
        {0x00000000, 0x00000000, static_cast<uint32_t>INTEL_RANGE_END}
};

int reg_gen(uint32_t devid)
{
	if(IS_GEN9(devid))
		return 9;
	else
        if(IS_GEN7(devid))
		return 7;
	else
	if(IS_GEN6(devid))
		return 6;
	else
	if(IS_GEN5(devid))
		return 5;
	else
	if(IS_GEN4(devid))
		return 4;
	else
	if(IS_GEN3(devid))
		return 3;
	else
	if(IS_GEN2(devid))
		return 2;
	else
		return -1;
}


struct reg_register_map get_register_map(uint32_t devid)
{
        struct reg_register_map map;
        const int gen = reg_gen(devid);

        if (gen >= 6) {
                map.map = gen6_gt_register_map;
                map.top = 0x180000;
	}
        map.alignment_mask = 0x3;

        return map;
}
