/*
 * Application related configuration definitions
 *
 *  Date: 27 Jan. 2024
 *  Author: xaviergr
 */

#ifndef _APP_CFG_H_
#define _APP_CFG_H_

#include "version_cfg.h"

#define PM_ENABLE				1
#define PA_ENABLE				0
#define	DEBUG_ENABLE				0
#define USB_PRINTF_MODE				0
#define	UART_PRINTF_MODE			0
#define VOLTAGE_DETECT_ENABLE			0
#define VOLTAGE_DETECT_ADC_PIN			GPIO_VBAT

#define NV_ENABLE				1
#define SECURITY_ENABLE				1
#define DEFAULT_CHANNEL				11

#define	ZCL_CLUSTER_NUM_MAX			4
#define ZCL_REPORTING_TABLE_NUM			0
#define	ZCL_SCENE_TABLE_NUM			0

#define ZB_ED_ROLE				1
#define ZB_MAC_RX_ON_WHEN_IDLE			0

#define APS_GROUP_TABLE_NUM			0
#define APS_BINDING_TABLE_NUM			0

#include "board.h"

#define ZCL_OTA_SUPPORT				0
#define ZCL_GROUP_SUPPORT			0
#define ZCL_IAS_ZONE_SUPPORT			0
#define ZCL_POWER_CFG_SUPPORT			0
#define ZCL_POLL_CTRL_SUPPORT			0
#define ZCL_OCCUPANCY_SENSING_SUPPORT		0
#define ZCL_TEMPERATURE_MEASUREMENT_SUPPORT	0

#define TOUCHLINK_SUPPORT			0
#define REJOIN_FAILURE_TIMER			1
#define DEFAULT_POLL_RATE_MS			3000

typedef enum{
	EV_POLL_ED_DETECT,
	EV_POLL_IDLE,
	EV_POLL_MAX,
}ev_poll_e;

#endif
