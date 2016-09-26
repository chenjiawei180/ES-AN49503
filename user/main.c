 /******************************************************************************
  * @file       USER/main.c
  * @author  cjw
  * @date     2016.9.22
  * @brief     This file contains the source of main function.
  ******************************************************************************/
	
#define OS_GLOBALS
/* system headers */

#include "common.h"
#include "ics.h"
#include "sysinit.h"

/* user headers */
#include "global.h"
#include "led.h"
#include "wdt.h"
#include "spi_app.h"

int main(void)
{
    uint32_t i;
    uint16_t data=0;
    /* Set the system clock */
    sysinit();

    /* WDT init */
    WDT_Init(1000);

    /* Led init */
    RED_Init();
    GREEN_Init();

    /* SPI0 init */
    SIM_RemapSPI0ToPTE_0_12_3(); //    remap SPI0 to pin PTE01/2/3
    SPI_APP_Init();
    
    while(1)
    {
        //for(i=0;i<0xffff;i++) WDT_Clear();
        WDT_Clear();
        //SPI_Read(DEV_ADD,0x01,&data);
        //SPI_Write(DEV_ADD,0x01,0x004a);
        GREEN_On();
        if( !SPI_Continue_Read(DEV_ADD, 0x01 , 10) )  GREEN_Off();
        else GREEN_On();
    }
}
