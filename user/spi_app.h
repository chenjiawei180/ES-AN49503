 /******************************************************************************
  * @file       USER/spi_app.h
  * @author  cjw
  * @date     2016.9.22
  * @brief     This file contains the headers of Spi_app.
  ******************************************************************************/

#ifndef __SPI_APP_H
#define __SPI_APP_H

#include "global.h"
#include "spi.h"
#include "an49503.h"

#define MAC_SPI_READ_RETRY 8
#define AN49503_READ_CNT 0x56

#define DEV_ADD 0xE0

void SPI_APP_Init(void);
void SPI_En(void);
void SPI_Dis(void);
uint8_t SPI_Read(uint8_t devadd,uint8_t regadd,uint16_t* data);
uint8_t SPI_Write(uint8_t devadd,uint8_t regadd,uint16_t data);
uint8_t SPI_Continue_Read(uint8_t devadd,uint8_t regadd, uint8_t time);

#endif

