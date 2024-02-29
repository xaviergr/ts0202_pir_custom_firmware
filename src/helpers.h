/*
 * Definitions for various common helper functions
 *
 *  Date: 29 Feb. 2024
 *  Author: xaviergr
 */

#ifndef _HELPERS_H_
#define _HELPERS_H_

#include <types.h>
#include <drivers/drv_nv.h>

static inline int flash_write_value(u8 item_id, u16 len, void *buf)
{
	return nv_flashWriteNew(1, NV_MODULE_APP, item_id, len, buf);
}

static inline int flash_read_value(u8 item_id, u16 len, void *buf)
{
	int rc = nv_flashReadNew(1, NV_MODULE_APP, item_id, len, buf);

	/* If the item doesn't exist (e.g. first boot), create it. */
	if(rc == NV_ITEM_NOT_FOUND) {
		flash_write_value(item_id, len, buf);
	}

	return rc;
}

#endif
