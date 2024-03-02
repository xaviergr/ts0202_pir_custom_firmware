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

u8 get_number_of_clusters(void);
zcl_specClusterInfo_t *get_cluster_list(void);
void zcl_process_incoming_msg(zclIncoming_t *message_handle);
int zcl_send_report(u16 cluster_id, u16 attr_id, u16 type, void *buf);

epInfo_t *get_destination_endpoint(void);
zdo_appIndCb_t *get_protocol_callbacks(void);
af_simple_descriptor_t *get_device_descriptor(void);

#endif
