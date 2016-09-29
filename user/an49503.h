 /******************************************************************************
  * @file       USER/an49503.h
  * @author  cjw
  * @date     2016.9.27
  * @brief     This file contains the define for An49503.
  ******************************************************************************/

#ifndef __AN49503_H
#define __AN49503_H

#include "global.h"

/* Regiseter */
/* 0x00 Reserved */
#define    PWR_CTRL_ADDR    ((uint8_t)0x01)
#define    SPIWD_CTRL_ADDR    ((uint8_t)0x02)
#define    FDRV_CTRL_ADDR    ((uint8_t)0x03)
#define    CVSEL_ADDR    ((uint8_t)0x04)
#define    GVSEL_ADDR    ((uint8_t)0x05)
#define    OUVCTL1_ADDR    ((uint8_t)0x06)
#define    OUVCTL2_ADDR    ((uint8_t)0x07)
#define    UVMSK_ADDR    ((uint8_t)0x08)
#define    OVMSK_ADDR    ((uint8_t)0x09)
#define    OP_MODE_ADDR    ((uint8_t)0x0A)
#define    LOCK_ADDR    ((uint8_t)0x0B)
#define    GPIO_CTRL1_ADDR    ((uint8_t)0x0C)
#define    GPIO_CTRL2_ADDR    ((uint8_t)0x0D)
#define    GPIO_CTRL3_ADDR    ((uint8_t)0x0E)
#define    GPIO_CTRL4_ADDR    ((uint8_t)0x0F)
/* 0x10 Reserved */
#define    ALARM_CTRL1_ADDR	((uint8_t)0x11)
#define    ALARM_CTRL2_ADDR	((uint8_t)0x12)
#define    ALARM_CTRL3_ADDR	((uint8_t)0x13)
#define    CB_CTL_ADDR    ((uint8_t)0x14)
#define    CBSEL_ADDR    ((uint8_t)0x15)
#define    GPIOSEL_ADDR    ((uint8_t)0x17)
#define    SPICTL_ADDR    ((uint8_t)0x18)
#define    ADCTRL1_ADDR    ((uint8_t)0x19)
#define    ADCTRL2_ADDR    ((uint8_t)0x1A)
#define    GPOH_ADDR    ((uint8_t)0x1B)
#define    INRCV1_ADDR    ((uint8_t)0x1C)
#define    INRCV2_ADDR    ((uint8_t)0x1D)
#define    INR_CTL_ADDR    ((uint8_t)0x1E)
/* 0x1F-0x20 Reserved */
#define    SPI_STAT_ADDR    ((uint8_t)0x21)
#define    MODE_STAT_ADDR    ((uint8_t)0x22)
/* 0x23-0x20 Reserved */
#define    FUSE_RADR_ADDR    ((uint8_t)0x2E)
#define    FUSE_DATA_ADDR    ((uint8_t)0x2F)
#define    STAT_ADDR    ((uint8_t)0x30)
#define    OVSTAT_ADDR    ((uint8_t)0x31)
#define    UVSTAT_ADDR    ((uint8_t)0x32)
#define    CV01_AD_ADDR    ((uint8_t)0x33)
#define    CV02_AD_ADDR    ((uint8_t)0x34)
#define    CV03_AD_ADDR    ((uint8_t)0x35)
#define    CV04_AD_ADDR    ((uint8_t)0x36)
#define    CV05_AD_ADDR    ((uint8_t)0x37)
#define    CV06_AD_ADDR    ((uint8_t)0x38)
#define    CV07_AD_ADDR    ((uint8_t)0x39)
#define    CV08_AD_ADDR    ((uint8_t)0x3A)
#define    CV09_AD_ADDR    ((uint8_t)0x3B)
#define    CV10_AD_ADDR    ((uint8_t)0x3C)
#define    CV11_AD_ADDR    ((uint8_t)0x3D)
#define    CV12_AD_ADDR    ((uint8_t)0x3E)
#define    CV13_AD_ADDR    ((uint8_t)0x3F)
#define    CV14_AD_ADDR    ((uint8_t)0x40)
#define    CV15_AD_ADDR    ((uint8_t)0x41)
#define    CV16_AD_ADDR    ((uint8_t)0x42)
#define    VPAC_AD_ADDR    ((uint8_t)0x43)
#define    TMONI1_AD_ADDR    ((uint8_t)0x44)
#define    TMONI2_AD_ADDR    ((uint8_t)0x45)
#define    TMONI3_AD_ADDR    ((uint8_t)0x46)
#define    TMONI4_AD_ADDR    ((uint8_t)0x47)
#define    TMONI5_AD_ADDR    ((uint8_t)0x48)
#define    VDD50_AD_ADDR    ((uint8_t)0x49)
#define    GPIO1_AD_ADDR    ((uint8_t)0x4A)
#define    GPIO2_AD_ADDR    ((uint8_t)0x4B)
#define    CVIH_AD_ADDR    ((uint8_t)0x4C)
#define    CVIL_AD_ADDR    ((uint8_t)0x4D)
/* 0x4E-0x51 Reserved */
#define    OVL_STAT_ADDR    ((uint8_t)0x52)
#define    UVL_STAT_ADDR    ((uint8_t)0x53)
#define    FAILSTAT_ADDR    ((uint8_t)0x54)
#define    FDRVSTAT_ADDR    ((uint8_t)0x55)
#define    CBSTAT_ADDR    ((uint8_t)0x56)
/* 0x57^0x7F Reserved */

#endif

