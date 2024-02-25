/*
 * Versioning definitions (firmware, chip, board, etc.)
 *
 *  Date: 27 Jan. 2024
 *  Author: xaviergr
 */

#ifndef _VERSION_CFG_H_
#define _VERSION_CFG_H_

#define TLSR_8267			0
#define TLSR_8269			1
#define TLSR_8258_512K			2
#define TLSR_8258_1M			3
#define TLSR_8278			4
#define TLSR_B91			5

#define BOARD				0
#define CHIP_TYPE			TLSR_8258_1M

#define RESV_FOR_APP_RAM_CODE_SIZE	0
#define APP_IMAGE_ADDR			0
#define BOOT_LOADER_MODE		0
#define IS_BOOT_LOADER_IMAGE		BOOT_LOADER_MODE
#define IMAGE_OFFSET			APP_IMAGE_ADDR

#define APP_RELEASE			0x10
#define APP_BUILD			0x01
#define STACK_RELEASE			0x30
#define STACK_BUILD			0x01

#define MANUFACTURER_CODE_TELINK	0x1141
#define IMAGE_TYPE			((CHIP_TYPE << 8) | BOARD)
#define APP_VERSION			((APP_RELEASE << 24) | (APP_BUILD << 16))
#define STACK_VERSION			((STACK_RELEASE << 8) | STACK_BUILD)
#define FILE_VERSION			(APP_VERSION | STACK_VERSION)

#define ZCL_BASIC_MODEL_ID		{6,'T','S','0','2','0','2'}
#define ZCL_BASIC_SW_BUILD_ID		APP_BUILD
#define ZCL_BASIC_SW_DATE_CODE		__DATE__
#define ZCL_BASIC_MFG_NAME \
{ \
	17, '_', 'T', 'Z', '3', '0', '0', '0', \
	'_', '6', 'y', 'g', 'j', 'f', 'y', 'l', 'l', '!' \
}

#endif

