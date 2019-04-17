#define INTEL_VGA_DEVICE(id, info) {	\
	0x8086, id,			\
	~0, ~0,				\
	0x030000, 0xff0000,		\
	(unsigned long) info }

#define INTEL_BXT_IDS(info) \
	INTEL_VGA_DEVICE(0x0A84, info), \
	INTEL_VGA_DEVICE(0x1A84, info), \
	INTEL_VGA_DEVICE(0x1A85, info), \
	INTEL_VGA_DEVICE(0x5A84, info), /* APL HD Graphics 505 */ \
	INTEL_VGA_DEVICE(0x5A85, info)  /* APL HD Graphics 500 */
