 /******************************************************************************
  * @file       USER/main.c
  * @author  cjw
  * @date     2016.9.22
  * @brief     This file contains the source of main function.
  ******************************************************************************/

/* system headers */
#include "common.h"
#include "ics.h"
#include "sysinit.h"

/* user headers */
#include "led.h"
#include "wdt.h"

int main(void)
{
    uint32_t i;
    /* Set the system clock */
    sysinit();

    /* WDT init */
    WDT_Init(1000);

    /* Led init */
    RED_Init();
    GREEN_Init();
    
    while(1)
    {
        //for(i=0;i<0xffff;i++); //WDT_Clear();
        GREEN_On();
        WDT_Clear();
    }
}
