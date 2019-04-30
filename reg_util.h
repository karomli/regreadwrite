#ifndef REG_READWRITE_TOOLS_H
#define REG_READWRITE_TOOLS_H

#include <iostream>
#include <stdint.h>
#include <sys/types.h>
#include <pciaccess.h>
#include <assert.h>

#include "reg_map.h"

void *mmio;

static struct _mmio_data {
        int inited;
        bool safe;
        char debugfs_path[FILENAME_MAX];
        char debugfs_forcewake_path[FILENAME_MAX];
        uint32_t i915_devid;
        struct reg_register_map map;
        int key;
} mmio_data;

void get_mmio(struct pci_device *pci_dev) {
        uint32_t devid, gen;
        int mmio_bar, mmio_size;
        int error;

	mmio_bar = 0;

	mmio_size = 2*1024*1024;

        error = pci_device_map_range (pci_dev,
		pci_dev->regions[mmio_bar].base_addr, mmio_size, 
		PCI_DEV_MAP_FLAG_WRITABLE, &mmio);
}

static int get_forcewake_lock(void)
{
        return open(mmio_data.debugfs_forcewake_path, 0);
}

static void release_forcewake_lock(int fd)
{
        close(fd);
}

int reg_access_init(struct pci_device *pci_dev, int safe) {
	int ret;

	/* after old API is deprecated, remove this */
	if (mmio == NULL) {
		get_mmio(pci_dev);
	}

	assert(mmio != NULL);

	if (mmio_data.inited)
                return -1;

	mmio_data.safe = safe != 0 ? true : false;
	mmio_data.i915_devid = pci_dev->device_id;
	if (mmio_data.safe)
		mmio_data.map = get_register_map(mmio_data.i915_devid);

	mmio_data.key = get_forcewake_lock();

        mmio_data.inited++;
        return 0;
	
}

void reg_access_fini(void) { 
	if (mmio_data.key)
		release_forcewake_lock(mmio_data.key);
	mmio_data.inited--;
}

struct pci_device *get_pci_device(void) {

	struct pci_device *pci_dev = NULL;
        int error;

        error = pci_system_init();
        if (error != 0) {
                fprintf(stderr, "Couldn't initialize PCI system: %s\n",
                        strerror(error));
		return pci_dev;
//                exit(1);
        }

        /* Grab the graphics card. Try the canonical slot first, then
         * walk the entire PCI bus for a matching device. */
        pci_dev = pci_device_find_by_slot(0, 0, 2, 0);
        if (pci_dev == NULL || pci_dev->vendor_id != 0x8086) {
	
                struct pci_device_iterator *iter;
                struct pci_id_match match;

                match.vendor_id = 0x8086; /* Intel */
                match.device_id = PCI_MATCH_ANY;
                match.subvendor_id = PCI_MATCH_ANY;
                match.subdevice_id = PCI_MATCH_ANY;

                match.device_class = 0x3 << 16;
                match.device_class_mask = 0xff << 16;

                match.match_data = 0;

                iter = pci_id_match_iterator_create(&match);
                pci_dev = pci_device_next(iter);
                pci_iterator_destroy(iter);
        }
        if (pci_dev == NULL)
                errx(1, "Couldn't find graphics card");

        error = pci_device_probe(pci_dev);
        if (error != 0) {
                fprintf(stderr, "Couldn't probe graphics card: %s\n",
                        strerror(error));
                exit(1);
        }

        if (pci_dev->vendor_id != 0x8086)
                errx(1, "Graphics card is non-intel");

        return pci_dev;
}
#endif /*REG_READWRITE_TOOLS_H*/
