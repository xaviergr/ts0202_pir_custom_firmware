/*
 * Accessors and initialization of the impelemented ZigBee Clusters
 *
 *  Date: 24 Feb. 2024
 *  Author: xaviergr
 */

#include "clusters.h"

#define APP_DEV_VERSION		1
#define RESERVED		0

#define NUM_ELEMENTS_OF(x)	(sizeof(x) / sizeof(x[0]))

static u16 output_cluster_list[] = {};
static u16 input_cluster_list[] = {};

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

static zcl_specClusterInfo_t cluster_list[] = {};

u8 get_number_of_clusters(void)
{
	return NUM_ELEMENTS_OF(cluster_list);
}

zcl_specClusterInfo_t *get_cluster_list(void)
{
	return cluster_list;
}

static void zcl_handle_write_request_cmd(u16 cluster, zclWriteCmd_t *cmd)
{
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
