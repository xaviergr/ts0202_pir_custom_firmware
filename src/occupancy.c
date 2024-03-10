/*
 * PIR motion sensor management and occupancy status reporting
 *
 *  Date: 28 Feb. 2024
 *  Author: xaviergr
 */

#include "clusters.h"
#include "occupancy.h"
#include "helpers.h"
#include "board.h"

static ev_timer_event_t *clear_event;

static void report_occupancy_state(bool occupied)
{
	zcl_occupancyAttr_t *attr = get_occupancy_attributes();

	attr->occupancy = occupied;
	zcl_send_report(ZCL_CLUSTER_MS_OCCUPANCY_SENSING, ZCL_ATTRID_OCCUPANCY,
			ZCL_DATA_TYPE_BITMAP8, &attr->occupancy);
}

static int occupancy_cleared_task(void *arg)
{
	bool pir_state = gpio_read(PIR_SENSE);
	cpu_set_gpio_wakeup(PIR_SENSE, !pir_state, 1);

	if(!pir_state) {
		report_occupancy_state(false);
	}

	clear_event = NULL;

	return -1;
}

void occupancy_report(void)
{
	static bool prev_pir_state = false;
	bool pir_state = gpio_read(PIR_SENSE);
	cpu_set_gpio_wakeup(PIR_SENSE, !pir_state, 1);

	if(pir_state == prev_pir_state) {
		return;
	}

	prev_pir_state = pir_state;

	if(pir_state) {
		if(clear_event != NULL) {
			ev_timer_taskCancel(&clear_event);
			return;
		}

		report_occupancy_state(true);
	} else {
		u32 delay = get_occupancy_attributes()->clear_delay * 1000;
		clear_event = ev_timer_taskPost(occupancy_cleared_task, NULL, delay);
	}
}

void occupancy_handle_write_cmd(zclWriteCmd_t *cmd)
{
	u16 new_delay = get_occupancy_attributes()->clear_delay;
	zclWriteRec_t *attr = cmd->attrList;
	u16 len = sizeof(new_delay);
	u16 prev_delay = 0;

	flash_read_value(NV_ITEM_APP_OCCU_DELAY, len, &prev_delay);

	if(attr[0].attrID != ZCL_ATTRID_PIR_OCCU2UNOCCU_DELAY) {
		return;
	}

	if(zcl_getAttrSize(attr->dataType, attr->attrData) != len) {
		return;
	}

	/* Occupancy delay can't be higher than the deep sleep delay. */
	if(new_delay >= DEFAULT_POLL_RATE_MS / 1000) {
		get_occupancy_attributes()->clear_delay = prev_delay;
		return;
	}

	if(prev_delay != new_delay) {
		flash_write_value(NV_ITEM_APP_OCCU_DELAY, len, &new_delay);
	}
}

void occupancy_init(void)
{
	u16 *clear_delay = &get_occupancy_attributes()->clear_delay;
	flash_read_value(NV_ITEM_APP_OCCU_DELAY, sizeof(*clear_delay), clear_delay);
}
