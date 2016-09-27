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
#include "eeprom_24c.h"

int main(void)
{
    uint32_t i;
    uint16_t data=0;
    uint8_t i2c_buff[3]={0x04,0x05,0x06};
    uint8_t buff[3];
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

    /* I2C init */
    IIC_Init();

    while(1)
    {
        for(i=0;i<0xffff;i++) WDT_Clear();
        WDT_Clear();
        IIC_Send_Buffer(0X01, i2c_buff, 3);
        IIC_Read_Buffer(0X01, buff, 3);
        GREEN_On();
        if( buff[0] == 4 && buff[1] == 5 && buff[2] == 6 )  GREEN_Off();
        else GREEN_On();
    }
}
