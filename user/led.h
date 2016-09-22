 /******************************************************************************
  * @file       USER/led.h
  * @author  cjw
  * @date     2016.9.22
  * @brief     This file contains the headers of Led.
  ******************************************************************************/

#ifndef __LED_H
#define __LED_H

#include "global.h"
#include "gpio.h"

void RED_Init(void);
void RED_On(void);
void RED_Off(void);
void RED_Toggle(void);

void GREEN_Init(void);
void GREEN_On(void);
void GREEN_Off(void);
void GREEN_Toggle(void);

#endif
