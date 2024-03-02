/*
 * Application start and initialization
 *
 *  Date: 27 Jan. 2024
 *  Author: xaviergr
 */

#include <zb_api.h>
#include <zb_common.h>
#include <drivers/drv_nv.h>

#include "clusters.h"
#include "bdb_init.h"

static void stack_init(void)
{
	af_simple_descriptor_t *descr = get_device_descriptor();

	zb_init();
	zb_zdoCbRegister(get_protocol_callbacks());

	af_powerDescPowerModeUpdate(POWER_MODE_RECEIVER_COMES_WHEN_STIMULATED);
	zcl_init(zcl_process_incoming_msg);

	af_endpointRegister(ENDPOINT_ID, descr, zcl_rx_handler, NULL);
	zcl_register(ENDPOINT_ID, get_number_of_clusters(), get_cluster_list());

	bdb_callbacks_init(descr);
}

static void app_init(bool memory_retained)
{
	if(memory_retained) {
		mac_phyReconfig();
		zb_setPollRate(DEFAULT_POLL_RATE_MS);
	} else {
		stack_init();
	}
}

int main(void)
{
	bool memory_retained = false;

	if(drv_platform_init() == SYSTEM_DEEP_RETENTION) {
		memory_retained = true;
	}

	os_init(memory_retained);
	app_init(memory_retained);
	drv_enable_irq();

	while(true) {
		ev_main();
		tl_zbTaskProcedure();
	}
}
