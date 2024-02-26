/*
 * Battery voltage measurement and reporting
 *
 *  Date: 27 Feb. 2024
 *  Author: xaviergr
 */

#include <types.h>
#include <zb_api.h>
#include <drivers/drv_hw.h>
#include <drivers/drv_adc.h>

#include "helpers.h"
#include "battery.h"
#include "clusters.h"

#define BATTERY_MIN_REPORTABLE_DIFF	3
#define BATTERY_LEVEL_MAX_VAL		200

#define BAT_LVL_OF(x) (((x) - BATTERY_SAFETY_THRESHOLD) / 4)

static u16 get_lvl_from_flash(void)
{
	u16 battery_lvl = 0;
	flash_read_value(NV_ITEM_APP_BATTERY_LVL, sizeof(battery_lvl), &battery_lvl);
	return battery_lvl;
}

void battery_report(void)
{
	static bool reported = false;
	zcl_powerAttr_t *attr = get_power_attributes();

	/* Report only once per non deep sleep cycle. */
	if(reported) {
		return;
	}

	reported = true;

	u16 bat_lvl_diff = abs(get_lvl_from_flash() - attr->battery_percentage);
	if(bat_lvl_diff < BATTERY_MIN_REPORTABLE_DIFF) {
		return;
	}

	zcl_send_report(ZCL_CLUSTER_GEN_POWER_CFG,
			ZCL_ATTRID_BATTERY_PERCENTAGE_REMAINING,
			ZCL_DATA_TYPE_UINT16, &attr->battery_percentage);

	u16 len = sizeof(attr->battery_percentage);
	flash_write_value(NV_ITEM_APP_BATTERY_LVL, len, &attr->battery_percentage);
}

void battery_init(void)
{
	u16 voltage = drv_get_adc_data();
	zcl_powerAttr_t *attr = get_power_attributes();
	u16 battery_level = min(BAT_LVL_OF(voltage), BATTERY_LEVEL_MAX_VAL);

	attr->battery_voltage = voltage;
	attr->battery_percentage = battery_level;
}
