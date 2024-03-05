/*
 * Board specific definitions
 *
 *  Date: 27 Jan. 2024
 *  Author: xaviergr
 */

#ifndef _BOARD_H
#define _BOARD_H

#define CLOCK_SYS_CLOCK_HZ	24000000

#if UART_PRINTF_MODE
#define DEBUG_INFO_TX_PIN	GPIO_SWS
#endif

#define SHL_ADC_VBAT		B3P
#define GPIO_VBAT		GPIO_PB3
#define PB3_FUNC		AS_GPIO
#define PB3_INPUT_ENABLE	1
#define PB3_OUTPUT_ENABLE	1
#define PB3_DATA_OUT		1

#define LED_ON			1
#define LED_OFF			0
#define GPIO_LED		GPIO_PB4
#define PB4_FUNC		AS_GPIO
#define PB4_OUTPUT_ENABLE	1
#define PB4_INPUT_ENABLE	0
#define PB4_DATA_OUT		LED_OFF
#define PULL_WAKEUP_SRC_PB4	PM_PIN_PULLDOWN_100K

#if BOARD_VARIANT == 1
// Tuya TS0202 by TZ3000_6ygjfyll - TLSR825x 1M Flash
// GPIO_PA7 - SWS programming
// GPIO_PB3 - Free ADC pin for battery voltage measurement
// GPIO_PB4 - LED (to GND)
// GPIO_PC0 - BUTTON (to GND)
// GPIO_PD7 - PIR REL motion sense
// GPIO_PA0 - PIR ONTIME clear delay
// GPIO_PA1 - PIR ONTIME clear delay
// GPIO_PD3 - PIR ONTIME clear delay

// BS612 PIR motion sensor pinout
// SENS   - GND (PULLDOWN_10K)
// OEN    - VDD (PULLUP_1M)
// REL    - GPIO_PD7
// ONTIME - Resistor network (GPIO_PD3, GPIO_PA0, GPIO_PA1)

#define LED_ON			1
#define LED_OFF			0
#define GPIO_LED		GPIO_PB4
#define PB4_FUNC		AS_GPIO
#define PB4_OUTPUT_ENABLE	1
#define PB4_INPUT_ENABLE	0
#define PB4_DATA_OUT		LED_OFF
#define PULL_WAKEUP_SRC_PB4	PM_PIN_PULLDOWN_100K

#define BUTTON			GPIO_PC0
#define PC0_FUNC		AS_GPIO
#define PC0_OUTPUT_ENABLE	0
#define PC0_INPUT_ENABLE	1
#define	PULL_WAKEUP_SRC_PC0	PM_PIN_PULLUP_10K

#define PIR_SENSE		GPIO_PD7
#define PD7_FUNC		AS_GPIO
#define PD7_OUTPUT_ENABLE	0
#define PD7_INPUT_ENABLE	1
#define PULL_WAKEUP_SRC_PD7	PM_PIN_PULLDOWN_100K

#define PD3_FUNC		AS_GPIO
#define PD3_OUTPUT_ENABLE	1
#define PD3_INPUT_ENABLE	0
#define PULL_WAKEUP_SRC_PD3	PM_PIN_PULLDOWN_100K
#define PD3_DATA_OUT		0

#define PA0_FUNC		AS_GPIO
#define PA0_OUTPUT_ENABLE	1
#define PA0_INPUT_ENABLE	0
#define PULL_WAKEUP_SRC_PA0	PM_PIN_PULLDOWN_100K
#define PA0_DATA_OUT		0

#define PA1_FUNC		AS_GPIO
#define PA1_OUTPUT_ENABLE	1
#define PA1_INPUT_ENABLE	0
#define PULL_WAKEUP_SRC_PA1	PM_PIN_PULLDOWN_100K
#define PA1_DATA_OUT		0

#elif BOARD_VARIANT == 2

// Tuya TS0202 by TZ3040_bb6xaihh - TLSR825x 1M Flash
// GPIO_PA7 - SWS, free, (debug TX)
// GPIO_PC2 - PIR REL (sense)
// GPIO_PB4 - LED (to GND)
// GPIO_PB3 - Battery sense
// GPIO_PC3 - BUTTON (to GND)
// ONTIME seems fixed at 60 seconds

#define BUTTON			GPIO_PC3
#define PC3_FUNC		AS_GPIO
#define PC3_OUTPUT_ENABLE	0
#define PC3_INPUT_ENABLE	1
#define	PULL_WAKEUP_SRC_PC3	PM_PIN_PULLUP_10K

#define PIR_SENSE		GPIO_PC2
#define PC2_FUNC		AS_GPIO
#define PC2_OUTPUT_ENABLE	0
#define PC2_INPUT_ENABLE	1
#define PULL_WAKEUP_SRC_PC2	PM_PIN_PULLDOWN_100K

#else
#error Wrong board variant!
#endif // BOARD_VARIANT
#endif
