/*
 * LED primitive definitions
 *
 *  Date: 25 Feb. 2024
 *  Author: xaviergr
 */

#include <types.h>

void led_on(void);
void led_off(void);
void led_toggle(void);

void led_blink_stop(void);
bool led_is_blinking(void);
void led_blink_start(u8 num_blinks, u16 on_time, u16 off_time);
