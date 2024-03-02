/*
 * ZigBee Base Device Behavior (BDB) initialization
 *
 *  Date: 24 Feb. 2024
 *  Author: xaviergr
 */

#include <zb_api.h>
#include <zb_common.h>

#define REJOIN_FAIL_RETRY_SLEEP_DURATION (180 * 1000)

static ev_timer_event_t *steer_task;
static ev_timer_event_t *rejoin_task;
static bdb_commissionSetting_t bdb_settings;

s32 do_network_steer(void *arg)
{
	bdb_networkSteerStart();
	steer_task = NULL;
	return -1;
}

s32 do_rejoin(void *arg)
{
	if(zb_isDeviceFactoryNew()) {
		rejoin_task = NULL;
		return -1;
	}

	zb_rejoinReqWithBackOff(zb_apsChannelMaskGet(), g_bdbAttrs.scanDuration);
	return 0;
}

void bdb_do_init(u8 status, bool joined_network)
{
	if(status != BDB_INIT_STATUS_SUCCESS) {
		if(joined_network) {
			u8 scan_ms = g_bdbAttrs.scanDuration;
			zb_rejoinReqWithBackOff(zb_apsChannelMaskGet(), scan_ms);
		}

		return;
	}

	if(joined_network) {
		zb_setPollRate(DEFAULT_POLL_RATE_MS);
	} else {
		if(steer_task) {
			ev_timer_taskCancel(&steer_task);
		}

		u32 sleep_ms = (zb_random() % 0x0fff) + 1;
		steer_task = ev_timer_taskPost(do_network_steer, NULL, sleep_ms);
	}
}

void bdb_commission(u8 status, void *arg)
{
	switch(status) {
	case BDB_COMMISSION_STA_SUCCESS:
		zb_setPollRate(DEFAULT_POLL_RATE_MS);

		if(steer_task) {
			ev_timer_taskCancel(&steer_task);
		}

		if(rejoin_task) {
			ev_timer_taskCancel(&rejoin_task);
		}
		break;
	case BDB_COMMISSION_STA_NO_NETWORK:
	case BDB_COMMISSION_STA_TARGET_FAILURE:
	case BDB_COMMISSION_STA_TCLK_EX_FAILURE:
		if(steer_task) {
			ev_timer_taskCancel(&steer_task);
		}

		u32 sleep_ms = (zb_random() % 0x0fff) + 60000;
		steer_task = ev_timer_taskPost(do_network_steer, NULL, sleep_ms);
		break;
	case BDB_COMMISSION_STA_NO_SCAN_RESPONSE:
	case BDB_COMMISSION_STA_PARENT_LOST:
		do_rejoin(NULL);
		break;
	case BDB_COMMISSION_STA_REJOIN_FAILURE:
		if(zb_isDeviceFactoryNew() == false) {
			u32 sleep_ms = REJOIN_FAIL_RETRY_SLEEP_DURATION;
			rejoin_task = ev_timer_taskPost(do_rejoin, NULL, sleep_ms);
		}
		break;
	default:
		break;
	}
}

static bdb_appCb_t bdb_callbacks =
{
	bdb_do_init,
	bdb_commission,
	NULL,
	NULL
};

void bdb_callbacks_init(af_simple_descriptor_t *device_descriptor)
{
	bool repower = drv_pm_deepSleep_flag_get() ? false : true;
	bdb_init(device_descriptor, &bdb_settings, &bdb_callbacks, repower);
}
