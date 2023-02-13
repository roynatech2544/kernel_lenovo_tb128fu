/*
 * Copyright (C) 2010 - 2021 Novatek, Inc.
 *
 * $Revision: 85753 $
 * $Date: 2021-07-27 17:21:08 +0800 (週二, 27 七月 2021) $
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 */
#ifndef 	_LINUX_NVT_TOUCH_H
#define		_LINUX_NVT_TOUCH_H

#include <linux/delay.h>
#include <linux/input.h>
#include <linux/of.h>
#include <linux/spi/spi.h>
#include <linux/uaccess.h>

#ifdef CONFIG_HAS_EARLYSUSPEND
#include <linux/earlysuspend.h>
#endif

#include "nt36xxx_mem_map.h"

#ifdef CONFIG_MTK_SPI
/* Please copy mt_spi.h file under mtk spi driver folder */
#include "mt_spi.h"
#endif

#ifdef CONFIG_SPI_MT65XX
#include <linux/platform_data/spi-mt65xx.h>
#endif

#define NVT_DEBUG 1

//---GPIO number---
#define NVTTOUCH_RST_PIN 1253
#define NVTTOUCH_INT_PIN 1247


//---INT trigger mode---
//#define IRQ_TYPE_EDGE_RISING 1
//#define IRQ_TYPE_EDGE_FALLING 2
#define INT_TRIGGER_TYPE IRQ_TYPE_EDGE_RISING


//---SPI driver info.---
#define NVT_SPI_NAME "NVT-ts"

#if NVT_DEBUG
#define NVT_LOG(fmt, args...)    pr_err("[%s] %s %d: " fmt, NVT_SPI_NAME, __func__, __LINE__, ##args)
#else
#define NVT_LOG(fmt, args...)    pr_info("[%s] %s %d: " fmt, NVT_SPI_NAME, __func__, __LINE__, ##args)
#endif
#define NVT_ERR(fmt, args...)    pr_err("[%s] %s %d: " fmt, NVT_SPI_NAME, __func__, __LINE__, ##args)

//---Input device info.---
#define NVT_TS_NAME "NVTCapacitiveTouchScreen"
#define NVT_PEN_NAME "NVTCapacitivePen"

//---Touch info.---
#define TOUCH_DEFAULT_MAX_WIDTH 1200
#define TOUCH_DEFAULT_MAX_HEIGHT 2000
#define TOUCH_MAX_FINGER_NUM 10
#define TOUCH_KEY_NUM 0
#if TOUCH_KEY_NUM > 0
extern const uint16_t touch_key_array[TOUCH_KEY_NUM];
#endif
#define TOUCH_FORCE_NUM 1000
//---for Pen---
#define PEN_PRESSURE_MAX (4095)
#define PEN_DISTANCE_MAX (1)
#define PEN_TILT_MIN (-60)
#define PEN_TILT_MAX (60)

/* Enable only when module have tp reset pin and connected to host */
#define NVT_TOUCH_SUPPORT_HW_RST 	0

//---Customerized func.---
#define NVT_TOUCH_PROC 1
#define NVT_TOUCH_EXT_PROC 1
//+OAK1911,shenwenbin.wt,ADD,20211228,add penraw node for customer
#define NVT_TOUCH_EXT_SYSFS 1
//-OAK1911,shenwenbin.wt,ADD,20211228,add penraw node for customer
#define NVT_TOUCH_MP 1
#define MT_PROTOCOL_B 1
//+OAK78,shenwenbin.wt,MOD,20211115,tuning double wakeup
#define WAKEUP_GESTURE 1
//-OAK78,shenwenbin.wt,MOD,20211115,tuning double wakeup
#if WAKEUP_GESTURE
extern const uint16_t gesture_key_array[];
extern uint8_t Double_WakeUp_Status(void);	//OAK78,shenwenbin.wt,MOD,20211115,tuning double wakeup
#endif
#define BOOT_UPDATE_FIRMWARE 1
#define BOOT_UPDATE_FIRMWARE_NAME "novatek_ts_fw.bin"
#define MP_UPDATE_FIRMWARE_NAME   "novatek_ts_mp.bin"
#define POINT_DATA_CHECKSUM 1
#define POINT_DATA_CHECKSUM_LEN 65

//---ESD Protect.---
#define NVT_TOUCH_ESD_PROTECT 0
#define NVT_TOUCH_ESD_CHECK_PERIOD 1500	/* ms */
#define NVT_TOUCH_WDT_RECOVERY 1

#define CHECK_PEN_DATA_CHECKSUM 0

//+OAK700,shenwenbin.wt,add,20211204,add charge mode
#define NVT_CHARGER_NOTIFIER_CALLBACK		1
//-OAK700,shenwenbin.wt,add,20211204,add charge mode

struct nvt_ts_data {
	struct spi_device *client;
	struct input_dev *input_dev;
	struct delayed_work nvt_fwu_work;
	uint16_t addr;
	int8_t phys[32];
#if defined(CONFIG_FB)
#if defined(CONFIG_DRM_PANEL)
	struct notifier_block drm_panel_notif;
#elif defined(_MSM_DRM_NOTIFY_H_)
	struct notifier_block drm_notif;
#else
	struct notifier_block fb_notif;
#endif
#elif defined(CONFIG_HAS_EARLYSUSPEND)
	struct early_suspend early_suspend;
#endif
	uint8_t fw_ver;
	uint8_t x_num;
	uint8_t y_num;
	uint16_t abs_x_max;
	uint16_t abs_y_max;
	uint8_t max_touch_num;
	uint8_t max_button_num;
	uint32_t int_trigger_type;
	int32_t irq_gpio;
	uint32_t irq_flags;
	int32_t reset_gpio;
	uint32_t reset_flags;
	struct mutex lock;
	const struct nvt_ts_mem_map *mmap;
	uint8_t hw_crc;
	uint16_t nvt_pid;
	uint8_t *rbuf;
	uint8_t *xbuf;
	struct mutex xbuf_lock;
	bool irq_enabled;
	bool pen_support;
	bool stylus_resol_double;
	uint8_t x_gang_num;
	uint8_t y_gang_num;
	struct input_dev *pen_input_dev;
	int8_t pen_phys[32];
#ifdef CONFIG_MTK_SPI
	struct mt_chip_conf spi_ctrl;
#endif
#ifdef CONFIG_SPI_MT65XX
    struct mtk_chip_config spi_ctrl;
#endif
	uint8_t edge_reject_state;
//+OAK700,shenwenbin.wt,add,20211204,add charge mode
#if NVT_CHARGER_NOTIFIER_CALLBACK
    struct notifier_block notifier_charger;
    struct workqueue_struct *nvt_charger_wq;
    struct work_struct  nvt_charger_work;
    uint8_t charger_mode;
    uint8_t update_floating;
#endif
//-OAK700,shenwenbin.wt,add,20211204,add charge mode
	//+OAK4146,shenwenbin.wt,ADD,20220125,add TP game mode
	uint8_t game_mode_state;
	int32_t edge_cmd_param[4];
	//-OAK4146,shenwenbin.wt,ADD,20220125,add TP game mode
};

#if NVT_TOUCH_PROC
struct nvt_flash_data{
	rwlock_t lock;
};
#endif

typedef enum {
	RESET_STATE_INIT = 0xA0,// IC reset
	RESET_STATE_REK,		// ReK baseline
	RESET_STATE_REK_FINISH,	// baseline is ready
	RESET_STATE_NORMAL_RUN,	// normal run
	RESET_STATE_MAX  = 0xAF
} RST_COMPLETE_STATE;

typedef enum {
    EVENT_MAP_HOST_CMD                      = 0x50,
    EVENT_MAP_HANDSHAKING_or_SUB_CMD_BYTE   = 0x51,
    EVENT_MAP_RESET_COMPLETE                = 0x60,
    EVENT_MAP_FWINFO                        = 0x78,
    EVENT_MAP_PROJECTID                     = 0x9A,
} SPI_EVENT_MAP;

//+OAK1911,shenwenbin.wt,ADD,20211228,add penraw node for customer
#if NVT_TOUCH_EXT_SYSFS
#define NOVA_MAX_BUFFER		32
#define MAX_IO_CONTROL_REPORT	16

enum pen_data_type{
    DATA_TYPE_RAW = 0
};

enum toch_point_status{
	TS_NONE,
	TS_RELEASE,
	TS_TOUCH,
};

struct nova_pen_coords_buffer {
    signed char status;
    signed char tool_type;
    signed char tilt_x;
    signed char tilt_y;
    unsigned long int x;
    unsigned long int y;
    unsigned long int p;
};

struct nova_pen_info {
	unsigned char frame_no;
	unsigned char data_type;
	unsigned char reserve[2];
	struct nova_pen_coords_buffer coords;
};

struct io_pen_report {
	unsigned char report_num;
	unsigned char reserve[3];
	struct nova_pen_info pen_info[MAX_IO_CONTROL_REPORT];
};
#endif
//-OAK1911,shenwenbin.wt,ADD,20211228,add penraw node for customer

//---SPI READ/WRITE---
#define SPI_WRITE_MASK(a)	(a | 0x80)
#define SPI_READ_MASK(a)	(a & 0x7F)

#define DUMMY_BYTES (1)
#define NVT_TRANSFER_LEN	(63*1024)
#define NVT_READ_LEN		(2*1024)
#define NVT_XBUF_LEN		(NVT_TRANSFER_LEN+1+DUMMY_BYTES)

typedef enum {
	NVTWRITE = 0,
	NVTREAD  = 1
} NVT_SPI_RW;

//---extern structures---
extern struct nvt_ts_data *ts;

//---extern functions---
int32_t CTP_SPI_READ(struct spi_device *client, uint8_t *buf, uint16_t len);
int32_t CTP_SPI_WRITE(struct spi_device *client, uint8_t *buf, uint16_t len);
void nvt_bootloader_reset(void);
void nvt_eng_reset(void);
void nvt_sw_reset(void);
void nvt_sw_reset_idle(void);
void nvt_boot_ready(void);
void nvt_bld_crc_enable(void);
void nvt_fw_crc_enable(void);
void nvt_tx_auto_copy_mode(void);
int32_t nvt_update_firmware(char *firmware_name);
int32_t nvt_check_fw_reset_state(RST_COMPLETE_STATE check_reset_state);
int32_t nvt_get_fw_info(void);
int32_t nvt_clear_fw_status(void);
int32_t nvt_check_fw_status(void);
int32_t nvt_check_spi_dma_tx_info(void);
int32_t nvt_set_page(uint32_t addr);
int32_t nvt_write_addr(uint32_t addr, uint8_t data);
#if NVT_TOUCH_ESD_PROTECT
extern void nvt_esd_check_enable(uint8_t enable);
#endif /* #if NVT_TOUCH_ESD_PROTECT */
//+OAK700,shenwenbin.wt,add,20211204,add charge mode
#if NVT_CHARGER_NOTIFIER_CALLBACK
void nvt_set_charger_mode(void);
#endif
int32_t nvt_edge_reject_set(int32_t status);
//-OAK700,shenwenbin.wt,add,20211204,add charge mode

#endif /* _LINUX_NVT_TOUCH_H */
