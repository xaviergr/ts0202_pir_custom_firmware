/*
 * Application start and initialization
 *
 *  Date: 27 Jan. 2024
 *  Author: xaviergr
 */

#include <zb_api.h>
#include <zb_common.h>
#include <drivers/drv_nv.h>

#include "button.h"
#include "battery.h"
#include "clusters.h"
#include "bdb_init.h"
#include "occupancy.h"

static void enter_low_power(void)
{
	u32 irq_state = drv_disable_irq();
	u32 sleep_time = OVERRIDE_POLL_RATE_MS;
	drv_pm_wakeup_src_e wake_src = PM_WAKEUP_SRC_PAD;
	ev_timer_event_t *next_task = ev_timer_nearestGet();
	drv_pm_sleep_mode_e sleep_mode = PM_SLEEP_MODE_DEEPSLEEP;

	if(next_task) {
		/* Override the next event if it is the default poll rate task.
		 * It means that the device is idle and we can sleep for longer. */
		if (next_task->period >= DEFAULT_POLL_RATE_MS) {
			next_task->timeout = 0;

			/* Don't deep sleep while processing occupancy. */
			if(get_occupancy_attributes()->occupancy) {
				sleep_mode = PM_SLEEP_MODE_DEEP_WITH_RETENTION;
				wake_src |= PM_WAKEUP_SRC_TIMER;
			}
		} else {
			/* Normal task. */
			sleep_mode = PM_SLEEP_MODE_DEEP_WITH_RETENTION;
			sleep_time = next_task->timeout;
			wake_src |= PM_WAKEUP_SRC_TIMER;
		}
	}

	/* Don't deep sleep when haven't joined the network. */
	if (zb_isDeviceJoinedNwk() == false) {
		wake_src = PM_WAKEUP_SRC_PAD | PM_WAKEUP_SRC_TIMER;
		sleep_mode = PM_SLEEP_MODE_DEEP_WITH_RETENTION;
	}

	rf_paShutDown();
	if(sleep_mode == PM_SLEEP_MODE_DEEPSLEEP) {
		drv_pm_deepSleep_frameCnt_set(ss_outgoingFrameCntGet());
	}

	if(sleep_time <= PM_NORMAL_SLEEP_MAX) {
		drv_pm_sleep(sleep_mode, wake_src, sleep_time);
	} else {
		drv_pm_longSleep(sleep_mode, wake_src, sleep_time);
	}

	drv_restore_irq(irq_state);
}

void main_task(void)
{
	check_button_status();

	if(bdb_isIdle() && zb_isDeviceJoinedNwk()) {
		battery_report();
		occupancy_report();
	}

	if(tl_stackBusy() || !zb_isTaskDone()) {
		return;
	}

	apsCleanToStopSecondClock();
	enter_low_power();
}

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
		battery_init();
		occupancy_init();

		ev_on_poll(EV_POLL_IDLE, main_task);
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
