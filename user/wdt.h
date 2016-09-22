 /******************************************************************************
  * @file       USER/wdt.h
  * @author  cjw
  * @date     2016.9.22
  * @brief     This file contains the headers of watch dog timer.
  ******************************************************************************/

#ifndef __WDT_H
#define __WDT_H

#include "global.h"
#include "wdog.h"

void WDT_Init(uint16_t timeout);
void WDT_Clear(void);

#endif
