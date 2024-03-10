/*
 * PIR motion sensor and occupancy reporting definitions
 *
 *  Date: 28 Feb. 2024
 *  Author: xaviergr
 */

#ifndef _OCCUPANCY_H_
#define _OCCUPANCY_H_

#include <zcl.h>

void occupancy_init(void);
void occupancy_report(void);
void occupancy_handle_write_cmd(zclWriteCmd_t *cmd);

#endif
