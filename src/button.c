/*
 * Button management
 *
 *  Date: 24 Feb. 2024
 *  Author: xaviergr
 */

#include <pm.h>
#include <gpio.h>
#include <timer.h>
#include <types.h>
#include <zcl_include.h>
#include <os/ev_timer.h>
#include <drivers/drv_pm.h>
#include <bdb.h>

#include "led.h"
#include "board.h"
#include "button.h"

#define START_PAIR_MODE_DURATION	(u32)(3000 * 1000)
#define BUTTON_PRESS_POLL_PERIOD	(u32)(500)
#define SLEEP_AFTER_PAIR_DURATION	(u32)(5000)

void check_button_status(void)
{
	static u32 time_of_press;
	static bool last_button_state;
	bool button_pressed = !gpio_read(BUTTON);
	u32 sleep_duration = BUTTON_PRESS_POLL_PERIOD;
	drv_pm_sleep_mode_e sleep_mode = PM_SLEEP_MODE_DEEP_WITH_RETENTION;
	drv_pm_wakeup_src_e wakeup_src = PM_WAKEUP_SRC_PAD | PM_WAKEUP_SRC_TIMER;

	/* Always wakeup when the button changes state. */
	cpu_set_gpio_wakeup(BUTTON, button_pressed, 1);

	if(button_pressed == false) {
		led_off();
		if(last_button_state == true) {
			last_button_state = false;
		}

		return;
	}

	led_on();

	if(last_button_state == false) {
		last_button_state = true;
		time_of_press = clock_time();
	}

	if(clock_time_exceed(time_of_press, START_PAIR_MODE_DURATION)) {
		sleep_duration = SLEEP_AFTER_PAIR_DURATION;
		sleep_mode = PM_SLEEP_MODE_DEEPSLEEP;
		tl_bdbReset2FN();
	}

	drv_pm_sleep(sleep_mode, wakeup_src, sleep_duration);
}
