/*
 * LED primitives for signaling events to the user
 *
 *  Date: 25 Feb. 2024
 *  Author: xaviergr
 */

#include <gpio.h>
#include <types.h>
#include <os/ev_timer.h>

#include "led.h"
#include "board.h"

static struct led_blink_cfg_s {
	bool state;
	u16 on_time;
	u16 off_time;
	u8 num_blinks;
	ev_timer_event_t *task;
} blink_cfg;

void led_on(void)
{
	gpio_write(GPIO_LED, LED_ON);
}

void led_off(void)
{
	gpio_write(GPIO_LED, LED_OFF);
}

void led_toggle(void)
{
	gpio_toggle(GPIO_LED);
}

static s32 led_do_blink(void *arg)
{
	struct led_blink_cfg_s *cfg = (struct led_blink_cfg_s *)arg;

	if(cfg->state == false) {
		led_on();
		cfg->state = true;
		return cfg->on_time;
	}

	led_off();
	cfg->state = false;

	if(cfg->num_blinks-- == 0) {
		cfg->task = NULL;
		return -1;
	}

	return cfg->off_time;
}

void led_blink_start(u8 num_blinks, u16 on_time, u16 off_time)
{
	if(blink_cfg.task != NULL) {
		led_blink_stop();
	}

	led_off();

	blink_cfg.state = false;
	blink_cfg.on_time = on_time;
	blink_cfg.off_time = off_time;
	blink_cfg.num_blinks = num_blinks - 1;
	blink_cfg.task = ev_timer_taskPost(led_do_blink, &blink_cfg, 0);
}

void led_blink_stop(void)
{
	if(blink_cfg.task == NULL) {
		return;
	}

	led_off();
	ev_timer_taskCancel(&blink_cfg.task);

	blink_cfg.task = NULL;
	blink_cfg.state = false;
}

bool led_is_blinking(void)
{
	return (blink_cfg.task != NULL);
}

