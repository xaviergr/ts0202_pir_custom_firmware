#
# Simple build script for the project and its SDK dependencies
#
#  Date: 27 Jan. 2024
#  Author: xaviergr
#
project(ts0202_custom_firmware C)
cmake_minimum_required(VERSION 3.18)

set(TARGET "ts0202")
set(BIN_TARGET ${TARGET}.bin)
set(ELF_TARGET ${TARGET}.elf)

set(SDK_PATH ${PROJECT_SOURCE_DIR}/SDK)
set(SRC_PATH ${PROJECT_SOURCE_DIR}/src)
set(TOOLCHAIN_PATH ${PROJECT_SOURCE_DIR}/toolchain)

set(SDK_ARCHIVE_PATH "${PROJECT_SOURCE_DIR}/extern/SDK_Zigbee_v3.6.8.6.zip")
if (NOT EXISTS "${SDK_PATH}")
	message(STATUS "Uncompressing SDK...")

	execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${SDK_PATH})
	execute_process(
		COMMAND ${CMAKE_COMMAND} -E tar xzf ${SDK_ARCHIVE_PATH}
		WORKING_DIRECTORY ${SDK_PATH}
	)
endif()

set(TOOLCHAIN_ARCHIVE_PATH "${PROJECT_SOURCE_DIR}/extern/tc32_gcc_v2.0.tar.bz2")
if (NOT EXISTS "${TOOLCHAIN_PATH}")
	message(STATUS "Uncompressing toolchain...")

	execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${TOOLCHAIN_PATH})
	execute_process(
		COMMAND ${CMAKE_COMMAND} -E tar xjf ${TOOLCHAIN_ARCHIVE_PATH}
		WORKING_DIRECTORY ${TOOLCHAIN_PATH}
	)
endif()

# GCC compiler asm and linker options
find_program(COMPILER tc32-elf-gcc toolchain/tc32/bin)
find_program(OBJCOPY tc32-elf-objcopy toolchain/tc32/bin)
set(CMAKE_C_COMPILER ${COMPILER})

list(APPEND GCC_FLAGS -DMCU_CORE_8258=1 -DEND_DEVICE=1 -DMCU_STARTUP_8258=1)
list(APPEND GCC_FLAGS
	-O2 -ffunction-sections -fdata-sections -Wall
	-fpack-struct -fshort-enums -finline-small-functions
	-std=gnu99 -funsigned-char -fshort-wchar -fms-extensions
	-nostartfiles -nostdlib
)

list(APPEND LINK_FLAGS -T${SDK_PATH}/platform/boot/8258/boot_8258.link)
list(APPEND LINK_FLAGS -Lbuild/)
list(APPEND LINK_FLAGS -Wl,--gc-sections -Wl,-nostdlib)
list(APPEND LINK_FLAGS
	-L${SDK_PATH}/platform/lib -L${SDK_PATH}/proj
	-L${SDK_PATH}/zigbee/lib/tc32 -L${SDK_PATH}/platform
)

list(APPEND ASM_FLAGS
	-fomit-frame-pointer -fshort-enums -Wall -Wpacked
	-Wcast-align -fdata-sections -ffunction-sections
	-fno-use-cxa-atexit -fno-rtti -fno-threadsafe-statics
)

# SDK zigbee dependency headers
list(APPEND HDR_FILES
	${SDK_PATH}/zigbee/af
	${SDK_PATH}/zigbee/ota
	${SDK_PATH}/zigbee/zdo
	${SDK_PATH}/zigbee/zcl
	${SDK_PATH}/zigbee/zbapi
	${SDK_PATH}/zigbee/zbhci
	${SDK_PATH}/zigbee/include
	${SDK_PATH}/zigbee/bdb/includes
	${SDK_PATH}/zigbee/common/includes
)

# SDK proj headers
list(APPEND HDR_FILES ${SDK_PATH}/proj ${SDK_PATH}/proj/common)

# SDK platform dependency headers
list(APPEND HDR_FILES
	${SDK_PATH}/platform
	${SDK_PATH}/platform/chip_8258
	${SDK_PATH}/platform/chip_8258/flash
)

# Main project headers (src)
list(APPEND HDR_FILES ${SRC_PATH} ${SRC_PATH}/includes)

# Build SDK zigbee dependencies
add_library(zigbee OBJECT
	${SDK_PATH}/zigbee/bdb/bdb.c
	${SDK_PATH}/zigbee/aps/aps_group.c
	${SDK_PATH}/zigbee/mac/mac_phy.c
	${SDK_PATH}/zigbee/mac/mac_pib.c
	${SDK_PATH}/zigbee/zdo/zdp.c
	${SDK_PATH}/zigbee/zcl/zcl.c
	${SDK_PATH}/zigbee/zcl/zcl_nv.c
	${SDK_PATH}/zigbee/zcl/zcl_reporting.c
	${SDK_PATH}/zigbee/zcl/hvac/zcl_thermostat.c
	${SDK_PATH}/zigbee/zcl/smart_energy/zcl_metering.c
	${SDK_PATH}/zigbee/zcl/smart_energy/zcl_metering_attr.c
	${SDK_PATH}/zigbee/zcl/zll_commissioning/zcl_toucklink_security.c
	${SDK_PATH}/zigbee/zcl/zll_commissioning/zcl_zllTouchLinkDiscovery.c
	${SDK_PATH}/zigbee/zcl/zll_commissioning/zcl_zllTouchLinkJoinOrStart.c
	${SDK_PATH}/zigbee/zcl/zll_commissioning/zcl_zll_commissioning.c
	${SDK_PATH}/zigbee/zcl/commissioning/zcl_commissioning.c
	${SDK_PATH}/zigbee/zcl/commissioning/zcl_commissioning_attr.c
	${SDK_PATH}/zigbee/zcl/hvac/zcl_thermostat.c
	${SDK_PATH}/zigbee/zcl/measument_sensing/zcl_electrical_measurement.c
	${SDK_PATH}/zigbee/zcl/measument_sensing/zcl_electrical_measurement_attr.c
	${SDK_PATH}/zigbee/zcl/measument_sensing/zcl_illuminance_measurement.c
	${SDK_PATH}/zigbee/zcl/measument_sensing/zcl_illuminance_measurement_attr.c
	${SDK_PATH}/zigbee/zcl/measument_sensing/zcl_occupancy_sensing.c
	${SDK_PATH}/zigbee/zcl/measument_sensing/zcl_occupancy_sensing_attr.c
	${SDK_PATH}/zigbee/zcl/measument_sensing/zcl_temperature_measurement.c
	${SDK_PATH}/zigbee/zcl/measument_sensing/zcl_temperature_measurement_attr.c
	${SDK_PATH}/zigbee/zcl/light_color_control/zcl_light_colorCtrl.c
	${SDK_PATH}/zigbee/zcl/light_color_control/zcl_light_colorCtrl_attr.c
	${SDK_PATH}/zigbee/zcl/closures/zcl_door_lock.c
	${SDK_PATH}/zigbee/zcl/closures/zcl_door_lock_attr.c
	${SDK_PATH}/zigbee/zcl/closures/zcl_window_covering.c
	${SDK_PATH}/zigbee/zcl/closures/zcl_window_covering_attr.c
	${SDK_PATH}/zigbee/zcl/general/zcl_alarm.c
	${SDK_PATH}/zigbee/zcl/general/zcl_alarm_attr.c
	${SDK_PATH}/zigbee/zcl/general/zcl_basic.c
	${SDK_PATH}/zigbee/zcl/general/zcl_basic_attr.c
	${SDK_PATH}/zigbee/zcl/general/zcl_binary_input.c
	${SDK_PATH}/zigbee/zcl/general/zcl_binary_input_attr.c
	${SDK_PATH}/zigbee/zcl/general/zcl_binary_output.c
	${SDK_PATH}/zigbee/zcl/general/zcl_binary_output_attr.c
	${SDK_PATH}/zigbee/zcl/general/zcl_devTemperatureCfg.c
	${SDK_PATH}/zigbee/zcl/general/zcl_devTemperatureCfg_attr.c
	${SDK_PATH}/zigbee/zcl/general/zcl_diagnostics.c
	${SDK_PATH}/zigbee/zcl/general/zcl_diagnostics_attr.c
	${SDK_PATH}/zigbee/zcl/general/zcl_greenPower.c
	${SDK_PATH}/zigbee/zcl/general/zcl_greenPower_attr.c
	${SDK_PATH}/zigbee/zcl/general/zcl_group.c
	${SDK_PATH}/zigbee/zcl/general/zcl_group_attr.c
	${SDK_PATH}/zigbee/zcl/general/zcl_identify.c
	${SDK_PATH}/zigbee/zcl/general/zcl_identify_attr.c
	${SDK_PATH}/zigbee/zcl/general/zcl_level.c
	${SDK_PATH}/zigbee/zcl/general/zcl_level_attr.c
	${SDK_PATH}/zigbee/zcl/general/zcl_multistate_input.c
	${SDK_PATH}/zigbee/zcl/general/zcl_multistate_input_attr.c
	${SDK_PATH}/zigbee/zcl/general/zcl_multistate_output.c
	${SDK_PATH}/zigbee/zcl/general/zcl_multistate_output_attr.c
	${SDK_PATH}/zigbee/zcl/general/zcl_onoff.c
	${SDK_PATH}/zigbee/zcl/general/zcl_onoff_attr.c
	${SDK_PATH}/zigbee/zcl/general/zcl_pollCtrl.c
	${SDK_PATH}/zigbee/zcl/general/zcl_pollCtrl_attr.c
	${SDK_PATH}/zigbee/zcl/general/zcl_powerCfg.c
	${SDK_PATH}/zigbee/zcl/general/zcl_powerCfg_attr.c
	${SDK_PATH}/zigbee/zcl/general/zcl_scene.c
	${SDK_PATH}/zigbee/zcl/general/zcl_scene_attr.c
	${SDK_PATH}/zigbee/zcl/general/zcl_time.c
	${SDK_PATH}/zigbee/zcl/general/zcl_time_attr.c
	${SDK_PATH}/zigbee/zcl/security_safety/zcl_ias_ace.c
	${SDK_PATH}/zigbee/zcl/security_safety/zcl_ias_wd.c
	${SDK_PATH}/zigbee/zcl/security_safety/zcl_ias_wd_attr.c
	${SDK_PATH}/zigbee/zcl/security_safety/zcl_ias_zone.c
	${SDK_PATH}/zigbee/zcl/security_safety/zcl_ias_zone_attr.c
	${SDK_PATH}/zigbee/zcl/zcl_wwah/zcl_wwah.c
	${SDK_PATH}/zigbee/zcl/zcl_wwah/zcl_wwah_attr.c
	${SDK_PATH}/zigbee/zcl/ota_upgrading/zcl_ota.c
	${SDK_PATH}/zigbee/zcl/ota_upgrading/zcl_ota_attr.c
	${SDK_PATH}/zigbee/common/zb_config.c
	${SDK_PATH}/zigbee/af/zb_af.c
	${SDK_PATH}/zigbee/wwah/wwah.c
	${SDK_PATH}/zigbee/wwah/wwahEpCfg.c
	${SDK_PATH}/zigbee/gp/gp.c
	${SDK_PATH}/zigbee/gp/gpEpCfg.c
	${SDK_PATH}/zigbee/gp/gp_proxyTab.c
	${SDK_PATH}/zigbee/ss/ss_nv.c
	${SDK_PATH}/zigbee/ota/ota.c
	${SDK_PATH}/zigbee/ota/otaEpCfg.c
)

target_include_directories(zigbee PUBLIC ${HDR_FILES})
target_compile_options(zigbee PUBLIC ${GCC_FLAGS})

# Build SDK proj dependencies
add_library(proj OBJECT
	${SDK_PATH}/proj/common/list.c
	${SDK_PATH}/proj/common/mempool.c
	${SDK_PATH}/proj/common/tlPrintf.c
	${SDK_PATH}/proj/common/string.c
	${SDK_PATH}/proj/common/utility.c
	${SDK_PATH}/proj/drivers/drv_gpio.c
	${SDK_PATH}/proj/drivers/drv_nv.c
	${SDK_PATH}/proj/drivers/drv_pm.c
	${SDK_PATH}/proj/drivers/drv_flash.c
	${SDK_PATH}/proj/drivers/drv_putchar.c
	${SDK_PATH}/proj/drivers/drv_pwm.c
	${SDK_PATH}/proj/drivers/drv_adc.c
	${SDK_PATH}/proj/drivers/drv_timer.c
	${SDK_PATH}/proj/drivers/drv_uart.c
	${SDK_PATH}/proj/drivers/drv_hw.c
	${SDK_PATH}/proj/drivers/drv_calibration.c
	${SDK_PATH}/proj/os/ev.c
	${SDK_PATH}/proj/os/ev_buffer.c
	${SDK_PATH}/proj/os/ev_poll.c
	${SDK_PATH}/proj/os/ev_queue.c
	${SDK_PATH}/proj/os/ev_timer.c
)

target_include_directories(proj PUBLIC ${HDR_FILES})
target_compile_options(proj PUBLIC ${GCC_FLAGS})

# Build SDK platform dependencies
set(ASM_SRCS
	${SDK_PATH}/platform/tc32/div_mod.S
	${SDK_PATH}/platform/boot/link_cfg.S
	${SDK_PATH}/platform/boot/8258/cstartup_8258.S
)

set_source_files_properties(${ASM_SRCS} PROPERTIES LANGUAGE C)
add_library(platform_asm OBJECT ${ASM_SRCS})

add_library(platform OBJECT
	${SDK_PATH}/platform/chip_8258/adc.c
	${SDK_PATH}/platform/chip_8258/flash.c
	${SDK_PATH}/platform/services/b85m/irq_handler.c
	${SDK_PATH}/platform/chip_8258/flash/flash_mid13325e.c
)

target_include_directories(platform PUBLIC ${HDR_FILES})
target_compile_options(platform PUBLIC ${GCC_FLAGS})
target_include_directories(platform_asm PUBLIC ${HDR_FILES})
target_compile_options(platform_asm PUBLIC ${GCC_FLAGS} ${ASM_FLAGS})

# This project's sources
add_library(src OBJECT
	${SRC_PATH}/led.c
	${SRC_PATH}/main.c
	${SRC_PATH}/button.c
	${SRC_PATH}/battery.c
	${SRC_PATH}/clusters.c
	${SRC_PATH}/bdb_init.c
	${SRC_PATH}/occupancy.c
)

target_include_directories(src PUBLIC ${HDR_FILES})
target_compile_options(src PUBLIC ${GCC_FLAGS})

# Build main elf target and set its dependencies
add_executable(${ELF_TARGET})
target_include_directories(${ELF_TARGET} PUBLIC ${HDR_FILES})
target_link_options(${ELF_TARGET} PUBLIC ${GCC_FLAGS} ${LINK_FLAGS})
target_link_libraries(${ELF_TARGET} PUBLIC drivers_8258 zb_ed)
target_link_libraries(${ELF_TARGET} PUBLIC src zigbee proj platform platform_asm)

# Create binary
add_custom_command(
	TARGET ${ELF_TARGET} POST_BUILD
	COMMAND ${OBJCOPY} ARGS -v -O binary ${ELF_TARGET} ${BIN_TARGET}
)
