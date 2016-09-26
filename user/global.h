 /******************************************************************************
  * @file       USER/global.h
  * @author  cjw
  * @date     2016.9.22
  * @brief     This file contains the all define .
  ******************************************************************************/
#ifndef __GLOBAL_H
#define __GLOBAL_H

#include "common.h"
#include "string.h"
#include "spi.h"

#ifdef OS_GLOBALS
#define OS_EXT
#else
#define OS_EXT extern
#endif

OS_EXT SPI_WidthType SPI_Conti_RecvData[100];

#define SPI_BIT_RATE                1000000

#endif
