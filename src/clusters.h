/*
 * Definitions for the impelemented ZigBee Clusters
 *
 *  Date: 24 Feb. 2024
 *  Author: xaviergr
 */

#ifndef _CLUSTERS_H_
#define _CLUSTERS_H_

#include <zcl_include.h>

#define ENDPOINT_ID		1
#define ZCL_BASIC_ATTR_URL_LENGTH	50
#define ZCL_BASIC_ATTR_DEFAULT_LENGTH	32
#define ZCL_BASIC_ATTR_HALF_LENGTH	(ZCL_BASIC_ATTR_DEFAULT_LENGTH / 2)

#define NV_ITEM_APP_BATTERY_LVL		(NV_ITEM_ID_MAX - 1)
#define NV_ITEM_APP_OCCU_DELAY		(NV_ITEM_ID_MAX - 2)

typedef struct {
	u8	zcl_version;
	u8	app_version;
	u8	stack_version;
	u8	hw_version;
	u8	power_source;
	u8	device_enabled;

	union {
		u8 full[ZCL_BASIC_ATTR_DEFAULT_LENGTH];
		struct {
			u8 size;
			u8 str[ZCL_BASIC_ATTR_DEFAULT_LENGTH - 1];
		} sub;
	} manufacturer;

	union {
		u8 full[ZCL_BASIC_ATTR_DEFAULT_LENGTH];
		struct {
			u8 size;
			u8 str[ZCL_BASIC_ATTR_DEFAULT_LENGTH - 1];
		} sub;
	} model_id;

	union {
		u8 full[ZCL_BASIC_ATTR_HALF_LENGTH];
		struct {
			u8 size;
			u8 str[ZCL_BASIC_ATTR_HALF_LENGTH - 1];
		} sub;
	} sw_build;

	union {
		u8 full[ZCL_BASIC_ATTR_HALF_LENGTH];
		struct {
			u8 size;
			u8 str[ZCL_BASIC_ATTR_HALF_LENGTH - 1];
		} sub;
	} date_code;

	union {
		u8 full[ZCL_BASIC_ATTR_URL_LENGTH];
		struct {
			u8 size;
			u8 str[ZCL_BASIC_ATTR_URL_LENGTH - 1];
		} sub;
	} product_url;

} zcl_basicAttr_t;

typedef struct {
	u16 battery_voltage;
	u16 battery_percentage;
} zcl_powerAttr_t;

typedef struct {
	u8  occupancy;
	u8  sensor_type;
	u16 clear_delay;
} zcl_occupancyAttr_t;

zcl_powerAttr_t *get_power_attributes(void);
zcl_occupancyAttr_t *get_occupancy_attributes(void);

u8 get_number_of_clusters(void);
zcl_specClusterInfo_t *get_cluster_list(void);
void zcl_process_incoming_msg(zclIncoming_t *message_handle);
int zcl_send_report(u16 cluster_id, u16 attr_id, u16 type, void *buf);

epInfo_t *get_destination_endpoint(void);
zdo_appIndCb_t *get_protocol_callbacks(void);
af_simple_descriptor_t *get_device_descriptor(void);

#endif
