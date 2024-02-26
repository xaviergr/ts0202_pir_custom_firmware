/*
 * Board specific definitions
 *
 *  Date: 27 Jan. 2024
 *  Author: xaviergr
 */

#ifndef _BOARD_H
#define _BOARD_H

// Tuya TS0202 by _TZ3000_6ygjfyll - TLSR825x 1M Flash
// GPIO_PA7 - SWS programming
// GPIO_PB4 - LED (to GND)

#define CLOCK_SYS_CLOCK_HZ	24000000

#define LED_ON			1
#define LED_OFF			0
#define GPIO_LED		GPIO_PB4
#define PB4_FUNC		AS_GPIO
#define PB4_OUTPUT_ENABLE	1
#define PB4_INPUT_ENABLE	0
#define PB4_DATA_OUT		LED_OFF
#define PULL_WAKEUP_SRC_PB4	PM_PIN_PULLDOWN_100K

#define SHL_ADC_VBAT		B3P
#define GPIO_VBAT		GPIO_PB3
#define PB3_FUNC		AS_GPIO
#define PB3_INPUT_ENABLE	1
#define PB3_OUTPUT_ENABLE	1
#define PB3_DATA_OUT		1

#if UART_PRINTF_MODE
#define DEBUG_INFO_TX_PIN	GPIO_SWS
#endif

#endif
