 /******************************************************************************
  * @file       USER/crc.h
  * @author  cjw
  * @date     2016.9.23
  * @brief     This file contains the headers of CRC of SPI trans for An49503.
  ******************************************************************************/

#ifndef __CRC_H
#define __CRC_H

#include "global.h"

uint8_t CRC_Calc(uint8_t size,uint8_t* index);

#endif

