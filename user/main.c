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

int main(void)
{
    uint32_t i;
    /* Set the system clock */
    sysinit();

    /* Led init */
    RED_Init();
    GREEN_Init();

    while(1)
    {
        for(i=0;i<0xfffff;i++);
    }
}
