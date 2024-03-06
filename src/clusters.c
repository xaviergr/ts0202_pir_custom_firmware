/*
 * Accessors and initialization of the impelemented ZigBee Clusters
 *
 *  Date: 24 Feb. 2024
 *  Author: xaviergr
 */

#include "clusters.h"
#include "occupancy.h"

#define TOSTRING(x)		STRINGIFY(x)
#define SW_BUILD_ID_STR		TOSTRING(APP_BUILD)
#define ZCL_BASIC_MFG_STR	TOSTRING(ZCL_BASIC_MFG_NAME)
#define ZCL_BASIC_MODEL_STR	TOSTRING(ZCL_BASIC_MODEL_ID)
#define ZCL_BASIC_URL_STR	TOSTRING(ZCL_BASIC_PRODUCT_URL)

#define APP_DEV_VERSION		1
#define RESERVED		0

#define APP_DEV_VERSION		1
#define ZCL_VERSION		3
#define ZIGBEE_STACK_VERSION	2
#define HW_VERSION		1
#define RESERVED		0

#define DEFAULT_OCCUPANCY_DELAY	5
#define PIR_SENSORY_TYPE	0

#define NUM_ELEMENTS_OF(x)	(sizeof(x) / sizeof(x[0]))

#define ZCL_BASIC_ATTR_NUM	sizeof(basic_cluster_config) / sizeof(zclAttrInfo_t)
#define ZCL_POWER_CFG_ATTR_NUM	sizeof(power_cluster_config) / sizeof(zclAttrInfo_t)
#define ZCL_OCCUPANCY_ATTR_NUM	sizeof(occupancy_cluster_config) / sizeof(zclAttrInfo_t)

#define ZCL_ATTRID_BASIC_URL	0xB

static u8 cluster_revision = ZCL_ATTR_GLOBAL_CLUSTER_REVISION_DEFAULT;

static u16 output_cluster_list[] = {};
static u16 input_cluster_list[] = {
	ZCL_CLUSTER_GEN_BASIC,
	ZCL_CLUSTER_GEN_POWER_CFG,
	ZCL_CLUSTER_MS_OCCUPANCY_SENSING,
};

static af_simple_descriptor_t device_descriptor = {
	HA_PROFILE_ID,
	HA_DEV_OCCUPANCY_SENSOR,
	ENDPOINT_ID,
	APP_DEV_VERSION,
	RESERVED,
	NUM_ELEMENTS_OF(input_cluster_list),
	NUM_ELEMENTS_OF(output_cluster_list),
	input_cluster_list,
	output_cluster_list,
};

zcl_basicAttr_t basic_attributes = {
	.device_enabled		= TRUE,
	.hw_version		= HW_VERSION,
	.zcl_version		= ZCL_VERSION,
	.app_version		= APP_DEV_VERSION,
	.stack_version		= ZIGBEE_STACK_VERSION,
	.power_source		= POWER_SOURCE_BATTERY,
	.sw_build.sub		= { sizeof(SW_BUILD_ID_STR), SW_BUILD_ID_STR },
	.date_code.sub		= { sizeof(__DATE__), ZCL_BASIC_SW_DATE_CODE },
	.manufacturer.sub	= { sizeof(ZCL_BASIC_MFG_STR), ZCL_BASIC_MFG_STR },
	.product_url.sub	= { sizeof(ZCL_BASIC_URL_STR), ZCL_BASIC_URL_STR },
	.model_id.sub		= { sizeof(ZCL_BASIC_MODEL_STR), ZCL_BASIC_MODEL_STR },
};

const zclAttrInfo_t basic_cluster_config[] =
{
	{
		ZCL_ATTRID_BASIC_ZCL_VER,
		ZCL_DATA_TYPE_UINT8,
		ACCESS_CONTROL_READ,
		&basic_attributes.zcl_version,
	},
	{
		ZCL_ATTRID_BASIC_APP_VER,
		ZCL_DATA_TYPE_UINT8,
		ACCESS_CONTROL_READ,
		&basic_attributes.app_version,
	},
	{
		ZCL_ATTRID_BASIC_STACK_VER,
		ZCL_DATA_TYPE_UINT8,
		ACCESS_CONTROL_READ,
		&basic_attributes.stack_version,
	},
	{
		ZCL_ATTRID_BASIC_HW_VER,
		ZCL_DATA_TYPE_UINT8,
		ACCESS_CONTROL_READ,
		&basic_attributes.hw_version,
	},
	{
		ZCL_ATTRID_BASIC_MFR_NAME,
		ZCL_DATA_TYPE_CHAR_STR,
		ACCESS_CONTROL_READ,
		basic_attributes.manufacturer.full,
	},
	{
		ZCL_ATTRID_BASIC_MODEL_ID,
		ZCL_DATA_TYPE_CHAR_STR,
		ACCESS_CONTROL_READ,
		basic_attributes.model_id.full,
	},
	{
		ZCL_ATTRID_BASIC_POWER_SOURCE,
		ZCL_DATA_TYPE_ENUM8,
		ACCESS_CONTROL_READ,
		&basic_attributes.power_source,
	},
	{
		ZCL_ATTRID_BASIC_DEV_ENABLED,
		ZCL_DATA_TYPE_BOOLEAN,
		ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,
		&basic_attributes.device_enabled,
	},
	{
		ZCL_ATTRID_BASIC_SW_BUILD_ID,
		ZCL_DATA_TYPE_CHAR_STR,
		ACCESS_CONTROL_READ,
		basic_attributes.sw_build.full,
	},
	{
		ZCL_ATTRID_BASIC_DATE_CODE,
		ZCL_DATA_TYPE_CHAR_STR,
		ACCESS_CONTROL_READ,
		basic_attributes.date_code.full,
	},
	{
		ZCL_ATTRID_BASIC_URL,
		ZCL_DATA_TYPE_CHAR_STR,
		ACCESS_CONTROL_READ,
		basic_attributes.product_url.full,
	},
	{
		ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,
		ZCL_DATA_TYPE_UINT16,
		ACCESS_CONTROL_READ,
		&cluster_revision,
	},
};

static zcl_powerAttr_t power_attributes = {};

static zclAttrInfo_t power_cluster_config[] = {
	{
		ZCL_ATTRID_BATTERY_VOLTAGE,
		ZCL_DATA_TYPE_UINT16,
		ACCESS_CONTROL_READ,
		(u8 *)&power_attributes.battery_voltage
	},
	{
		ZCL_ATTRID_BATTERY_PERCENTAGE_REMAINING,
		ZCL_DATA_TYPE_UINT16,
		ACCESS_CONTROL_READ,
		(u8 *)&power_attributes.battery_percentage
	},
	{
		ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,
		ZCL_DATA_TYPE_UINT16,
		ACCESS_CONTROL_READ,
		&cluster_revision
	},
};

static zcl_occupancyAttr_t occupancy_attributes = {
	.occupancy	= 0,
	.sensor_type	= 0,
	.clear_delay	= DEFAULT_OCCUPANCY_DELAY,
};

const zclAttrInfo_t occupancy_cluster_config[] = {
	{
		ZCL_ATTRID_OCCUPANCY,
		ZCL_DATA_TYPE_BITMAP8,
		ACCESS_CONTROL_READ,
		&occupancy_attributes.occupancy,
	},
	{
		ZCL_ATTRID_OCCUPANCY_SENSOR_TYPE,
		ZCL_DATA_TYPE_ENUM8,
		ACCESS_CONTROL_READ,
		&occupancy_attributes.sensor_type,
	},
	{
		ZCL_ATTRID_PIR_OCCU2UNOCCU_DELAY,
		ZCL_DATA_TYPE_UINT16,
		ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,
		(u8*)&occupancy_attributes.clear_delay,
	},
	{
		ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,
		ZCL_DATA_TYPE_UINT16,
		ACCESS_CONTROL_READ,
		(u8*)&zcl_attr_global_clusterRevision,
	},
};

static zcl_specClusterInfo_t cluster_list[] = {
	{
		ZCL_CLUSTER_GEN_BASIC,
		MANUFACTURER_CODE_NONE,
		ZCL_BASIC_ATTR_NUM,
		basic_cluster_config,
		zcl_basic_register,
		NULL,
	},
	{
		ZCL_CLUSTER_GEN_POWER_CFG,
		MANUFACTURER_CODE_NONE,
		ZCL_POWER_CFG_ATTR_NUM,
		power_cluster_config,
		zcl_powerCfg_register,
		NULL,
	},
	{
		ZCL_CLUSTER_MS_OCCUPANCY_SENSING,
		MANUFACTURER_CODE_NONE,
		ZCL_OCCUPANCY_ATTR_NUM,
		occupancy_cluster_config,
		zcl_occupancySensing_register,
		NULL,
	},
};

u8 get_number_of_clusters(void)
{
	return NUM_ELEMENTS_OF(cluster_list);
}

zcl_specClusterInfo_t *get_cluster_list(void)
{
	return cluster_list;
}

zcl_powerAttr_t *get_power_attributes(void)
{
	return &power_attributes;
}

zcl_occupancyAttr_t *get_occupancy_attributes(void)
{
	return &occupancy_attributes;
}

static void zcl_handle_write_request_cmd(u16 cluster, zclWriteCmd_t *cmd)
{
	switch(cluster) {
	case ZCL_CLUSTER_MS_OCCUPANCY_SENSING:
		occupancy_handle_write_cmd(cmd);
		break;
	default:
		break;
	}
}

void zcl_process_incoming_msg(zclIncoming_t *message_handle)
{
	u16 cluster = message_handle->msg->indInfo.cluster_id;

	if(message_handle->hdr.cmd == ZCL_CMD_WRITE) {
		zcl_handle_write_request_cmd(cluster, message_handle->attrCmd);
	}
}

int zcl_send_report(u16 cluster_id, u16 attr_id, u16 type, void *buf)
{
	return zcl_sendReportCmd(ENDPOINT_ID, get_destination_endpoint(),
				 TRUE, ZCL_FRAME_SERVER_CLIENT_DIR,
				 cluster_id, attr_id, type, buf);
}

af_simple_descriptor_t *get_device_descriptor(void)
{
	return &device_descriptor;
}

static zdo_appIndCb_t protocol_callbacks = {
	bdb_zdoStartDevCnf,
};

zdo_appIndCb_t *get_protocol_callbacks(void)
{
	return &protocol_callbacks;
}

static epInfo_t destination_endpoint = {
	.dstEp = ENDPOINT_ID,
	.profileId = HA_PROFILE_ID,
	.dstAddr.shortAddr = 0x0000,
	.dstAddrMode = APS_SHORT_DSTADDR_WITHEP,
};

epInfo_t *get_destination_endpoint(void)
{
	return &destination_endpoint;
}
